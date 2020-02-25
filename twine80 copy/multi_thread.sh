#!/bin/bash

mkdir test2
cd test2
cp ../test_twine80.cpp ./ 
g++ --std c++11 test_twine80.cpp -o test_twine80
for ((i=0;i<8;i++));
do	
	nohup ./test_twine80 $i > r_$i.txt &	
done
cd ..



