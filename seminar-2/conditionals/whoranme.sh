#!/bin/bash

# WhoRanMe.sh
# Script that prints a message depending on who ran it 

if [ "$(id -u)" == 0 ]; then # get user id and check if it is 0 (root's id)
  echo "This script was run by root." # if so, print the corresponding message
else
  echo "This script was run by ordinary user." # otherwise, another message
fi
