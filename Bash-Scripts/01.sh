#!/bin/bash
test $# -lt 1 && exit 1

if [ $# -eq 2 ]; then
    DIR=$1
    if [ ! -d "${DIR}" ]; then
	echo "$1 is not a directory!"
	exit 2
    fi
    NUM=$2
    while read FILE_NAME HARD_LINKS; do
	if [ ${HARD_LINKS} -ge ${NUM} ]; then
	    echo "${FILE_NAME} - ${HARD_LINKS}"
	fi
    done < <(find ${DIR} -type f -printf "%p %n\n")
else
    DIR=$1
    if [ ! -d "${DIR}" ]; then
	echo "$1 is not a directory!"
	exit 2
    fi

    while read SYM_LINK; do
	echo "${SYM_LINK} -> broken symoblic link"
    done < <(find -L $1 -type l)
fi

    
