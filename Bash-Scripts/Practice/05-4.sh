#!/bin/bash
# Shows the name of the process that has used the most CPU time for each user in the system:

while read USER; do
    echo "User: ${USER} , Process: $(ps -U ${USER} -o comm= --sort -cputime | head -n 1)"
done < <(ps -eo user | sort | uniq)
