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
cam --camera=1 --stream=role=video,pixelformat=RGB565,width=1536,height=864 --capture=1500 --file=/tmp/frames.rgb565
```
