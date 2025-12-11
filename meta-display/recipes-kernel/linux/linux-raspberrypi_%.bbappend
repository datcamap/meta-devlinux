FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# SRC_URI += " \
#     file://0003-Fix-nokia5110-driver.patch \
#     file://0004-Add-illi9225-device-tree-node.patch \
#     file://0005-Add-sh1106-driver.patch \
#     file://0006-modify-device-tree-to-port-imx708.patch \
#     file://0007-modify-unicam-embedded-data-size.patch"

KERNEL_MODULE:append = " nokia5110"
KERNEL_MODULE:append = " ssd1306"
KERNEL_MODULE:append = " sh1106"
KERNEL_MODULE:append = " spi_bcm2835"
KERNEL_MODULE:append = " spi_bcm2835aux"
# KERNEL_MODULE:append = " ssd1307fb"
# KERNEL_MODULE:append = " ili9225"

KERNEL_MODULE_AUTOLOAD:append = " nokia5110"
KERNEL_MODULE_AUTOLOAD:append = " sh1106"
