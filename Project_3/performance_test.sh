#!/bin/bash

for i in 1 2 4 8
do
  for j in {1..20}
  do
    echo Doing $i x $j
    ./mr-wordc "input/hamlet.txt" "output/hamlet/count.txt" "output/hamlet/$j" $i
    echo "    " Done hamlet
    ./mr-wordc "input/pangurban.txt" "output/pangurban/count.txt" "output/pangurban/$j" $i
    echo "    " Done pengur ban
    ./mr-wordc "input/arabiannights.txt" "output/arabiannights/count.txt" "output/arabiannights/$j" $i
    echo "    " Done arabian nights
  done
done
