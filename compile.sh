#!/bin/bash

rm -rf deploy

NCORES=4
unamestr=`uname`
if [[ "$unamestr" == "Linux" ]]; then
        NCORES=`grep -c ^processor /proc/cpuinfo`
fi

if [[ "$unamestr" == "Darwin" ]]; then
        NCORES=`sysctl -n hw.ncpu`
fi


for program in viem graphchecker generate_model evaluator; do 
scons program=$program variant=optimized -j $NCORES 
if [ "$?" -ne "0" ]; then 
        echo "compile error in $program. exiting."
        exit
fi
done

mkdir deploy

cp ./optimized/viem deploy/
cp ./optimized/graphchecker deploy/
cp ./optimized/generate_model deploy/
cp ./optimized/evaluator deploy/

rm -rf ./optimized
rm -rf config.log
