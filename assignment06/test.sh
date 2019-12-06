#!/bin/bash

echo "Testing"
declare -i counter=1
i=0

#while [ $i -lt 8 ]
#do
 #   echo "Test '$i'_command"
 #    ./nicomush testing/inputs/{$i}_command > testing/outputs/his{$i}.out
 #    ./mush testing/inputs/{$i}_command > testing/outputs/mine{$i}.out

 #    echo "Diff $i"
 #    diff testing/outputs/his{$i}.out testing/outputs/mine{$i}.out
 #    ((i++))
#done


echo "Test 1_command"
./nicomush testing/inputs/1_command > testing/outputs/his1.out
./mush testing/inputs/1_command > testing/outputs/mine1.out

echo "Diff 1"
diff testing/outputs/his1.out testing/outputs/mine1.out

echo "Test 2_command"
./nicomush testing/inputs/2_command > testing/outputs/his2.out
./mush testing/inputs/2_command > testing/outputs/mine2.out

echo "Diff 2"
diff testing/outputs/his2.out testing/outputs/mine2.out

echo "Test 3_command"
./nicomush testing/inputs/3_command > testing/outputs/his3.out
./mush testing/inputs/3_command > testing/outputs/mine3.out

echo "Diff 3"
diff testing/outputs/his3.out testing/outputs/mine3.out

echo "Test 4_command"
./nicomush testing/inputs/4_command > testing/outputs/his4.out
./mush testing/inputs/4_command > testing/outputs/mine4.out

echo "Diff 4"
diff testing/outputs/his4.out testing/outputs/mine4.out

echo "Test 5_command"
./nicomush testing/inputs/5_command > testing/outputs/his5.out
./mush testing/inputs/5_command > testing/outputs/mine5.out

echo "Diff 5"
diff testing/outputs/his5.out testing/outputs/mine5.out

echo "Test 6_command"
./nicomush testing/inputs/6_command > testing/outputs/his6.out
./mush testing/inputs/6_command > testing/outputs/mine6.out

echo "Diff 6"
diff testing/outputs/his6.out testing/outputs/mine6.out

echo "Test 7_command"
./nicomush testing/inputs/7_command > testing/outputs/his7.out
./mush testing/inputs/7_command > testing/outputs/mine7.out

echo "Diff 7"
diff testing/outputs/his7.out testing/outputs/mine7.out

echo "Test 8_command"
./nicomush testing/inputs/8_command > testing/outputs/his8.out
./mush testing/inputs/8_command > testing/outputs/mine8.out

echo "Diff 8"
diff testing/outputs/his8.out testing/outputs/mine8.out
