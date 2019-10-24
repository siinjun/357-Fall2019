#!/bin/bash

echo "Initial Test"

make

./a.out inputs/basic basic.mine
mv basic.mine file_tests
xxd file_tests/basic.mine

make hdecode
./b.out file_tests/basic.mine
