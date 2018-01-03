#!/bin/bash
# Find the file with maximum count of links in the directory given as an argument:

test $# -ne 1 && exit 1
DIR=$1

if [ ! -d ${DIR} ]; then
    echo "${DIR} is not a directory!"
    exit 1
fi

MAX=0
FILE=""

while read FILENAME LINKS; do
    if [ ${LINKS} -gt ${MAX} ]; then
	MAX=${LINKS}
	FILE=${FILENAME}
    fi
done < <(find ${DIR} -type f -printf "%p %n\n")

echo "File with max (${MAX}) number of links is ${FILE}"
