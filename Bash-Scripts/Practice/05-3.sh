#!/bin/bash
# Finds out which user has the most processes in running state:

MAX=0
WHICH=""

while read USER; do
    RUNNING=$(ps -U ${USER} -o state | grep "^R$" | wc -l)
    if [ ${RUNNING} -gt ${MAX} ]; then
	MAX=${RUNNING}
	WHICH=${USER}
    fi
done < <(ps -eo user | sort | uniq)

echo "User ${WHICH} with ${MAX} running processes."
