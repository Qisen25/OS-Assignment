#!/bin/bash

size=$1
count=1

if [ "$size" -gt 0 ]; then
    echo "Creating task_file"
    while [ $count -le $size ]; do
        echo $count $(((RANDOM % 50) + 1))
        count=$(($count + 1))
    done > task_file
else
    echo "Command arg must be greater than 0"
fi
