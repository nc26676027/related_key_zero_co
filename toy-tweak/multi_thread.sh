#!/bin/bash

mkdir test0
cd test0
cp ../test_twine80.cpp ./ 
g++ --std c++11 test_twine80.cpp -o test_twine80
for ((i=0;i<16;i++));
do	
	nohup ./test_twine80 $i > r_$i.txt &	
done
cd ..



