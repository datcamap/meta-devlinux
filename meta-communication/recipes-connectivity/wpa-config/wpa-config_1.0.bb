SUMMARY = "Install default wpa_supplicant.conf from template"
DESCRIPTION = "Installs a default wpa_supplicant.conf with build-time SSID/PSK substitutions."
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = "file://wpa_supplicant.conf \
           file://wpa_supplicant.service"

inherit systemd

WIFI_SSID ?= ""
WIFI_PSK ?= ""

do_install() {
    install -d ${D}${sysconfdir}/wpa_supplicant
    sed -e "s|@WIFI_SSID@|${WIFI_SSID}|g" \
        -e "s|@WIFI_PSK@|${WIFI_PSK}|g" \
        ${WORKDIR}/wpa_supplicant.conf > ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant.conf

    chmod 600 ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant.conf

    install -d ${D}${sysconfdir}/systemd/system
    install -m 644 ${WORKDIR}/wpa_supplicant.service ${D}${sysconfdir}/systemd/system/wpa_supplicant.service
}

SYSTEMD_SERVICE:${PN} = "wpa_supplicant.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

FILES_${PN} = "${sysconfdir}/wpa_supplicant ${sysconfdir}/systemd/system/wpa_supplicant.service"