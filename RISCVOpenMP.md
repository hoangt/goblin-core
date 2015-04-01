# Introduction #
This pages provides an introduction to building and using the OpenMP runtime library for LLVM as compiled for the RISCV target architecture.  Note, the build system does not currently support building a cross-compiled library.  It must be compiled using the native compiler environment.

# Acknowledgments #
Given that the RISCV OpenMP port is not yet upstreamed, we want to fully acknowledge the original authors and supporters of the OpenMP runtime and RISCV.
  * Intel's Open Source OpenMP Runtime: https://www.openmprtl.org/
  * LLVM OpenMP: http://openmp.llvm.org/
  * Clang patches for OpenMP: http://clang-omp.github.io/
  * RISCV Project: http://riscv.org/
  * RISCV LLVM Trunk: https://github.com/ucb-bar/riscv-llvm/tree/riscv-trunk

# Getting code #

At some point in the future we will provide explicit patch sets for the current OpenMP runtime trunk.  However, at this time, we maintain a separate tree.  The primary tree is located in our Googlecode SVN repository.  We also maintain a mirrored tree in Github.  You can retrieve either tree via the following:

SVN:
```
$> svn co https://goblin-core.googlecode.com/svn/trunk/riscv/riscv-omp riscv-omp
```

GIT:
```
$> git clone https://github.com/jleidel/riscv-llvm-omp.git riscv-omp
```

# Building the Code #
The library can be built using either GNU make (with PERL scripts under the covers) or CMake.  We have modified both build systems but currently only actively test the GNU Make path.

In order to build the libraries, you must have the following installed:
  * PERL : the build system makes extensive use of PERL scripts
  * GNU Make
  * GCC or CLANG native builds for RISCV

You can perform the build by executing the following (GCC):
```
$> cd riscv-omp/runtime
$> make compiler=gcc arch=riscv
```

You can perform the build by executing the following (CLANG):
```
$> cd riscv-omp/runtime
$> make compiler=clang arch=riscv
```

TODO: Add CMake build directions

# Testing the Library #
TODO: See UHouston OpenMP test suite