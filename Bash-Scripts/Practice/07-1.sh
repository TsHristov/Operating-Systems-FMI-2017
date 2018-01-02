#!/bin/bash

while read pid ppid; do
    PID_CHLDRN_CNT=$(ps --ppid ${pid} 2&>/dev/null | wc -l)
    PPID_CHLDRN_CNT=$(ps --ppid ${ppid} 2&>/dev/null | wc -l)
    test ${PID_CHLDRN_CNT} -gt ${PPID_CHLDRN_CNT} && echo ${pid}
done < <(ps -e -o pid= -o ppid=)
