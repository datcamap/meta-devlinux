FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://output.rgb565"

do_install:append() {
    install -d ${D}${datadir}/images
    install -m 0644 ${WORKDIR}/output.rgb565 ${D}${datadir}/images/output.rgb565
}

FILES:${PN} += "${datadir}/images/output.rgb565"

SRC_URI += "file://login-command.sh"

do_install:append() {
    install -d ${D}${sysconfdir}/profile.d
    install -m 0644 ${WORKDIR}/login-command.sh ${D}${sysconfdir}/profile.d/login-command.sh
}

FILES:${PN} += "${sysconfdir}/profile.d/login-command.sh"