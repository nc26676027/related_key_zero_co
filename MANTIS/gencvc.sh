#!/bin/bash
for ((i=0;i<1;i++));
do
	mkdir head_state$i
	cd head_state$i
	cp ../excute.sh ./	
	chmod 755 ./excute.sh
	for ((j=0;j<16;j++));
	do	
		cp ../Find$j.java ./
		javac Find$j.java		
		mkdir key_state$j
		cd key_state$j
		cp ../../mantis.cpp ./
		g++ --std c++11 mantis.cpp -o mantis
		for ((k=0;k<16;k++));
		do			
			./mantis $j $i $k
		done
		cd ..
	done
	cd ..
done

