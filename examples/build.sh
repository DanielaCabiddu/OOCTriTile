#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

mkdir -p ${SCRIPT_DIR}/build
cd ${SCRIPT_DIR}/build
cmake -DCMAKE_CXX_STANDARD=11 ..
make -j8
