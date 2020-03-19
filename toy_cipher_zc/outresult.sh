#!/bin/bash

mkdir cout_re
cd cout_re
cp ../add_counter*.cpp ./ 
for ((i=3;i<6;i++));
do	
	g++ --std c++11 add_counter$i.cpp -o add_counter$i
	nohup ./add_counter$i > re_counter$i.txt &	
done
cd ..



