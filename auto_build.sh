#!/bin/bash
#Author: David Hu
#Date: 2022-05

# Exit on error
set -e
set -x

if [ ! -e "./build" ];then
  mkdir build
  echo "create ./build/"
fi

echo "start cmake build"
cd ./build
cmake ..
make