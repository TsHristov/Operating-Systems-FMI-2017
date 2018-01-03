#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage: $0 <directory> <size>"
    exit 1
fi

DIR=$1
SIZE=$2

if [ ! -d "${DIR}" ]; then
    echo "${DIR} is not a directory!"
    exit 1
fi

for FILE in $(find ${DIR} -type f); do
    if [ -f ${FILE} ]; then
	FILE_SIZE=$(stat ${FILE} --printf "%s")
	if [ ${FILE_SIZE} -gt ${SIZE} ]; then
	    echo "${FILE} -> ${FILE_SIZE}"
	fi
    fi
done

