# Introduction #

This page is dedicated to those interested in building and using the HMC simulation framework as developed in conjunction with the GC64 architecture research effort.  This guide assumes basic knowledge of Unix/Linux command line prompts, building software in the aforementioned environment and the HMC devices themselves.  If you're looking for an introduction into the HMC device, we would suggest reading our publication from LSPP 2014 http://discl.cs.ttu.edu/lib/exe/fetch.php?media=wiki:papers:lspp14_jleidel_camera_ready_ieee.pdf.

Any bugs encountered in the following process can be submitted here:
https://code.google.com/p/goblin-core/issues/list

# HMC-Sim Overview #
The HMC simulation framework (or HMC-Sim) is designed to provide a very low-level simulation environment for any of the supported HMC 1.0 configurations.  We currently do not support HMC version 2.0 device configurations.  The simulation environment operates on individual clock boundaries in order to provide sample flow control between registered device buffers, FIFOs and link boundaries.  In this manner, the tracing output can display individual stall events on minute hardware components.  HMC-Sim also supports the basic ability to chain multiple HMC devices together and instantiate the HMC routing protocol using multiple CUB id's.  At this time, HMC-Sim supports all the current 1.0 packet types, including the JTAG interfaces.

The simulation framework is implemented as a standard C library with an associated API.  It is designed to be embedded in larger simulation frameworks (such as the GC64 simulator), or operated using driver applications.  The sample applications provided with the source code are examples of the latter.  They essentially instantiate basic HMC packets for typical memory I/O (reads, writes, etc).

# Retrieving the Source #

Given that the HMC-Sim source is built as a stand-alone library, it is not necessary to retrieve the entire GC64 source tree.  Checking out the code requires the use of Subversion (SVN).  The code can be checked out read-only using the following command:
```
$> svn checkout http://goblin-core.googlecode.com/svn/trunk/sim/hmc_sim hmc_sim
```

This places the source code in the "hmc\_sim" directory.  If you desire to update your source tree (as the development progresses), you may do so using the following commands:
```
$> cd hmc_sim 
$> svn update
```

At this point, you will be required to rebuild the source.

# Tested Platforms #
  * Mac OSX 13.4.0: gcc 4.2.1; clang 3.3
  * OpenSuSE (linux) 13.1; kernel 3.11.10-25-desktop; gcc 4.8.1

# Building the Source #

Once you have checked out the source, you will need to build it.  By default, the source is built with the GCC compiler.  The source is known to also build with the LLVM/Clang compiler as well.  The project source tree contains all the necessary makefiles to build the source.  If you have "gcc" in your default PATH environment variable, you may build the source as follows:
```
$> cd hmc_sim 
$> make
```
This should build "libhmcsim.a".

If you desire to modify the build environment (such as modifying the compiler, the compiler options, etc), you'll need to edit the "Makefile.inc" file.  This contains all the configurable build options.  If you seek to simply test the library, you shouldn't need to modify any of these options.

# Building the Sample Code #

The source code contains a number of sample driver applications that represent the following memory I/O scenarios:
  * Simple : a simple test driver that can be used as an example
  * Stream : a test driver that mimics the Stream Triad memory I/O pattern
  * GUPS : a test driver that mimics the HPCC RandomAccess I/O pattern
  * HMC\_Physrand : a test driver that executes a simple randomized I/O pattern
  * Decode\_Physrand : the same test as HMC\_Physrand, but exhibits how to decode response packets
  * Several other test directories exist, but are not yet completed

The tests can be built using the top-level makefile as follows (from the hmc\_sim directory):
```
$> make test
```

# Executing the Tests #

Each of the test drivers may accept special options depending upon the complexity of the respective code.  Each of the tests build an executable in the respective source directory.  For example, the "simple" executable is built and located in:
```
~/hmc_sim/test/simple/
```
Each of the executables accepts the standard "-h" option that displays the known command line options.  For example, executing "sample -h" displays the following:
```
$> ./simple -h
usage : ./simple -bcdhlnqvx
 -b <num_banks>
 -c <capacity>
 -d <num_drams>
 -h ...print help
 -l <num_links>
 -n <num_devs>
 -q <queue_depth>
 -v <num_vaults>
 -x <xbar_depth>
```

