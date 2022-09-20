#!/bin/bash
#Author: David Hu
#Date: 2022-09

# Exit on error
set -e

BUILD_MODE_DEBUG=1
BUILD_MODE_RELEASE=0
OP_YES=1

echo "Start cmake build"
read -p "Please select build mode [Debug(1)/Release(0)]:" BUILD_MODE
read -p "IS install lib and include files on the workspace, please input [yes(1)/no[0]]:" IS_INSTALL

if [ ! -e "./build" ];then
  mkdir build
  echo "create ./build/"
fi

cd ./build

if [ ${BUILD_MODE} == ${BUILD_MODE_DEBUG} ];then
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../install ..
elif [ ${BUILD_MODE} == ${BUILD_MODE_RELEASE} ];then
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install ..
else
  echo "build mode input error"
  exit 0
fi

make

if [ ${IS_INSTALL} == ${OP_YES} ]
then
  if [ ! -e "../install" ]
  then
    mkdir ../install
    echo "create ../install/"
  fi
  make install
  echo "build and install finished."
else
  echo "build finished."
fi