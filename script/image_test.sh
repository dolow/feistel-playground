#!/bin/sh -ex

SCRIPT_DIR=`dirname $0`
SCRIPT_DIR=`cd ${SCRIPT_DIR} && pwd -P`
ROOT_DIR=${SCRIPT_DIR}/..
LIBRARY_BUILD_DIR=${ROOT_DIR}/build
EXAMPLE_BUILD_DIR=${ROOT_DIR}/example_build

if [ ! -d ${LIBRARY_BUILD_DIR} ]; then
  mkdir ${LIBRARY_BUILD_DIR}
fi

pushd ${LIBRARY_BUILD_DIR}
  cmake .. && make
popd # ${LIBRARY_BUILD_DIR}

if [ ! -d ${EXAMPLE_BUILD_DIR} ]; then
  mkdir ${EXAMPLE_BUILD_DIR}
fi

pushd ${EXAMPLE_BUILD_DIR}

clang++ ${ROOT_DIR}/example/image_test.cpp -I${ROOT_DIR}/include/ -L${ROOT_DIR}/build/ -lfeistelxor

popd # ${EXAMPLE_BUILD_DIR}

chmod 755 ${EXAMPLE_BUILD_DIR}/a.out 


