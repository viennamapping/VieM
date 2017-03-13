#!/bin/bash

NCORES=4
unamestr=`uname`
if [[ "$unamestr" == "Linux" ]]; then
        NCORES=`grep -c ^processor /proc/cpuinfo`
fi

if [[ "$unamestr" == "Darwin" ]]; then
        NCORES=`sysctl -n hw.ncpu`
fi


scons program=viem variant=optimized -j $NCORES -c 
scons program=graphchecker variant=optimized -j $NCORES -c 

rm -rf deploy
rm -rf optimized
rm -rf config.log
