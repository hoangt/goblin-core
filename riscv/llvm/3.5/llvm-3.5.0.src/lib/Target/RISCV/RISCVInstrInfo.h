//===-- RISCVInstrInfo.h - RISCV instruction information ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the RISCV implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_RISCVINSTRINFO_H
#define LLVM_TARGET_RISCVINSTRINFO_H

#include "RISCV.h"
#include "RISCVRegisterInfo.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "RISCVGenInstrInfo.inc"

namespace llvm {

class RISCVTargetMachine;

namespace RISCVII {
  enum {
    // See comments in RISCVInstrFormats.td.
    SimpleLoad  = (1 << 0),
    SimpleStore = (1 << 1)
  };
  // RISCV MachineOperand target flags.
  enum {
    // Masks out the bits for the access model.
    MO_SYMBOL_MODIFIER = (1 << 0),

    // @GOT (aka @GOTENT)
    MO_GOT = (1 << 0),

    MO_ABS_HI,
    MO_ABS_LO,
    MO_TPREL_HI,
    MO_TPREL_LO
  };
}

class RISCVInstrInfo : public RISCVGenInstrInfo {
  const RISCVRegisterInfo RI;
  const RISCVTargetMachine &TM;

  void splitMove(MachineBasicBlock::iterator MI, unsigned NewOpcode) const;
  void splitAdjDynAlloc(MachineBasicBlock::iterator MI) const;

public:
  explicit RISCVInstrInfo(RISCVTargetMachine &TM);

  // Override TargetInstrInfo.
  virtual unsigned isLoadFromStackSlot(const MachineInstr *MI,
                                       int &FrameIndex) const override;
  virtual unsigned isStoreToStackSlot(const MachineInstr *MI,
                                      int &FrameIndex) const override;
  void adjustStackPtr(unsigned SP, int64_t Amount,
                                     MachineBasicBlock &MBB,
                                     MachineBasicBlock::iterator I) const;
  unsigned GetInstSizeInBytes(MachineInstr *I) const;
  virtual bool AnalyzeBranch(MachineBasicBlock &MBB,
                             MachineBasicBlock *&TBB,
                             MachineBasicBlock *&FBB,
                             SmallVectorImpl<MachineOperand> &Cond,
                             bool AllowModify) const override;
  virtual unsigned RemoveBranch(MachineBasicBlock &MBB) const override;
  virtual unsigned InsertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                                MachineBasicBlock *FBB,
                                const SmallVectorImpl<MachineOperand> &Cond,
                                DebugLoc DL) const override;
  virtual unsigned InsertBranchAtInst(MachineBasicBlock &MBB, MachineInstr *I,
                                MachineBasicBlock *TBB,
                                const SmallVectorImpl<MachineOperand> &Cond,
                                DebugLoc DL) const;
  virtual unsigned InsertConstBranchAtInst(MachineBasicBlock &MBB, MachineInstr *I,
                                int64_t offset,
                                const SmallVectorImpl<MachineOperand> &Cond,
                                DebugLoc DL) const;
  virtual void copyPhysReg(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI, DebugLoc DL,
                           unsigned DestReg, unsigned SrcReg,
                           bool KillSrc) const override;
  virtual void
    storeRegToStackSlot(MachineBasicBlock &MBB,
                        MachineBasicBlock::iterator MBBI,
                        unsigned SrcReg, bool isKill, int FrameIndex,
                        const TargetRegisterClass *RC,
                        const TargetRegisterInfo *TRI) const override;
  virtual void
    loadRegFromStackSlot(MachineBasicBlock &MBB,
                         MachineBasicBlock::iterator MBBI,
                         unsigned DestReg, int FrameIdx,
                         const TargetRegisterClass *RC,
                         const TargetRegisterInfo *TRI) const override;
  virtual bool
    expandPostRAPseudo(MachineBasicBlock::iterator MBBI) const override;
  virtual bool
    ReverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const
    override;

  // Return the RISCVRegisterInfo, which this class owns.
  const RISCVRegisterInfo &getRegisterInfo() const { return RI; }

  // Return true if MI is a conditional or unconditional branch.
  // When returning true, set Cond to the mask of condition-code
  // values on which the instruction will branch, and set Target
  // to the operand that contains the branch target.  This target
  // can be a register or a basic block.
  bool isBranch(const MachineInstr *MI, SmallVectorImpl<MachineOperand> &Cond,
                const MachineOperand *&Target) const;

  // Get the load and store opcodes for a given register class.
  void getLoadStoreOpcodes(const TargetRegisterClass *RC,
                           unsigned &LoadOpcode, unsigned &StoreOpcode) const;

  // Opcode is the opcode of an instruction that has an address operand,
  // and the caller wants to perform that instruction's operation on an
  // address that has displacement Offset.  Return the opcode of a suitable
  // instruction (which might be Opcode itself) or 0 if no such instruction
  // exists.
  unsigned getOpcodeForOffset(unsigned Opcode, int64_t Offset) const;

  // Emit code before MBBI in MI to move immediate value Value into
  // physical register Reg.
  void loadImmediate(MachineBasicBlock &MBB,
                     MachineBasicBlock::iterator MBBI,
                     unsigned *Reg, int64_t Value) const;
};
} // end namespace llvm

#endif
