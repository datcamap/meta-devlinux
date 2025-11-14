# Add SSH server support
IMAGE_FEATURES:append = " ssh-server-openssh"
IMAGE_INSTALL:append = " openssh openssh-sftp-server"

# Add login support
# IMAGE_INSTALL:append = " busybox"
IMAGE_INSTALL:append = " systemd"

# Add firmware for various devices
IMAGE_INSTALL:append = " linux-firmware"

# Add WiFi support
IMAGE_INSTALL:append = " wpa-supplicant wpa-config dhcpcd iw"

# Add Vim editor
IMAGE_INSTALL:append = " vim"
