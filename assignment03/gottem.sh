#!/bin/bash

echo "Testing"

make

echo "hencode test 1"

./hencode testing/NICO-Hencode/00_empty testing/hencode_mine/encode0.mine
diff testing/hencode_mine/encode0.mine testing/NICO-hdecode/00_empty.huff

echo "hencode test 2"
./hencode testing/NICO-Hencode/01_onechar testing/hencode_mine/encode1.mine
diff testing/hencode_mine/encode1.mine testing/NICO-hdecode/01_onechar.huff
echo "hencode test 3"
./hencode testing/NICO-Hencode/02_twochar testing/hencode_mine/encode2.mine
diff testing/hencode_mine/encode2.mine testing/NICO-hdecode/02_twochar.huff
echo "hencode test 4"
./hencode testing/NICO-Hencode/03_short testing/hencode_mine/encode3.mine
diff testing/hencode_mine/encode3.mine testing/NICO-hdecode/03_short.huff
echo "hencode test 5"
./hencode testing/NICO-Hencode/04_medium testing/hencode_mine/encode4.mine
diff testing/hencode_mine/encode4.mine testing/NICO-hdecode/04_medium.huff
echo "hencode test 6"
./hencode testing/NICO-Hencode/05_binary testing/hencode_mine/encode5.mine
diff testing/hencode_mine/encode5.mine testing/NICO-hdecode/05_binary.huff
echo "hencode test 1 stdout"

./hencode testing/NICO-Hencode/00_empty testing/hencode_mine/encode_stdout0.mine
diff testing/hencode_mine/encode_stdout0.mine testing/NICO-hdecode/00_empty.huff

echo "hencode test 2 stdout"
./hencode testing/NICO-Hencode/01_onechar > testing/hencode_mine/encode_stdout1.mine
diff testing/hencode_mine/encode_stdout1.mine testing/NICO-hdecode/01_onechar.huff
echo "hencode test 3 stdout"
./hencode testing/NICO-Hencode/02_twochar > testing/hencode_mine/encode_stdout2.mine
diff testing/hencode_mine/encode_stdout2.mine testing/NICO-hdecode/02_twochar.huff
echo "hencode test 4 stdout"
./hencode testing/NICO-Hencode/03_short > testing/hencode_mine/encode_stdout3.mine
diff testing/hencode_mine/encode_stdout3.mine testing/NICO-hdecode/03_short.huff
echo "hencode test 5 stdout"
./hencode testing/NICO-Hencode/04_medium > testing/hencode_mine/encode_stdout4.mine
diff testing/hencode_mine/encode_stdout4.mine testing/NICO-hdecode/04_medium.huff
echo "hencode test 6 stdout"
./hencode testing/NICO-Hencode/05_binary > testing/hencode_mine/encode_stdout5.mine
diff testing/hencode_mine/encode_stdout5.mine testing/NICO-hdecode/05_binary.huff

echo "hdecode test 1"
cd testing
../hdecode NICO-hdecode/00_empty.huff hdecode-mine/0decode
diff NICO-Hencode/00_empty hdecode-mine/0decode
echo "hdecode test 2"
../hdecode NICO-hdecode/01_onechar.huff hdecode-mine/1decode
diff NICO-Hencode/01_onechar hdecode-mine/1decode
echo "hdecode test 3"
../hdecode NICO-hdecode/02_twochar.huff hdecode-mine/2decode
diff NICO-Hencode/02_twochar hdecode-mine/2decode
echo "hdecode test 4"
../hdecode NICO-hdecode/03_short.huff hdecode-mine/3decode
diff NICO-Hencode/03_short hdecode-mine/3decode
echo "hdecode test 5"
../hdecode NICO-hdecode/04_medium.huff hdecode-mine/4decode
diff NICO-Hencode/04_medium hdecode-mine/4decode
echo "hdecode test 6"
../hdecode NICO-hdecode/05_binary.huff hdecode-mine/5decode
diff NICO-Hencode/05_binary hdecode-mine/5decode

echo "hdecode stdin test 1"
../hdecode - hdecode-mine/0decodestdin < NICO-hdecode/00_empty.huff
diff NICO-Hencode/00_empty hdecode-mine/0decodestdin
echo "hdecode stdin test 2"
../hdecode - hdecode-mine/1decodestdin < NICO-hdecode/01_onechar.huff
diff NICO-Hencode/01_onechar hdecode-mine/1decode
echo "hdecode stdin test 3"
../hdecode - hdecode-mine/2decodestdin < NICO-hdecode/02_twochar.huff
diff NICO-Hencode/02_twochar hdecode-mine/2decodestdin
echo "hdecode stdin test 4"
../hdecode - hdecode-mine/3decodestdin < NICO-hdecode/03_short.huff
diff NICO-Hencode/03_short hdecode-mine/3decodestdin
echo "hdecode stdin test 5"
../hdecode - hdecode-mine/4decodestdin < NICO-hdecode/04_medium.huff
diff NICO-Hencode/04_medium hdecode-mine/4decodestdin
echo "hdecode stdin test 6"
../hdecode - hdecode-mine/5decodestdin < NICO-hdecode/05_binary.huff
diff NICO-Hencode/05_binary hdecode-mine/5decodestdin

cd ..
