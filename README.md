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
