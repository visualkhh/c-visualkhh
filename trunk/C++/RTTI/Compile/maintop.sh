#!/bin/bash
#PIDS=`pgrep main`
PID=`ps -eo "%c %r %a"  |grep main |grep -v grep | grep -v .sh |  awk '{print $2}'`

echo $PID
top -p $PID

