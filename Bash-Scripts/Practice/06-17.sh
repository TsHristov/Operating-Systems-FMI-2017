#!/bin/bash
test $# -ne 2 && exit 1

DIR=$1
if [ ! -d "${DIR}" ]; then
    echo "${DIR} is not a directory"
    exit 1
fi

SIZE=$2
SIZE_SUM=0
FILE_SIZES=$(find "${DIR}" -type f -size +${SIZE}c -printf "%s ")

for SIZE in ${FILE_SIZES}; do
    SIZE_SUM=$(expr ${SIZE_SUM} + ${SIZE})
done

echo "Total size: ${TOTAL}"

