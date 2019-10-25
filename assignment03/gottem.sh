#!/bin/bash

echo "Testing"

make

echo "test 1"

./hencode testing/NICO-Hencode/00_empty testing/hencode_mine/encode0.mine
diff testing/hencode_mine/encode0.mine testing/NICO-hdecode/00_empty.huff

echo "test 2"
./hencode testing/NICO-Hencode/01_onechar testing/hencode_mine/encode1.mine
diff testing/hencode_mine/encode1.mine testing/NICO-hdecode/01_onechar.huff
