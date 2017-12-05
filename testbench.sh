#!/bin/bash

sim_location=$1
bin_location=$2
tmp=tmp.txt

read -p 'Author name: ' uservar

output='results.csv'

command="$1 $2"

result=$($command)

echo $result

