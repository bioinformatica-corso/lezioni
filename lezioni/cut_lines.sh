#!/bin/bash


#Inspired by http://stackoverflow.com/questions/17988756/using-sed-awk-to-select-lines-between-two-patterns-occurring-which-is-occurring


input_file="$1"
output_dir="$2"
test -d "$2" || mkdir -p "$2"
for id in `grep '/*** start:'  $input_file | cut -d ' ' -f 3`
do
    echo "$output_dir/$id"
    awk "/start: $id/{a=1;next}/***end: /{a=0}a" $input_file > "$output_dir/$id"
done
