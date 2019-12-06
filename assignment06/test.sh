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
while [ $i -lt 8 ]
do
    echo Filename [$i]_command
    ((i++))
done
