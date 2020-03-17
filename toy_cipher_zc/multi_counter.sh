#!/bin/bash

mkdir counter1_h
cd counter1_h
cp ../counter*.cpp ./ 
g++ --std c++11 counter1.cpp -o counter1
g++ --std c++11 counter2.cpp -o counter2
g++ --std c++11 counter3.cpp -o counter3
g++ --std c++11 counter4.cpp -o counter4
g++ --std c++11 counter5.cpp -o counter5
g++ --std c++11 counter6.cpp -o counter6
g++ --std c++11 counter7.cpp -o counter7
g++ --std c++11 counter8.cpp -o counter8

for ((i=1;i<9;i++));
do	
	nohup ./counter$i 1 &
	
done
cd ..



