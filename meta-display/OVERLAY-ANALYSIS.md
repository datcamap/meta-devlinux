# ili9225 Overlay - Conflict Analysis & Fixes

## Issues Found in Original Overlay

### 1. ❌ Missing DTC Properties
**Problem:**
```dts
fragment@0 {
    target = <&spi0>;
    __overlay__ {
        status = "okay";
        display@0{ ... }
    };
}
```

**Issue:** Missing `#address-cells = <1>;` and `#size-cells = <0>;` in the spi0 fragment will cause device tree compiler warnings.

**Fix:** Added to fragment@2:
```dts
fragment@2 {
    target = <&spi0>;
    __overlay__ {
        status = "okay";
        #address-cells = <1>;
        #size-cells = <0>;
        ...
    };
}
```

---

### 2. ⚠️ SPI Device Conflict (CE0)

**Problem:**
The base device tree for RPi4 defines:
```dts
&spi0 {
    spidev0: spidev@0 {
        reg = <0>;  /* CE0 */
        ...
    };
    spidev1: spidev@1 {
        reg = <1>;  /* CE1 */
        ...
    };
};
```

Your original overlay also uses:
```dts
display@0 {
    reg = <0>;  /* Also CE0! */
    ...
}
```

**Conflict:** Both try to use the same chip select (CE0 / reg=0). This causes undefined behavior.

**Fix:** Disabled spidev0 in fragment@0:
```dts
fragment@0 {
    target = <&spidev0>;
    __overlay__ {
        status = "disabled";
    };
}
```

This removes the conflicting spidev and frees CE0 for your display device.

---

### 3. ❌ Missing GPIO Pin Configuration

**Problem:**
Your device uses GPIO 24 (reset) and GPIO 25 (rs/dc), but there's no pinctrl configuration defining how these pins should be set up.

**Fix:** Added fragment@1 to configure pins properly:
```dts
fragment@1 {
    target = <&gpio>;
    __overlay__ {
        ili9225_pins: ili9225_pins {
            brcm,pins = <24 25>;
            brcm,function = <1>;    /* Output mode */
            brcm,pull = <0>;        /* No pull */
        };
    };
}
```

Then in the device node:
```dts
pinctrl-names = "default";
pinctrl-0 = <&ili9225_pins>;
```

---

### 4. ➕ Added SPI Protocol Flags

**Enhancement:** Added optional SPI mode flags:
```dts
spi-cpol;  /* Clock polarity high */
spi-cpha;  /* Clock phase (sampling on second edge) */
```

These may or may not be needed depending on your ili9225 driver, but they're included for completeness.

---

## SPI Overlay Checklist

Use this checklist when creating SPI device overlays:

- [ ] Fragment 0: Disable conflicting `spidev@N` device (if using standard CS pins)
- [ ] Fragment 1: Configure GPIO pins via `&gpio` target
- [ ] Fragment 2: Configure `&spi0` (or other SPI bus) with:
  - [ ] `#address-cells = <1>`
  - [ ] `#size-cells = <0>`
  - [ ] Device node with `reg = <N>` (0 or 1 for spi0)
  - [ ] `compatible` string matching driver
  - [ ] `spi-max-frequency` set appropriately
  - [ ] `pinctrl-names` and `pinctrl-0` references
  - [ ] GPIO control pins (reset, dc, etc.)
- [ ] `__overrides__` section for runtime parameters

---

## Corrected ili9225-overlay.dts Structure

```
Fragment 0: Disable spidev@0
    ↓
Fragment 1: Define ili9225_pins on &gpio
    ↓
Fragment 2: Add display device on &spi0 with pinctrl binding
    ↓
__overrides__: Allow runtime parameter tuning
```

---

## Building & Testing

Build the overlay:
```bash
cd /home/dat/yocto/poky/build
bitbake linux-raspberrypi -c cleanall
bitbake core-image-base
```

Enable at runtime in `/boot/config.txt`:
```
dtoverlay=ili9225
```

Or with parameters:
```
dtoverlay=ili9225,speed=6000000
```

Check if it loaded:
```bash
sudo dmesg | grep ili9225
```

---

## GPIO Pin Mapping (RPi4)

Common pins used in overlays:
- **GPIO 7**: CE1 (SPI chip select)
- **GPIO 8**: CE0 (SPI chip select)
- **GPIO 24**: Often used for reset or Data/Command (DC)
- **GPIO 25**: Often used for reset or chip select

Your overlay uses GPIO 24 & 25, which are not standard SPI lines, so no conflict there.

---

## References

- [Device Tree Specification](https://devicetree-specification.readthedocs.io/)
- [Raspberry Pi Device Tree Documentation](https://github.com/raspberrypi/documentation)
- [Linux SPI Device Binding](https://www.kernel.org/doc/html/latest/devicetree/bindings/spi/spi-bus.txt)
