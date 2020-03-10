#!/bin/bash

mkdir counter0_h
cd counter0_h
cp ../counter*.cpp ./ 
g++ --std c++11 counterone.cpp -o counterone
g++ --std c++11 countertwo.cpp -o countertwo
g++ --std c++11 counterthree.cpp -o counterthree
g++ --std c++11 counterfour.cpp -o counterfour
g++ --std c++11 counterfive.cpp -o counterfive
g++ --std c++11 countersix.cpp -o countersix
g++ --std c++11 counterseven.cpp -o counterseven
g++ --std c++11 countereight.cpp -o countereight

for ((i=0;i<1;i++));
do	
	nohup ./counterone $i &	
	nohup ./countertwo $i &	
	nohup ./counterthree $i &	
	nohup ./counterfour $i &		
	nohup ./counterfive $i &	
	nohup ./countersix $i &	
	nohup ./counterseven $i &	
	nohup ./countereight $i &	

done
cd ..



