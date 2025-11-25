#!/bin/bash

IFACE="${1:-wlan0}"

while true; do
    ip_addr=$(ip -4 -o addr show dev "$IFACE" 2>/dev/null | awk '{split($4,a,"/"); print a[1]}')
    if [ -n "$ip_addr" ]; then
        echo "\nIP address on $IFACE: \n$ip_addr\n" > /dev/sh11060
        break
    else
        echo "No IP found on $IFACE, retrying in 2 seconds..."
        sleep 2
    fi
done