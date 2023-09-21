#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cd ${SCRIPT_DIR}/build
mkdir -p bunny_output
./01_Trimesh_Tiling/01_Trimesh_Tiling -v 5000 -f ../../data/bun_zipper.off -o ./bunny_output/
