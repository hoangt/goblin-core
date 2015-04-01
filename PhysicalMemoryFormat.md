# Introduction #

This page describes the physical memory addressing format for the GC64 architecture.  The physical memory format provides 50 bits of physical address space within a local node that covers:
  * Hybrid Memory Cube Addressing [memory](main.md)
  * Scratchpad Memory [managed shared scratchpad](software.md)
  * Partitioned/Global Memory


# Address Fields #

The physical address fields are defined as follows {[bits](bits.md) : Field}
  * [33:0]  : Base physical address : Defines the base physical address
  * [37:34] : CUB Field : Defines the base address's physical location; corresponds to an HMC Cube ID [CUB](CUB.md).  0xF refers to the scratchpad.
  * [41:38] : Reserved for future expansion
  * [49:42] : Socket ID. Refers to the attached physical socket ID within the given node
  * [63:50] : Unused/Ignored