Notice that the options specify the device parameters associated with one or more HMC devices.  Remember, the HMC-Sim internal API logic will perform the necessary checks to ensure that the requested hardware configuration is valid.  As such, if you specify an invalid configuration, the application will likely fail.  An example of successful test options resembles the following:
```
$> ./simple -b 8 -c 2 -d 20 -l 4 -n 1 -q 64 -v 16 -x 128
SUCCESS : INITALIZED HMCSIM
SUCCESS : SET MAXIMUM BLOCK SIZE
SUCCESS : FREE'D HMCSIM
```

A second example would be to execute the "hmc\_physrand" test application.  This application test includes a series of driver scripts that drive common configurations.  These scripts were also utilized in the tests for the aforementioned LSPP '14 paper.  These scripts are located in:
```
~/hmc_sim/test/hmc_physrand/scripts/
```

The "physrand" executable is located at:
```
~/hmc_sim/test/hmc_physrand/physrand
```

A simple example of executing an hmc-physrand test would resemble the following:
```
$> cd hmc_sim/test/hmc_physrand/scripts/
$> sh ./smalltest.sh
SUCCESS : INITALIZED HMCSIM
SUCCESS : INITIALIZED LINK 0
SUCCESS : INITIALIZED LINK 1
SUCCESS : INITIALIZED LINK 2
SUCCESS : INITIALIZED LINK 3
SUCCESS : INITALIZED MAX BLOCK SIZE
SUCCESS : INITIALIZED TRACE HANDLERS
SUCCESS : ZERO'D PACKETS
SUCCESS : BEGINNING TEST EXECUTION
....sending packets
...building read request for device : 0
...sending packet : base addr=0x000000038b8c9000
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building write request for device : 0
...sending packet : base addr=0x0000001eb9856800
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building write request for device : 0
...sending packet : base addr=0x00000005e58f0400
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building write request for device : 0
...sending packet : base addr=0x000000021052a200
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building write request for device : 0
...sending packet : base addr=0x0000001da11b2600
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building read request for device : 0
...sending packet : base addr=0x0000001bfd8bce00
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building read request for device : 0
...sending packet : base addr=0x00000002e602a000
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building write request for device : 0
...sending packet : base addr=0x0000000ab6ba5600
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building read request for device : 0
...sending packet : base addr=0x0000000283ab3e00
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...building write request for device : 0
...sending packet : base addr=0x000000125fe7a000
SUCCESS : PACKET WAS SUCCESSFULLY SENT
...reading responses
STALLED : STALLED IN RECEIVING
SIGNALING HMCSIM TO CLOCK
ALL_SENT = 10
ALL_RECV = 0
...reading responses
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
STALLED : STALLED IN RECEIVING
SIGNALING HMCSIM TO CLOCK
ALL_SENT = 10
ALL_RECV = 9
...reading responses
SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE
STALLED : STALLED IN RECEIVING
SIGNALING HMCSIM TO CLOCK
ALL_SENT = 10
ALL_RECV = 10
SUCCESS : FREE'D HMCSIM
```

