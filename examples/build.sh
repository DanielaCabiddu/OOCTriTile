#!/bin/bash

mkdir -p build
cd build
cmake -DCMAKE_CXX_STANDARD=11 ..
make -j8
