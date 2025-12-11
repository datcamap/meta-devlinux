# Add imx708 camera overlay to kernel device tree overlays
# This overlay enables the Sony IMX708 camera sensor on Raspberry Pi

RPI_KERNEL_DEVICETREE_OVERLAYS:append = " overlays/imx708.dtbo"
