#!/bin/bash

set -Eeuo pipefail
export LD_LIBRARY_PATH="/opt/gcc-14/lib64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

cd /home/alumnos/a0522256/render-2025-a-m81-12

perf stat -r 5 ./out/build/default/aos/Release/render-aos ./config_full/config4.txt ./scene/scene4.txt ./out_full/4out.txt