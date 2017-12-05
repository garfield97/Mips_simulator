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
		arguments="$output_filename $passed $name $username $counter"
		echo $arguments
		./src/write_csv $arguments
		;;
	*)
		arguments="$output_filename $failed $name $username $counter"
		echo $arguments
		./src/write_csv $arguments
		;;
	esac

	((counter++)) 
done