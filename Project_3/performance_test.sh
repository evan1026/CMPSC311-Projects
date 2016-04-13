#!/usr/bin/bash

for i in 1 2 4 8
do
  for j in {1..20}
  do
    ./mr-wordc "input/hamlet.txt" "output/hamlet/count.txt" "output/hamlet/$j" $i
    ./mr-wordc "input/pangurban.txt" "output/pangurban/count.txt" "output/pangurban/$j" $i
    ./mr-wordc "input/arabiannights.txt" "output/arabiannights/count.txt" "output/arabiannights/$j" $i
  done
done
