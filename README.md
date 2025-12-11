## My custom yocto layers for Raspberry Pi 4 Model B

__meta-communication:__ UART console support and Wifi configuration
__meta-display:__ support ILI9225 TFT display on SPI0 bus and SH1106 monitor on I2C1 bus

### Adding each layer to your build by running 

```bash
bitbake-layers add-layer [path to layer]
```

### Also add these to your conf/local.conf file

```conf
MACHINE ?= "raspberrypi4"

# Default Wi-Fi credentials consumed by the wifi-config recipe.
# Replace with the SSID and passphrase for the access point you want the image to join.
WIFI_SSID = ""
WIFI_PSK = ""

# Ensure Wi-Fi support is available to the image.
DISTRO_FEATURES:append = " systemd wifi"
VIRTUAL-RUNTIME_init_manager = "systemd"
VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"
VIRTUAL-RUNTIME_init = "systemd"
PREFERRED_PROVIDER_virtual/init = "systemd"

IMAGE_INSTALL:remove = " sysvinit"

RM_OLD_IMGAGES = "1"
INHERIT += "extrausers rm_work"
EXTRA_USERS_PARAMS = "groupadd netdev; "
```

```bash
v4l2-ctl -d /dev/video0 --set-fmt-video=width=4608,height=2592,pixelformat=YUYV --stream-mmap --stream-count=1 --stream-to=frame.yuv

/lib/modules/5.15.92-v7/kernel/drivers/media/platform/bcm2835/bcm2835-unicam.ko.xz

v4l2-ctl -d /dev/video0 --set-fmt-video=width=4096,height=2592,pixelformat=BA12 --stream-mmap --stream-count=1 --stream-to=raw_image.ba12

v4l2-ctl -d /dev/video0 --set-fmt-video=width=4608,height=2592,pixelformat=RG10 --stream-mmap --stream-count=1 --stream-to=frame.raw --verbose

media-ctl -d /dev/media0 -v -V "'imx708':1 [fmt:METADATA_8/28800x1 field:none]"

v4l2-ctl -d /dev/video0 --set-fmt-video=width=2304,height=1296,pixelformat=RG10 --stream-mmap --stream-count=1 --stream-to=frame.raw

v4l2-ctl -d /dev/video14 --set-fmt-video=width=2304,height=1296,pixelformat=RGBP --stream-mmap --stream-count=1 --stream-to=frame.rgb565

v4l2-ctl -d /dev/video13 --set-fmt-video=pixelformat=RG10,width=4608,height=2592

media-ctl -d /dev/media0 -V '"imx708":0 [fmt:SRGGB10_1X10/4608x2592]' --verbose

check camera hong ngoai
Change mode cua camera
Them supported mode cho driver

gst-launch-1.0 filesrc location=frame.raw ! rawvideoparse width=2304 height=1296 format=bayer_rggb10 ! bayer2rgb ! videoconvert ! video/x-raw,format=RGB16 ! filesink location=frame.rgb565

media-ctl 1.30.1
v4l2-ctl 1.30.1

v4l-utils:
  meta-oe              1.22.1
  
cam --camera=1 --stream=role=video,pixelformat=RGB565,width=1536,height=864 --capture=1500 --file=/tmp/frames.rgb565

ffmpeg -f rawvideo -pixel_format rgb565le -video_size 4608x2592 -i frame.rgb565 -q:v 2 frame.jpg

ffmpeg -f rawvideo -pixel_format rgb565le -video_size 1536x864 -framerate 30 -i /tmp/frames.rgb565 -vcodec libx264 -pix_fmt yuv420p /tmp/output.mp4

ffmpeg -f rawvideo -pixel_format rgb565le -video_size 2304x1296 -framerate 30 -i frames.rgb565 -vcodec mjpeg output.avi
```