#!/bin/bash
IMAGE_PATH="$(ls ~/yocto/poky/build/tmp/deploy/images/raspberrypi4/core-image-minimal*.rootfs.wic.bz2 | tail -n 1)"

echo "Burning image from: 
${IMAGE_PATH}"

mkdir -p ~/yocto/poky/temporary
rm -f ~/yocto/poky/temporary/temp.wic

cp ${IMAGE_PATH} ~/yocto/poky/temporary/temp.wic.bz2
sudo dd if=/dev/zero of=/dev/sdb bs=512 count=1
bzcat ~/yocto/poky/temporary/temp.wic.bz2 | sudo dd of=/dev/sdb bs=4M status=progress
sync
