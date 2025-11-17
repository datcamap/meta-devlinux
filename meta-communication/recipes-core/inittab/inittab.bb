SUMMARY = "Configure serial login"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/BSD;md5=ff6b8b26de76f38337a2b4aa04e3c0b5"

SRC_URI = "file://inittab"

do_install() {
    install -d ${D}/etc
    install -m 0644 ${WORKDIR}/inittab ${D}/etc/inittab
}