You'll notice that the directory now contains an HMC-Sim trace file as well.  The file (physrand.out) should resemble the following:
```
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:0:1:2:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:0:2:2:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:1:0:0:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:1:1:2:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:1:2:0:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:2:0:0:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:2:1:0:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:3:0:2:0
HMCSIM_TRACE : 0 : XBAR_LATENCY : 0:3:1:2:0
HMCSIM_TRACE : 0 : BANK_CONFLICT : 0:0:0:0:0x00000002e602a000
HMCSIM_TRACE : 0 : RD64 : 0:0:0:4:0x000000038b8c9000:1
HMCSIM_TRACE : 0 : WR64 : 0:0:0:2:0x00000002b9856800:5
HMCSIM_TRACE : 0 : WR64 : 0:0:0:0:0x000000025fe7a000:5
HMCSIM_TRACE : 0 : WR64 : 0:0:0:1:0x00000001e58f0400:5
HMCSIM_TRACE : 0 : WR64 : 0:2:0:1:0x00000001a11b2600:5
HMCSIM_TRACE : 0 : RD64 : 0:2:0:7:0x0000000283ab3e00:1
HMCSIM_TRACE : 0 : RD64 : 0:2:0:3:0x00000003fd8bce00:1
HMCSIM_TRACE : 0 : WR64 : 0:2:0:0:0x000000021052a200:5
HMCSIM_TRACE : 0 : WR64 : 0:2:0:5:0x00000002b6ba5600:5
HMCSIM_TRACE : 1 : RD64 : 0:0:0:0:0x00000002e602a000:1
```

**!WARNING!**  Executing the other test scripts may generate very large trace files (GB's) and require significant CPU time.  This is also true for the other test drivers as well (Gups, Stream, etc).

# Interpreting Trace Files #

The HMC-Sim built-in tracing mechanisms can generate rather large and rather verbose trace files.  This is very deliberate in order to expose all the necessary internal data held within the framework.  As such, we provide a tool that may translate the trace files to GNUPlot data scripts.  This tool (hmctognuplot) is located in the following directory:
```
~/hmc_sim/tools/
```

It can be built from the top-level hmc\_sim directory using the following command:
```
$> make tools
```

It can be executed using the following:
```
$> ./tools/hmctognuplot -F /path/to/tracefile.out
```

The tool will produce gnuplot output files for each of [bank\_conflict, rd64, wr64, xbar\_latency, xbar\_rqst\_stall}.out.

Alternatively, you can interpret the HMC-Sim trace files manually.  The trace files are formatted using the following specification:

> | HMCSIM\_TRACE |CLOCK\_TICK| XBAR\_LATENCY | device | link | quad | vault | a request was entered on a link that was not co-located with the destination quad|
|:--------------|:----------|:--------------|:-------|:-----|:-----|:------|:---------------------------------------------------------------------------------|
> | HMCSIM\_TRACE | CLOCK\_TICK | BANK\_CONFLICT | device | quad | vault | bank | address | a bank conflict occurred on the target address |
> | HMCSIM\_TRACE | CLOCK\_TICK | RDsize | device | quad | vault | bank | address | packet\_size\_in\_flits | read memory request |
> | HMCSIM\_TRACE | CLOCK\_TICK | WRsize | device | quad | vault | bank | address | packet\_size\_in\_flits | write memory request |
> | HMCSIM\_TRACE | CLOCK\_TICK | XBAR\_RQST\_STALL | device | quad | vault | slot | Crossbar request stall |
> | HMCSIM\_TRACE | CLOCK\_TICK | VAULT\_RQST\_STALL | device | quad | vault | slot | Vault fifo/queue request stall |
> | HMCSIM\_TRACE | CLOCK\_TICK | XBAR\_RSP\_STALL | device | quad | vault | slot | Crossbar response queue/fifo stall |
> | HMCSIM\_TRACE | CLOCK\_TICK | ROUTE\_RQST\_STALL | device | src | dest | link | slot | Route request stall |
> | HMCSIM\_TRACE | CLOCK\_TICK | ROUTE\_RSP\_STALL | device | src | dest | link | slot | Route response stall |
> | HMCSIM\_TRACE | CLOCK\_TICK | UNDEF\_STALL | device | quad | vault | slot | undefined event |

# References #
  * HMC Spec: http://www.hybridmemorycube.org/
  * Texas Tech Sponsoring Lab: http://discl.cs.ttu.edu/doku.php
  * LSPP 14' paper describing HMC-Sim: http://discl.cs.ttu.edu/lib/exe/fetch.php?media=wiki:papers:lspp14_jleidel_camera_ready_ieee.pdf