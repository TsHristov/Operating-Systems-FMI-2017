#!/bin/bash
if [ $# -ne 1 ]; then
    echo "usage: $0 <user>"
    exit 1
fi

USER_NAME=$1

echo "Per process RSS/RSZ ratio:"

while read PID RSS VSZ; do
    # Avoid division by zero:
    if [ ${VSZ} -gt 0 ]; then
	echo "PID: ${PID} ratio: $(echo "scale=5; ${RSS}/${VSZ}" | bc)"
    fi
done < <(ps -u ${USER_NAME} -o pid= -o rss= -o vsz= --sort -vsz)
