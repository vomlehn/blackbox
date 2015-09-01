#!/bin/sh

# Test blackbox devices

sync				# Reduce likelihood of disk corruption

if sudo modprobe blackbox; then
	echo Installed backbox

	if sudo modprobe blackbox-ram;then
		echo Installed Blackbox-ram
		sudo modprobe -r blackbox_ram
	else
		echo Failed to install Blackbox-ram
	fi

	sudo modprobe -r blackbox
else
	echo Failed to install Blackbox
fi

dmesg | tail -15
