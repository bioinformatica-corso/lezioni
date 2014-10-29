#!/bin/bash

dir="tests"
test -f "$dir/chrI.fa.gz" || wget -O "$dir/chrI.fa.gz" http://hgdownload.cse.ucsc.edu/goldenPath/sacCer3/chromosomes/chrI.fa.gz
test -f "$dir/chr1.fa.gz" || wget -O "$dir/chr1.fa.gz" http://hgdownload.cse.ucsc.edu/goldenPath/hg38/chromosomes/chr1.fa.gz
for t in ${dir}/input/*
do
    p=$(cat "$t")
    i=$(basename "$t")
    ./karp-rabin --text "$dir"/chrI.fa.gz --pattern="$p" --rounds=5 | grep -oP "\d+" > "${dir}/output/$i".1
    ./karp-rabin --text "$dir"/chr1.fa.gz --pattern="$p" --rounds=5 | grep -oP "\d+" > "${dir}/output/$i".2
done
diff -uaw --strip-trailing-cr --ignore-all-space "${dir}/output/" "${dir}/ok/"
