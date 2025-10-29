#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

cd /home/alumnos/a0522256/render-2025-a-m81-12

perf stat -r 5 ./out/build/default/soa/Release/render-soa ./config_full/config3.txt ./scene/scene3.txt ./out_full/3out.txt