#!/bin/bash

tmp=tmp.txt

file_names='j.bin jal.bin' #Names of all test binaries

passed=pass
failed=fail
test_directory=test_binaries/ #Path to test binaries

read -p 'Author name: ' username

counter=1
type=-1
num_failures=0

for name in $file_names
do
	export output_filename passed name username counter type command
	file_loc="$test_directory$name"
	command="$1 $file_loc"
	$command #Run test binary in simulator
	result=$? #Retrieve return value of simulator

	case $result in
	255) #Passed
		type=-1
		arguments="$passed $name $username $counter $type"
		;;
	246) #Arithmetic overflow
		type=-10
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	245) #Memory exception
		type=-11
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	244) #Invalid instruction
		type=-12
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	236) #Internal error
		type=-20
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	*) #Unkown Error
		type=0
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	esac

	./bin/write_csv $arguments >> results.csv #Print result to csv file

	((counter++)) #Counter increments, acts as test reference
done

echo
echo
echo $num_failures failed instructions
echo Failures occured at $failure_list