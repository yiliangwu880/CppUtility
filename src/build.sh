#!/bin/sh


SVN_VERSION=0

mkdir -p Debug
echo $SVN_VERSION
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DVERSION_REVISION=$SVN_VERSION  ..
make -j9
