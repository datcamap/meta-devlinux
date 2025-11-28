DESCRIPTION = "IMX708 Camera Overlay"
LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "file://imx708-overlay.dts"

DEPENDS += "dtc-native"
inherit kernel-arch

do_compile() {
    dtc -@ -I dts -O dtbo -o ${B}/imx708-overlay.dtbo ${WORKDIR}/imx708-overlay.dts
}

do_install() {
    install -d ${D}/boot/overlays
    install -m 0644 ${B}/imx708-overlay.dtbo ${D}/boot/overlays/
}

FILES:${PN} += "/boot/overlays/imx708-overlay.dtbo"
