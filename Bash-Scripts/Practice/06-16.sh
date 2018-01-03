#!/bin/bash
test $# -eq 0 && exit 1
ARGS=$*

for arg in ${ARGS}; do
    if [ -f ${arg} -a -r ${arg} ]; then
	echo "${arg} exists and read permission is granted"
    fi
    
    if [ -d ${arg} ]; then
	FILES_COUNT=$(find ${arg} -type f | wc -l)
	# Print all files that have size greater than FILES_COUNT:
	while read FILE SIZE; do
	    echo "FILE: ${FILE}, SIZE: ${SIZE}"
	done < <(find ${arg} -type f -size -${FILES_COUNT}c -printf "%p %s\n")
    fi
done
