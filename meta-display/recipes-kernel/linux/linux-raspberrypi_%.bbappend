FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
    file://0003-Fix-nokia5110-driver.patch \
    file://0004-Add-illi9225-device-tree-node.patch \
    "

KERNEL_MODULE:append = " nokia5110 ssd1306 spi_bcm2835 spi_bcm2835aux ili9225"
