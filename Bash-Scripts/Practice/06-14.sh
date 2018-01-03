#!/bin/bash
test $# -ne 0 && exit 1

FILES=$*
read -p "Search query: " STRING

for FILE in ${FILES}; do
    if [ -f ${FILE} -a -r ${FILE} ]; then
	echo "$(grep -F "${STRING}" "${FILE}" | wc -l) lines matching ${STRING} in ${FILE}"
    else
	echo "${FILE} does not exists or cannot be read"
    fi
done

