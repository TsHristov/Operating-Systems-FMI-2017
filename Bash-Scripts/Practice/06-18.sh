#!/bin/bash
# Takes user as an argument and terminates all currently running processess of this user:

test $# -ne 1 && exit 1

USER=$1
TOTAL=0

while read process ; do
    TOTAL=$(expr ${TOTAL} + 1)
    kill ${process}
done < <(ps -eo pid= -u ${USER})

echo "Total number of processes killed (terminated): ${TOTAL}"
