#!/bin/bash
for ((i=0;i<5;i++));
do
	mkdir key_state$i
	cd key_state$i
	cp ../excute.sh ./	
	chmod 755 ./excute.sh
	for ((j=0;j<16;j++));
	do	
		cp ../Find$j.java ./
		javac Find$j.java		
		mkdir head_state$j
		cd head_state$j
		cp ../../skinny2.cpp ./
		g++ --std c++11 skinny2.cpp -o skinny
		for ((k=0;k<16;k++));
		do			
			./skinny $i $j $k
		done
		cd ..
	done
	cd ..
done

