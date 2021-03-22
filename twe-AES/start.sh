#!/bin/bash
for ((i=0;i<5;i++));
do
	cd key_state$i
	./excute.sh
	cd ..
done

