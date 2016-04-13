#!/bin/bash

#Just declaring each different array
declare -a hamlet1_100 
declare -a hamlet2_100
declare -a hamlet4_100
declare -a hamlet8_100
declare -a hamlet1_1024
declare -a hamlet2_1024
declare -a hamlet4_1024
declare -a hamlet8_1024
declare -a hamlet1_10240
declare -a hamlet2_10240
declare -a hamlet4_10240
declare -a hamlet8_10240
declare -a pangurban1_100
declare -a pangurban2_100
declare -a pangurban4_100
declare -a pangurban8_100
declare -a pangurban1_1024
declare -a pangurban2_1024
declare -a pangurban4_1024
declare -a pangurban8_1024
declare -a pangurban1_10240
declare -a pangurban2_10240
declare -a pangurban4_10240
declare -a pangurban8_10240
declare -a arabiannights1_100
declare -a arabiannights2_100
declare -a arabiannights4_100
declare -a arabiannights8_100
declare -a arabiannights1_1024
declare -a arabiannights2_1024
declare -a arabiannights4_1024
declare -a arabiannights8_1024
declare -a arabiannights1_10240
declare -a arabiannights2_10240
declare -a arabiannights4_10240
declare -a arabiannights8_10240

array_index = 0
array = 0

for i in {1..20} 
do 
	FILE=output/hamlet/$i
	while read line; do
		if [$array = 0]; then
			hamlet1_100[$array_index] = $line
		elif [$array = 1]; then
			hamlet2_100[$array_index] = $line		
		elif [$array = 2]; then
			hamlet4_100[$array_index] = $line		
		elif [$array = 3]; then
			hamlet8_100[$array_index] = $line		
		elif [$array = 4]; then
			hamlet1_1024[$array_index] = $line
		elif [$array = 5]; then
			hamlet2_1024[$array_index] = $line		
		elif [$array = 6]; then
			hamlet4_1024[$array_index] = $line		
		elif [$array = 7]; then
			hamlet8_1024[$array_index] = $line		
		elif [$array = 8]; then
			hamlet1_10240[$array_index] = $line
		elif [$array = 9]; then
			hamlet2_10240[$array_index] = $line		
		elif [$array = 10]; then
			hamlet4_10240[$array_index] = $line		
		else
			hamlet8_10240[$array_index] = $line	
		fi

		((array++))
		
	done

	((array_index++))

done

array_index = 0
array = 0

for i in {1..20} 
do 
	FILE=output/pangurban/$i
	while read line; do
		if [$array = 0]; then
			pangurban1_100[$array_index] = $line
		elif [$array = 1]; then
			pangurban2_100[$array_index] = $line		
		elif [$array = 2]; then
			pangurban4_100[$array_index] = $line		
		elif [$array = 3]; then
			pangurban8_100[$array_index] = $line		
		elif [$array = 4]; then
			pangurban1_1024[$array_index] = $line
		elif [$array = 5]; then
			pangurban2_1024[$array_index] = $line		
		elif [$array = 6]; then
			pangurban4_1024[$array_index] = $line		
		elif [$array = 7]; then
			pangurban8_1024[$array_index] = $line		
		elif [$array = 8]; then
			pangurban1_10240[$array_index] = $line
		elif [$array = 9]; then
			pangurban2_10240[$array_index] = $line		
		elif [$array = 10]; then
			pangurban4_10240[$array_index] = $line		
		else
			pangurban8_10240[$array_index] = $line	
		fi

		((array++))
		
	done

	((array_index++))

done

array_index = 0
array = 0

for i in {1..20} 
do 
	FILE=output/arabiannights/$i
	while read line; do
		if [$array = 0]; then
			arabiannights1_100[$array_index] = $line
		elif [$array = 1]; then
			arabiannights2_100[$array_index] = $line		
		elif [$array = 2]; then
			arabiannights4_100[$array_index] = $line		
		elif [$array = 3]; then
			arabiannights8_100[$array_index] = $line		
		elif [$array = 4]; then
			arabiannights1_1024[$array_index] = $line
		elif [$array = 5]; then
			arabiannights2_1024[$array_index] = $line		
		elif [$array = 6]; then
			arabiannights4_1024[$array_index] = $line		
		elif [$array = 7]; then
			arabiannights8_1024[$array_index] = $line		
		elif [$array = 8]; then
			arabiannights1_10240[$array_index] = $line
		elif [$array = 9]; then
			arabiannights2_10240[$array_index] = $line		
		elif [$array = 10]; then
			arabiannights4_10240[$array_index] = $line		
		else
			arabiannights8_10240[$array_index] = $line	
		fi

		((array++))
		
	done

	((array_index++))

done

mean1 = $hamlet1_100[@] / 20
mean2 = $hamlet2_100[@] / 20
mean3 = $hamlet4_100[@] / 20
mean4 = $hamlet8_100[@] / 20
mean5 = $hamlet1_1204[@] / 20
mean6 = $hamlet2_1204[@] / 20
mean7 = $hamlet4_1204[@] / 20
mean8 = $hamlet8_1204[@] / 20
mean9 = $hamlet1_12040[@] / 20
mean10 = $hamlet2_12040[@] / 20
mean11 = $hamlet4_12040[@] / 20
mean12 = $hamlet8_12040[@] / 20
mean13 = $pangurban1_100[@] / 20
mean14 = $pangurban2_100[@] / 20
mean15 = $pangurban4_100[@] / 20
mean16 = $pangurban8_100[@] / 20
mean17 = $pangurban1_1204[@] / 20
mean18 = $pangurban2_1204[@] / 20
mean19 = $pangurban4_1204[@] / 20
mean20 = $pangurban8_1204[@] / 20
mean21 = $pangurban1_12040[@] / 20
mean22 = $pangurban2_12040[@] / 20
mean23 = $pangurban4_12040[@] / 20
mean24 = $pangurban8_12040[@] / 20
mean25 = $arabiannights1_100[@] / 20
mean26 = $arabiannights2_100[@] / 20
mean27 = $arabiannights4_100[@] / 20
mean28 = $arabiannights8_100[@] / 20
mean29 = $arabiannights1_1204[@] / 20
mean30 = $arabiannights2_1204[@] / 20
mean31 = $arabiannights4_1204[@] / 20
mean32 = $arabiannights8_1204[@] / 20
mean33 = $arabiannights1_12040[@] / 20
mean34 = $arabiannights2_12040[@] / 20
mean35 = $arabiannights4_12040[@] / 20
mean36 = $arabiannights8_12040[@] / 20

for i in {1..36}
do
	echo $mean$i \n
done
