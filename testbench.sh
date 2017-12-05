#!/bin/bash

tmp=tmp.txt

read -p 'Author name: ' uservar

output='results.csv'

command="$1 $2"

result=$($command)

echo $result