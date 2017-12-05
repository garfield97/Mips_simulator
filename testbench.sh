#!/bin/bash

tmp=tmp.txt

file_names='src/tst.bin'

output_filename=results.csv
passed=pass
failed=fail

read -p 'Author name: ' username

output='results.csv'
counter=1

for name in $file_names
do
	command="$1 $name"
	result=$($command)

	export output_filename passed name username counter

	case $result in
	-1)
		./src/write_csv $output_filename $passed $name $username $counter
		;;
	*)
		./src/write_csv $output_filename $failed $name $username $counter
		;;
	esac

	((counter++)) 
done