# Introduction #

This page introduces the GC64 extensions to the RISCV simulator infrastructure "spike".  The original RISCV simulator source code is hosted on Github here: https://github.com/ucb-bar/riscv-isa-sim

The current GC64 simulator extensions includes support for the software-managed scratchpad infrastructure for each socket.  The simulator has explicit knowledge of the address space (address range) for which the scratchpad maps.  By default, the scratchpad addresses range from 0x0000000080000000 to 0x0000000080000000+SIZE, where size is the size of the respective scratchpad.  Do not rely on these addresses explicitly.  Always utilize the necessary library API's to access scratchpad memory.

# Download the Source #
The source code can be checked out of SVN here:
https://goblin-core.googlecode.com/svn/tags/riscv/gc64-sim-ALPHA-RELEASE-R374

This is currently the best released version.

# Building From Source #

Building the scratchpad interfaces can be performed using the standard RISCV spike build as outlined here: http://riscv.org/download.html#tab_isa-sim

The build instructions assume that the $RISCV environment variables references your install location for the remaining RISCV tools.

```
$> cd gc64-sim
$> mkdir build
$> cd build
$> ../configure --prefix=$RISCV --with-fesvr=$RISCV
$> make
$> make install 
```


# Executing the Simulator #

The simulator is also executed in the same manner as the standard RISCV spike simulator.  However, we add an additional option [-s NUM\_BYTES] that permits the user to describe the size (in bytes) of the scratchpad space.

```
$> spike pk hello_world.exe
$> spike -s 1024000 pk hello_world.exe 
```