SUMMARY = "Install default wpa_supplicant.conf from template"
DESCRIPTION = "Installs a default wpa_supplicant.conf with build-time SSID/PSK substitutions."
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = "file://wpa_supplicant.conf \
           file://wpa_supplicant.service \
           file://wait_ip.sh \
           file://print_ip_address.service"

inherit systemd

WIFI_SSID ?= ""
WIFI_PSK ?= ""

do_install:append() {
    install -d ${D}${sysconfdir}/wpa_supplicant
    sed -e "s|@WIFI_SSID@|${WIFI_SSID}|g" \
        -e "s|@WIFI_PSK@|${WIFI_PSK}|g" \
        ${WORKDIR}/wpa_supplicant.conf > ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant.conf

    chmod 0600 ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant.conf

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/wpa_supplicant.service ${D}${systemd_system_unitdir}/my_wpa_supplicant.service

    install -d ${D}${sbindir}
    install -m 0755 ${WORKDIR}/wait_ip.sh ${D}${sbindir}/wait_ip.sh

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/print_ip_address.service ${D}${systemd_system_unitdir}/print_ip_address.service
}

RDEPENDS:${PN} = "wpa-supplicant bash"

SYSTEMD_SERVICE:${PN} = "my_wpa_supplicant.service print_ip_address.service"

SYSTEMD_AUTO_ENABLE:${PN} = "enable"

# FILES:${PN} = "${sysconfdir}/wpa_supplicant ${sysconfdir}/systemd/system/wpa_supplicant.service"