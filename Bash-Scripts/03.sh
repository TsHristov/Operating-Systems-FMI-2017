#!/bin/bash
if [ $(id -u) -ne 0 ]; then
    echo "Must be run as root"
    exit 1
fi

for USER in $(ps -e -o user= | sort | uniq); do
    TOTAL_RSS=0
    PS_COUNT=0
    
    while read CPID CRSS; do
	PS_COUNT=$(expr ${PS_COUNT} +1)
	TOTAL_RSS=$(expr ${TOTAL_RSS} + ${CRSS})
	MAX_RSS=${CRSS}
	MAX_RSS_PID=${CPID}
    done < <(ps -u ${USER} -o pid= -o rss= | sort -n -k 2)

    if [ ${PS_COUNT} -gt 0 ]; then
	AVERAGE_RSS=$(expr ${TOTAL_RSS} / ${PS_COUNT})
	echo "${user} ${PS_COUNT} ${TOTAL_RSS}"
	
	if [ ${MAX_RSS} -gt $(expr ${AVERAGE_RSS} "*" 2) ]; then
	    kill --signal SIGTERM ${MAX_RSS_PID}
	    sleep 2
	    kill --signal SIGKILL ${MAX_RSS_PID}
	fi
    fi
done

    
