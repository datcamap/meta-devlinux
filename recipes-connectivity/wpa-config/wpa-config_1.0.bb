SUMMARY = "Install default wpa_supplicant.conf from template"
DESCRIPTION = "Installs a default wpa_supplicant.conf with build-time SSID/PSK substitutions."
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = "file://wpa_supplicant.conf \
           file://wpa_supplicant.service"

# Allow the recipe to be architecture-independent
inherit allarch
inherit systemd

# Default variables (can be overridden in local.conf)
WIFI_SSID ?= ""
WIFI_PSK ?= ""

do_install() {
    install -d ${D}${sysconfdir}/wpa_supplicant
    # substitute placeholders with build variables. If WIFI_PSK is empty we'll leave it blank.
    sed -e "s|@WIFI_SSID@|${WIFI_SSID}|g" \
        -e "s|@WIFI_PSK@|${WIFI_PSK}|g" \
        ${WORKDIR}/wpa_supplicant.conf > ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant.conf

    chmod 600 ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant.conf

    # Install systemd service file
    install -d ${D}/etc/systemd/system
    install -m 644 ${WORKDIR}/wpa_supplicant.service ${D}/etc/systemd/system/wpa_supplicant.service
}

# SYSTEMD_SERVICE_${PN} = "wpa_supplicant@wlan0.service"
# ${systemd_system_unitdir}
SYSTEMD_SERVICE_AUTO_ENABLE = "wpa_supplicant.service"

FILES_${PN} += "${sysconfdir}/wpa_supplicant /etc/systemd/system"
