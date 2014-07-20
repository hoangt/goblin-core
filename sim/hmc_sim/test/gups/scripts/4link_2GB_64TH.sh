#!/bin/bash

ulimit -s unlimited 

GUPS=../gups
RUNDIR=`pwd`

BANKS=8
CAPACITY=2
LINKS=4
BSIZE=64
QDEPTH=64
XDEPTH=128
VAULTS=16
#NRQSTS=134217728
#NRQSTS=33554432
NRQSTS=2147483648
#NRQSTS=4096
DRAMS=20
#THREADS=16
#THREADS=32
THREADS=64
SIMD=8


echo "Executing $GUPS -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -s $SIMD -v $VAULTS -x $XDEPTH -N $NRQSTS -T $THREADS"


$GUPS -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -s $SIMD -v $VAULTS -x $XDEPTH -N $NRQSTS -T $THREADS
