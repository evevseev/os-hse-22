#!/bin/bash

# SysInfo.sh
# Script that gathers information about the system OS and prints it


# function that gathers system info
function get_system_info { 
  uname -a
  lsb_release -d -s
}

echo "-=-=-=-= System info =-=-=-=-"
get_system_info