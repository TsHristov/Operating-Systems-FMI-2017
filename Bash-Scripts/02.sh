#!/bin/bash
check_dir () {
    if [ ! -d "$1" ]; then
	echo "$1 is not a directory!"
	exit 1
    fi
}

if [ $# -ne 3 ]; then
    echo "usage: $0 <source> <destination> <string>"
    exit 1
fi

if [ $(id -u) -ne 0 ]; then
    echo "Script must be run as root!"
    exit 1
fi


check_dir "$1"
check_dir "$2"

if [ $(find $2 -type f | wc -l) -ne 0 ]; then
    echo "$2 is non-empty!"
    exit 1
fi

SRC=$(dirname "$1")"/"$(basename "$1")
DST=$(dirname "$2")"/"$(basename "$2")
STRING="$3"

while read FILE; do
    echo "${FILE}"
done < <(find ${SRC} -name "*${STRING}*")


