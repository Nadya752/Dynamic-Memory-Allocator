#!/bin/bash

# Trigger all your test cases with this script
# USYD CODE CITATION ACKNOWLEDGEMENT
# I declare that the following lines of code have been copied from the
# tutorial solutions of Week 8 with only minor changes and it is not my own work. 

# Tutorial solutions for week 8 from INFO1110 course
# https://edstem.org/courses/4000

echo "##########################"
echo "### Running e2e tests! ###"
echo "##########################"
echo
count=0 # number of test cases run so far

for folder in `ls -d testcases/*/ | sort -V`; do
    name=$(basename "$folder")
    
    echo Running test $name.
    expected_file=testcases/$name/$name.out
    gcc -fsanitize=address -Wall -Werror -std=gnu11 -g -lm -Wno-deprecated-declarations testcases/$name/$name.c virtual_alloc.c tests.c -o $name
    ./$name | diff - $expected_file && echo "Test $name: passed!"|| echo "Test $name: failed!"
    echo

    count=$((count+1))
done

echo "Finished running $count tests!"