#!/bin/bash

dir="tests"
for ac in $(seq 0 1)
do
    for id in $(seq 2)
    do
	text="$dir"/testo."$id".fa.gz
	test -f "$text" || exit 1
	for t in ${dir}/input/*
	do
	    p=$(cat "$t")
	    i=$(basename "$t")
	    ./sa-matching --text "$text" --pattern="$p" --accelerant="$ac" | grep -oP "\d+" | sort -n > "${dir}/output/$i".sa.${id}
	done
    done
    diff -uaw --strip-trailing-cr --ignore-all-space "${dir}/output/" "${dir}/ok/"
done
