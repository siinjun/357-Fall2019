#!/bin/bash

echo "Initial Test"

make
./a.out tests/04_medium > medium.mine
diff medium.mine outputs/04_medium.expected
