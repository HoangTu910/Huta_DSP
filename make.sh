#!/usr/bin/env bash
echo "==> Building project..."
cd build || exit
make

echo "==> Running project..."
./huta_dsp

echo "==> Adding to testing..."
cd ..
./vis.sh

echo "==> Done"
