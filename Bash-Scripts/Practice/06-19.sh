#!/bin/bash
# Finds the count of executable files in $PATH:

TOTAL=0

while read path; do
    if [ -d ${path} ]; then
	TOTAL=$(expr ${TOTAL} + $(find ${path} -type f -executable | wc -l))
    fi
done < <(echo ${PATH} | tr -s ':' '\n')

echo "TOTAL: ${TOTAL}"
