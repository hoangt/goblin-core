//===-- RISCVBranchSelector.cpp - Emit long conditional branches ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a pass that scans a machine function to determine which
// conditional branches need more than 12 bits of displacement to reach their
// target basic block.  It does this in two passes; a calculation of basic block
// positions pass, and a branch pseudo op to machine branch opcode pass.  This
// pass should be run last, just before the assembly printer.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "riscv-branch-select"
#include "RISCV.h"
#include "RISCVInstrBuilder.h"
#include "RISCVInstrInfo.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Target/TargetMachine.h"
using namespace llvm;

STATISTIC(NumExpanded, "Number of branches expanded to long format");

namespace llvm {
  void initializeRISCVBSelPass(PassRegistry&);
}

namespace {
  struct RISCVBSel : public MachineFunctionPass {
    static char ID;
    RISCVBSel() : MachineFunctionPass(ID) {
      initializeRISCVBSelPass(*PassRegistry::getPassRegistry());
    }

    /// BlockSizes - The sizes of the basic blocks in the function.
    std::vector<unsigned> BlockSizes;

    virtual bool runOnMachineFunction(MachineFunction &Fn);

    const char *getPassName() const override {
      return "RISCV Branch Selector";
    }
  };
  char RISCVBSel::ID = 0;
}

INITIALIZE_PASS(RISCVBSel, "riscv-branch-select", "RISCV Branch Selector",
                false, false)

/// createRISCVBranchSelectionPass - returns an instance of the Branch Selection
/// Pass
///
FunctionPass *llvm::createRISCVBranchSelectionPass() {
  return new RISCVBSel();
}

bool RISCVBSel::runOnMachineFunction(MachineFunction &Fn) {
  const RISCVInstrInfo *TII =
                static_cast<const RISCVInstrInfo*>(Fn.getTarget().getInstrInfo());
  // Give the blocks of the function a dense, in-order, numbering.
  Fn.RenumberBlocks();
  BlockSizes.resize(Fn.getNumBlockIDs());

  // Measure each MBB and compute a size for the entire function.
  unsigned FuncSize = 0;
  for (MachineFunction::iterator MFI = Fn.begin(), E = Fn.end(); MFI != E;
       ++MFI) {
    MachineBasicBlock *MBB = MFI;

    unsigned BlockSize = 0;
    for (MachineBasicBlock::iterator MBBI = MBB->begin(), EE = MBB->end();
         MBBI != EE; ++MBBI)
      BlockSize += TII->GetInstSizeInBytes(MBBI);
    
    BlockSizes[MBB->getNumber()] = BlockSize;
    FuncSize += BlockSize;
  }
  
  // If the entire function is smaller than the displacement of a branch field,
  // we know we don't need to shrink any branches in this function.  This is a
  // common case.
  if (FuncSize < (1 << 11)) {
    BlockSizes.clear();
    return false;
  }
  
  // For each conditional branch, if the offset to its destination is larger
  // than the offset field allows, transform it into a long branch sequence
  // like this:
  //   short branch:
  //     bCC MBB
  //   long branch:
  //     b!CC $PC+8
  //     j MBB
  //
  bool MadeChange = true;
  bool EverMadeChange = false;
  while (MadeChange) {
    // Iteratively expand branches until we reach a fixed point.
    MadeChange = false;
  
    for (MachineFunction::iterator MFI = Fn.begin(), E = Fn.end(); MFI != E;
         ++MFI) {
      MachineBasicBlock &MBB = *MFI;
      unsigned MBBStartOffset = 0;
      for (MachineBasicBlock::iterator I = MBB.begin(), E = MBB.end();
           I != E; ++I) {
        MachineBasicBlock *Dest = 0;
        //All RISCV Branches have their dest MBB as the first machine operand
        SmallVector<MachineOperand, 4> Cond;
        Cond.push_back(MachineOperand::CreateImm(0));
        const MachineOperand *DestOp;

        //const MachineInstr *const_i = I;
        if (!TII->isBranch(I, Cond, DestOp)){
          MBBStartOffset += TII->GetInstSizeInBytes(I);
          continue;
        }

        MachineBasicBlock *FBB = 0;
        Cond.clear();
        if(TII->AnalyzeBranch(MBB, Dest, FBB, Cond, false)){
          //we can't fix this branch since we can't even analyze it
          MBBStartOffset += TII->GetInstSizeInBytes(I);
          continue;
        }

        if(Cond.empty() || Cond[0].getImm() == RISCV::CCMASK_ANY){
          MBBStartOffset += TII->GetInstSizeInBytes(I);
          continue;
        }
        
        // Determine the offset from the current branch to the destination
        // block.
        int BranchSize;
        if (Dest->getNumber() <= MBB.getNumber()) {
          // If this is a backwards branch, the delta is the offset from the
          // start of this block to this branch, plus the sizes of all blocks
          // from this block to the dest.
          BranchSize = MBBStartOffset;
          
          for (unsigned i = Dest->getNumber(), e = MBB.getNumber(); i != e; ++i)
            BranchSize += BlockSizes[i];
        } else {
          // Otherwise, add the size of the blocks between this block and the
          // dest to the number of bytes left in this block.
          BranchSize = -MBBStartOffset;

          for (unsigned i = MBB.getNumber(), e = Dest->getNumber(); i != e; ++i)
            BranchSize += BlockSizes[i];
        }

        // If this branch is in range, ignore it.
        if (isInt<12>(BranchSize)) {
          MBBStartOffset += 4;
          continue;
        }

        // Otherwise, we have to expand it to a long branch.
        MachineInstr *OldBranch = I;
        DebugLoc dl = OldBranch->getDebugLoc();
 
        TII->ReverseBranchCondition(Cond);
        TII->InsertConstBranchAtInst(MBB, I, 8, Cond, dl);

        // Uncond branch to the real destination.
        I = BuildMI(MBB, I, dl, TII->get(RISCV::J)).addMBB(Dest);

        // Remove the old branch from the function.
        OldBranch->eraseFromParent();
        
        // Remember that this instruction is 8-bytes, increase the size of the
        // block by 4, remember to iterate.
        BlockSizes[MBB.getNumber()] += 4;
        MBBStartOffset += 8;
        ++NumExpanded;
        MadeChange = true;
      }
    }
    EverMadeChange |= MadeChange;
  }
  
  BlockSizes.clear();
  return true;
}

