FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://serial-fragment.cfg"
KERNEL_CONFIG_FRAGMENTS += "serial-fragment.cfg"

KERNEL_MODULE_AUTOLOAD:append = " cfg80211"
