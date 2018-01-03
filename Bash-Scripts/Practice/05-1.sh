#!/bin/bash

# Checks how many of the files in the current directory are:
# -regular
# -block special
# -character special
# -directory

test $# -ne 1 && exit 1

DIR=$1

if [ ! -d ${DIR} ]; then
    echo "${DIR} is not a directory!"
    exit 1
fi

echo "Files in ${DIR}: "

REGULAR_FILES=$(find ${DIR} -type f | wc -l)
echo "${REGULAR_FILES} regular files"

BLOCK_SPECIAL=$(find ${DIR} -type b | wc -l)
echo "${BLOCK_SPECIAL} block (buffered) special"

CHARACTER_SPECIAL=$(find ${DIR} -type c | wc -l)
echo "${CHARACTER_SPECIAL} character (unbuffered) special"

DIRECTORIES=$(find ${DIR} -type d | wc -l)
echo "${DIRECTORIES} directories"
