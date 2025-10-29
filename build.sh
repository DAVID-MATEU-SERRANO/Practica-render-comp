#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

rm -rf out

cmake --preset default 
cmake --build --preset gcc-release --config Release --target all --parallel

# Se pueden añadir más modos de compilación