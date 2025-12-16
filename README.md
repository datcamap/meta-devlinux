# My custom yocto layers for Raspberry Pi 4 Model B

### *These layers are only compatible with scarthgap branch*

**meta-communication:** UART console support and Wifi configuration
**meta-display:** support ILI9225 TFT display on SPI0 bus and SH1106 monitor on I2C1 bus
**meta-camera:** add support for raspberry camera module 3

### Initialize build evironment
```bash
source oe-init-build-env
```

### Adding each layer to your build by running 

```bash
bitbake-layers add-layer [path to layer]
```

### Also add these to your conf/local.conf file

```conf
MACHINE = "raspberrypi4"

# Default Wi-Fi credentials consumed by the wifi-config recipe.
# Replace with the SSID and passphrase of your access point
WIFI_SSID = ""
WIFI_PSK = ""

DISTRO_FEATURES:append = " systemd usrmerge wifi"

VIRTUAL-RUNTIME_init_manager = "systemd"
VIRTUAL-RUNTIME_initscripts = "systemd-compat-units"
VIRTUAL-RUNTIME_init = "systemd"
PREFERRED_PROVIDER_virtual/init = "systemd"
IMAGE_INSTALL:remove = " sysvinit"

LICENSE_FLAGS_ACCEPTED += " commercial synaptics-killswitch"
RASPBERRYPI_CAMERA_V3 = "1"

RM_OLD_IMGAGES = "1"
INHERIT += "extrausers rm_work"
EXTRA_USERS_PARAMS = "groupadd netdev; "
```

### Build image

```bash
bitbake rpi-test-image
```

### Burn image

I've create a small script that can automatically burn image to sd card, modify `burnimage.sh` if needed then execute it

## This is test commands

```bash
ffmpeg -i out.png -f rawvideo -vf scale=220:176 -pix_fmt rgba output.rgba
ffmpeg -i out.png -vf scale=220:176 -pix_fmt bgra -f rawvideo /dev/fb0

cam --camera=1 --stream=role=video,pixelformat=RGB565,width=1536,height=864 --capture=1500 --file=/tmp/frames.rgb565

ffmpeg -f rawvideo -pixel_format rgb565le -video_size 4608x2592 -i frame.rgb565 -q:v 2 frame.jpg

ffmpeg -f rawvideo -pixel_format rgb565le -video_size 1536x864 -framerate 30 -i /tmp/frames.rgb565 -vcodec libx264 -pix_fmt yuv420p /tmp/output.mp4

ffmpeg -f rawvideo -pixel_format rgb565le -video_size 2304x1296 -framerate 30 -i frames.rgb565 -vcodec mjpeg output.avi
```

### [In progress] Check IC power supply: 
	+ [Done] 5V supply already available
	+ [Pending] Checking current
### Porting monitor (ILI9225,..)
	+ [Done] Modify device tree, confirm ILI9225 module loaded on runtime (OK)
	+ [Done] Checking SPI signal presence on MOSI pin (OK)
	+ [Pending] Check framebuffer input format (RGBA?)
### Write a driver to control gpios, exposing as a character device
	+ power-pin: GPIO17
	+ LED-pin: GPIO18
	+ halfcup-pin: GPIO22
	+ fullcup-pin: GPIO23
### Make Qt app to show 2 buttons and send control string to driver
	+ "on" : power on
	+ "off": power off
	+ "half": half cup mode
	+ "full": full cup mode
### Pending features
- Server host
- Wireless control