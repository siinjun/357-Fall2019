#!/bin/bash

echo "Initial Test"

make

./a.out inputs/basic basic.mine
mv basic.mine file_tests
