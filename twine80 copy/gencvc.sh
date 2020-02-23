#!/bin/bash
for ((i=0;i<2;i++));
do
	mkdir key_state$i
	cd key_state$i
	cp ../excute.sh ./	
	chmod 755 ./excute.sh
	for ((j=0;j<4;j++));
	do	
		cp ../Find$j.java ./
		javac Find$j.java		
		mkdir head_state$j
		cd head_state$j
		cp ../../twine80.cpp ./
		g++ --std c++11 twine80.cpp -o twine80
		for ((k=0;k<4;k++));
		do			
			./twine80 $i $j $k
		done
		cd ..
	done
	cd ..
done

