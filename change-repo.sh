#!/bin/bash
# Change it on system
REPO="$2"
CHROOT_LOCATION="$1"
cd $1
sed -i "s/REPO=.*/REPO=$REPO/" usr/bin/mercury

