#!/bin/bash

mkdir test_h
cd test_h
cp ../Toy_gen ./ 
g++ --std c++11 Toy_gen.cpp -o Toy_gen
for ((i=0;i<16;i++));
do	
	nohup ./Toy_gen $i &	
done
cd ..



