#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

cd /home/alumnos/a0522256/render-2025-a-m81-12
rm -rf out

cmake --preset default
cmake --build --preset gcc-release --config Release --target all --parallel

perf stat -r 5 ./out/build/default/soa/Release/render-soa ./config_full/config3.txt ./scene/scene3.txt ./out_full/3out.txt

perf stat -r 5 ./out/build/default/soa/Release/render-soa ./config_full/config4.txt ./scene/scene2.txt ./out_full/4out.txt