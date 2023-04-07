#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Please provide a file name as an argument."
  exit 1
fi

echo "$(grep -n 'number' $1 | grep -oP "LINE\s+\K\w+" | awk -F: '{print "LINE : " $1}' | wc -l) lines found."
for i in $(grep -n 'number' $1 | grep -oP "LINE\s+\K\w+" | awk -F: '{print $1}')
do
  echo "LINE : $i"
done
