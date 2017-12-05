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

	export output_filename passed name username counter type

	case $result in
	-1)
		type=-1
		arguments="$output_filename $passed $name $username $counter $type"
		;;
	-10)
		type=-10
		arguments="$output_filename $failed $name $username $counter $type"
		;;
	-11)
		type=-11
		arguments="$output_filename $failed $name $username $counter $type"
		;;
	-12)
		type=-12
		arguments="$output_filename $failed $name $username $counter $type"
		;;
	-20)
		type=-20
		arguments="$output_filename $failed $name $username $counter $type"
		;;
	*)
		type=0
		arguments="$output_filename $failed $name $username $counter $type"
		;;
	esac

	./bin/write_csv $arguments

	((counter++)) 
done