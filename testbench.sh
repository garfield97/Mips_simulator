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
		fail_type=-1
		arguments="$output_filename $passed $name $username $counter $fail_type"
		;;
	-10)
		fail_type=-10
		arguments="$output_filename $failed $name $username $counter $fail_type"
		/bin/write_csv $arguments
		;;
	-11)
		fail_type=-11
		arguments="$output_filename $failed $name $username $counter $fail_type"
		;;
	-12)
		fail_type=-12
		arguments="$output_filename $failed $name $username $counter $fail_type"
		;;
	-20)
		fail_type=-20
		arguments="$output_filename $failed $name $username $counter $fail_type"
		;;
	*)
		fail_type=0
		arguments="$output_filename $failed $name $username $counter $fail_type"
		;;
	esac

	echo $arguments

	./bin/write_csv $arguments

	((counter++)) 
done