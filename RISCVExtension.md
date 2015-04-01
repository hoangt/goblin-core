#RISC-V Instruction Set Extensions.

# Introduction #

The following list denotes RISC-V ISA Extensions.  Our extension assumes the use of the base IMAFD ISA as detailed in the RISC-V ISA Spec: http://riscv.org/download.html#tab_isaspec


# Instructions #

Format: Instruction: Mnemonic : Description
  * Scatter : _scatter Ra, Rb, Rt_ : Scatter Memory Instruction
  * Gather : _gather Rt, Ra, Rb_ : Gather Memory Instruction
  * Task Spawn : _spawn Rt,Ra_ : Spawn a task using the task context at Ra and return the status in Rt
  * Task Join : _join Rt, Ra_ : Join the task using the context at Ra and return the status in Rt
  * Instruction Wait : _iwait rd,rs1,rs2_ : Pend the current task until register at index Rt clears its hazard
  * Context Switch : _ctxsw_ : Force a context switch of the current task
  * Set Key : _skey Ra, gkey_ : Set the gkey to value Ra **privileged instruction**
  * Load-Modify After : _ldma Rt, Ra, Rb_ : Load the address at Ra into Rt and add Rb to Ra. Rt=load(Ra); Ra+= Rb
  * Store-Modify After : _stma Ra, Rb, Rt_ : Store the value at Ra into the address at Rt and add Rb to Rt. Rt=store(Ra); Rt+=Rb
  * Load-Modify Before: _ldmb Rt, Ra, Rb_ : Add Rb to Ra and load the address at Ra into Rt.  Ra+=Rb; Rt=load(Ra).
  * Store-Modify Before: _stmb Ra,Rb,Rt_ : Add Rt to Ra and store the value at Ra to Rt. Rt+=Rb; Rt=store(Ra)
  * Get TaskCTX : _gettask Rt, tctx_ : Retrieve the task context value and write the contents to Rt
  * Set TaskCTX: _settask Ra, tctx_ : Set the task ID to Ra
  * Get TaskID: _gettid Rt, tid : Retrieve the task ID value and writ the contents to Rt
  * Get Gconst :_getgconst Rt, gconst : Retrieve the GC64 constant register value