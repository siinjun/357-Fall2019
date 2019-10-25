#!/bin/bash

echo "Initial Test"

make

./a.out inputs/basic basic.mine
mv basic.mine encoded
xxd encoded/basic.mine

make hdecode
./b.out encoded/basic.mine
