#!/bin/env bash

# Make sure we're root/sudo
if [ "$(id -u)" -ne 0 ]; then 
    printf "%s\n" "This script needs to be run with sudo."
    exit 1
fi

# Delete any old profiles in system-wide opensd dir
id [ -d 
