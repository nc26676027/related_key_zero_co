for ((i=0;i<16;i++));
do
  nohup java Find$i 0 > result_$i.txt &
done
