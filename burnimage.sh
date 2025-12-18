#!/bin/bash
WORKDIR=~/yocto/poky/build
MACHINE="raspberrypi0-2w"
IMAGE_TYPE="rpi-test-image"
SD_CARD="/dev/sdb"

IMAGE_PATH="$(ls ${WORKDIR}/tmp/deploy/images/${MACHINE}/${IMAGE_TYPE}*.rootfs.wic.bz2 | tail -n 1)"

echo "Burning image from: 
${IMAGE_PATH}"

sudo dd if=/dev/zero of=${SD_CARD} bs=512 count=1   # Errase sd card before writing
bzcat ${IMAGE_PATH} | sudo dd of=${SD_CARD} bs=4M status=progress
sync
