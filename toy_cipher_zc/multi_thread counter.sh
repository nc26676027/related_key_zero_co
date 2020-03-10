#!/bin/bash

mkdir counter0_h
cd counter0_h
cp ../counter*.cpp ./ 
g++ --std c++11 counter1.cpp -o counter1
g++ --std c++11 counter2.cpp -o counter2
g++ --std c++11 counter3.cpp -o counter3
g++ --std c++11 counter4.cpp -o counter4
g++ --std c++11 counter5.cpp -o counter5
g++ --std c++11 counter6.cpp -o counter6
g++ --std c++11 counter7.cpp -o counter7
g++ --std c++11 counter8.cpp -o counter8

for ((i=0;i<1;i++));
do	
	nohup ./counter1 $i &	
	nohup ./counter2 $i &	
	nohup ./counter3 $i &	
	nohup ./counter4 $i &		
	nohup ./counter5 $i &	
	nohup ./counter6 $i &	
	nohup ./counter7 $i &	
	nohup ./counter8 $i &	

done
cd ..



