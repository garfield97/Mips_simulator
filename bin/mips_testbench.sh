#!/bin/bash

tmp=tmp.txt

file_names='j.bin jal.bin' #Names of all non arithmetic filenames
arith_filenames=' ' #Names of arithmetic tests
passed=pass #Pass variable
failed=fail #Fail variable
test_directory=test_binaries/ #Path to test binaries

read -p 'Author name: ' username

counter=1
type=-1
num_failures=0

echo TestId , Instruction , Status , Author, Message > results.csv

for name in $file_names
do
	export output_filename passed name username counter type command
	file_loc="$test_directory$name"
	command="$1 $file_loc"
	$command #Run test binary in simulator
	result=$? #Retrieve return value of simulator

	case $result in
	10) #Passed
		type=-1
		arguments="$passed $name $username $counter $type"
		;;
	1) #Arithmetic overflow
		type=-10
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	2) #Memory exception
		type=-11
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	3) #Invalid instruction
		type=-12
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	4) #Internal error
		type=-20
		arguments="$failed $name $username $counter $type"
		failure_list=$($failure_list $name)
		((num_failures++))
		;;
	*) #Check Result
		val=$(cat test_binaries/solutions/$sol_name) #Load expected solution and compare

		if [ $val -eq $result ]
		then 
			type=-1
		arguments="$passed $name $username $counter $type"
		else
			arguments="$failed $name $username $counter $type"
			failure_list=$($failure_list $name)
			((num_failures++))
		fi
		;;
	esac

	./bin/write_csv $arguments >> results.csv #Print result to csv file

	((counter++)) #Counter increments, acts as test reference
done

if [ $num_failures -gt 0 ]
then
	echo $num_failures failed instructions >> test/output/output/failures.txt
	echo Failures occured at $failure_list >> test/output/output/failures.txt
fi