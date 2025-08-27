#!/usr/bin/env bash
echo "==> Buiding project..."
cd build && make
echo "==> Running project..."
./huta_dsp
echo "==> Adding to testing..."
cd ..
./vis.sh
echo "==> Done"
