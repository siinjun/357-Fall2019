#!/bin/bash

echo "Testing"

make

echo "test 1"

./hencode testing/NICO-Hencode/00_empty testing/hencode_mine/encode0.mine
diff testing/hencode_mine/encode0.mine testing/NICO-hdecode/00_empty.huff

echo "test 2"
./hencode testing/NICO-Hencode/01_onechar testing/hencode_mine/encode1.mine
diff testing/hencode_mine/encode1.mine testing/NICO-hdecode/01_onechar.huff
echo "test 3"
./hencode testing/NICO-Hencode/02_twochar testing/hencode_mine/encode2.mine
diff testing/hencode_mine/encode2.mine testing/NICO-hdecode/02_twochar.huff
echo "test 4"
./hencode testing/NICO-Hencode/03_short testing/hencode_mine/encode3.mine
diff testing/hencode_mine/encode3.mine testing/NICO-hdecode/03_short.huff
echo "test 5"
./hencode testing/NICO-Hencode/04_medium testing/hencode_mine/encode4.mine
diff testing/hencode_mine/encode4.mine testing/NICO-hdecode/04_medium.huff
echo "test 6"
./hencode testing/NICO-Hencode/05_binary testing/hencode_mine/encode5.mine
diff testing/hencode_mine/encode5.mine testing/NICO-hdecode/05_binary.huff
