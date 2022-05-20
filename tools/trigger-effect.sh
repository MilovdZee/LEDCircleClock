#!/bin/bash

IP=192.168.0.18

if [ "$1" == "" ]
then
  echo "Usage: $0 effect-nr [-l]"
  echo "Pass optional -l option to loop the effect."
  echo "See file effects.ino for which effect number is what effect."
  echo "Please update the IP variable in this script to point to the clock. Current value: $IP."
  echo "Example: $0 11 -l"
  exit 1
fi


if [ "$2" == "-l" ]
then
  while true
  do
   curl -m 2 http://$IP/effect -X POST -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "effect=$1"
   sleep 2
  done
else
  curl -m 2 http://$IP/effect -X POST -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "effect=$1"
fi

