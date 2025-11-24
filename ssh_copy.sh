#!/bin/bash
FILE_PATH="$(find /home/dat/yocto/poky/build/tmp -name $1 )"

echo "Sending file: 
${FILE_PATH}"

scp ${FILE_PATH} root@192.168.1.35:
