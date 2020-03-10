#!/bin/bash

mkdir counter_h
cd counter_h
cp ../counter*.cpp ./ 
g++ --std c++11 counterone.cpp -o counterone
g++ --std c++11 countertwo.cpp -o countertwo

for ((i=0;i<1;i++));
do	
	nohup ./counterone $i &	
	nohup ./countertwo $i &	

done
cd ..



