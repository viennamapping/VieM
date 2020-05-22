#!/bin/bash

NCORES=4
unamestr=`uname`
if [[ "$unamestr" == "Linux" ]]; then
        NCORES=`grep -c ^processor /proc/cpuinfo`
fi

if [[ "$unamestr" == "Darwin" ]]; then
        NCORES=`sysctl -n hw.ncpu`
fi

rm -rf deploy
rm -rf build
mkdir build
cd build
cmake ../
make -j $NCORES
cd ..

mkdir deploy
cp ./build/viem deploy/
cp ./build/graphchecker deploy/
cp ./build/generate_model deploy/
cp ./build/evaluator deploy/

rm -rf ./build
rm -rf config.log


