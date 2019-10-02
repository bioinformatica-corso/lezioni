#!/bin/bash

dir="tests"
datadir="${dir}/.data/"
test -d "$datadir" || mkdir -p "$datadir"
test -f "$datadir"/testo.1.fa.gz || wget -O "$datadir"/testo.1.fa.gz http://hgdownload.cse.ucsc.edu/goldenPath/sacCer3/chromosomes/chrI.fa.gz
test -f "$datadir"/testo.2.fa.gz || wget -O "$datadir"/testo.2.fa.gz http://hgdownload.cse.ucsc.edu/goldenPath/hg38/chromosomes/chr1.fa.gz

for id in $(seq 2)
do
    text="${datadir}/testo.${id}.fa.gz"
    test -f "$text" || exit 1
    test -f "$dir"/output || mkdir -p "$dir"/output
    for t in ${dir}/input/*
    do
	p=$(cat "$t")
	i=$(basename "$t")
	./karp-rabin --text="$text" --pattern="$p" --rounds=5 | grep -oP "\d+" | sort -n > "${dir}/output/$i".kr.${id}
    done
done
diff -uaw --strip-trailing-cr --ignore-all-space "${dir}/output/" "${dir}/ok/"
