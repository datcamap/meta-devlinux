SUMMARY = "Enable UART login via systemd"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://serial-getty@.service;md5=3a7bd3f18b6a5ef3d4b2a0c123456789"

SRC_URI = "file://serial-getty@.service"

inherit systemd

SYSTEMD_SERVICE_${PN} = "serial-getty@ttyAMA0.service"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/serial-getty@.service ${D}${systemd_system_unitdir}/
}
