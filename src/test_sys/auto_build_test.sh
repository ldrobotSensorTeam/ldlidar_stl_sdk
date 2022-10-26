#!/bin/bash
#Author: David Hu
#Date: 2022-09

# Exit on error
set -e

echo "Start cmake build"

if [ ! -e "./build" ];then
  mkdir build
  echo "create ./build/"
fi

cd ./build

cmake -DCMAKE_BUILD_TYPE=Debug ..

make -j2

echo "build finished."