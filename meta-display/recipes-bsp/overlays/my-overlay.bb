SUMMARY = "Compiled device tree overlays for Raspberry Pi"
DESCRIPTION = "Compiles DTS overlay files"
LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI = "file://ili9225-overlay.dts"

DEPENDS = "dtc-native"

S = "${WORKDIR}"

do_compile() {
    dtc -@ -I dts -O dtb -o ili9225.dtbo ili9225-overlay.dts
}

do_install() {
    install -d ${D}/boot/overlays
    install -m 0644 ili9225.dtbo ${D}/boot/overlays/
}

FILES:${PN} = "/boot/overlays/*.dtbo"








