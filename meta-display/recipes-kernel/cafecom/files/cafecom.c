// SPDX-License-Identifier: GPL-2.0
/* gpio-cafecom.c - simple platform driver exposing char device to control GPIOs
 *
 * Controls GPIOs from device tree node properties:
 *  - power-gpios: power control (active-low in DT)
 *  - halfcup-gpios: half-cup control (active-low in DT)
 *  - fullcup-gpios: full-cup control (active-low in DT)
 *  - led-gpios: input GPIO to read status
 *
 * User writes the following strings to the char device to control the hardware:
 *  "on"  -> power on
 *  "off" -> power off
 *  "half"-> half-cup mode
 *  "full"-> full-cup mode
 *
 * Reading the char device returns the current `led` input state as "0\n" or "1\n".
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/slab.h>

#define DRIVER_NAME "cafe_com"
#define DEVICE_NAME "cafe_com"
#define MAX_CMDLEN 32

struct cafe_dev {
    struct device *dev;
    struct gpio_desc *power;
    struct gpio_desc *halfcup;
    struct gpio_desc *fullcup;
    struct gpio_desc *led; /* input */

    dev_t devt;
    struct cdev cdev;
    struct class *class;
    struct mutex lock; /* protects state */
};

static ssize_t cafe_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct cafe_dev *c = file->private_data;
    char out[4];
    int val;

    if (*ppos > 0)
        return 0; /* EOF */

    if (!c->led)
        return -ENODATA;

    val = gpiod_get_value_cansleep(c->led);
    snprintf(out, sizeof(out), "%d\n", val ? 1 : 0);

    if (copy_to_user(buf, out, strlen(out)))
        return -EFAULT;

    *ppos = strlen(out);
    return strlen(out);
}

static int handle_command(struct cafe_dev *c, const char *cmd)
{
    if (!cmd)
        return -EINVAL;

    if (strncmp(cmd, "on", 2) == 0) {
        /* power on */
        if (c->power)
            gpiod_set_value_cansleep(c->power, 1);
        /* clear cup lines */
        if (c->halfcup)
            gpiod_set_value_cansleep(c->halfcup, 0);
        if (c->fullcup)
            gpiod_set_value_cansleep(c->fullcup, 0);
    } else if (strncmp(cmd, "off", 3) == 0) {
        if (c->power)
            gpiod_set_value_cansleep(c->power, 0);
        if (c->halfcup)
            gpiod_set_value_cansleep(c->halfcup, 0);
        if (c->fullcup)
            gpiod_set_value_cansleep(c->fullcup, 0);
    } else if (strncmp(cmd, "half", 4) == 0) {
        if (c->power)
            gpiod_set_value_cansleep(c->power, 1);
        if (c->halfcup)
            gpiod_set_value_cansleep(c->halfcup, 1);
        if (c->fullcup)
            gpiod_set_value_cansleep(c->fullcup, 0);
    } else if (strncmp(cmd, "full", 4) == 0) {
        if (c->power)
            gpiod_set_value_cansleep(c->power, 1);
        if (c->halfcup)
            gpiod_set_value_cansleep(c->halfcup, 0);
        if (c->fullcup)
            gpiod_set_value_cansleep(c->fullcup, 1);
    } else {
        return -EINVAL;
    }

    return 0;
}

static ssize_t cafe_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    struct cafe_dev *c = file->private_data;
    char kbuf[MAX_CMDLEN];
    size_t len = min(count, (size_t)(MAX_CMDLEN - 1));
    int ret;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;
    kbuf[len] = '\0';

    /* trim trailing newline or spaces */
    while (len && (kbuf[len-1] == '\n' || kbuf[len-1] == '\r' || kbuf[len-1] == ' ')) {
        kbuf[len-1] = '\0';
        len--;
    }

    mutex_lock(&c->lock);
    ret = handle_command(c, kbuf);
    mutex_unlock(&c->lock);

    if (ret)
        return ret;

    return count;
}

static int cafe_open(struct inode *inode, struct file *file)
{
    struct cafe_dev *c = container_of(inode->i_cdev, struct cafe_dev, cdev);
    file->private_data = c;
    return 0;
}

static int cafe_release(struct inode *inode, struct file *file)
{
    return 0;
}

static const struct file_operations cafe_fops = {
    .owner = THIS_MODULE,
    .open = cafe_open,
    .release = cafe_release,
    .read = cafe_read,
    .write = cafe_write,
};

static int cafe_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct cafe_dev *c;
    int ret;

    c = devm_kzalloc(dev, sizeof(*c), GFP_KERNEL);
    if (!c)
        return -ENOMEM;

    mutex_init(&c->lock);
    c->dev = dev;

    /* Get GPIOs from DT. Use devm_gpiod_get_optional so missing lines are allowed. Flags:
     * DT already marks active-low where applicable; request logical OUT/IN behavior.
     */
    c->power = devm_gpiod_get_optional(dev, "power", GPIOD_OUT_LOW);
    if (IS_ERR(c->power)) {
        dev_err(dev, "failed to get power gpio\n");
        return PTR_ERR(c->power);
    }

    c->halfcup = devm_gpiod_get_optional(dev, "halfcup", GPIOD_OUT_LOW);
    if (IS_ERR(c->halfcup))
        return PTR_ERR(c->halfcup);

    c->fullcup = devm_gpiod_get_optional(dev, "fullcup", GPIOD_OUT_LOW);
    if (IS_ERR(c->fullcup))
        return PTR_ERR(c->fullcup);

    c->led = devm_gpiod_get_optional(dev, "led", GPIOD_IN);
    if (IS_ERR(c->led))
        return PTR_ERR(c->led);

    /* Register char device */
    ret = alloc_chrdev_region(&c->devt, 0, 1, DEVICE_NAME);
    if (ret) {
        dev_err(dev, "failed alloc_chrdev_region: %d\n", ret);
        return ret;
    }

    cdev_init(&c->cdev, &cafe_fops);
    c->cdev.owner = THIS_MODULE;
    ret = cdev_add(&c->cdev, c->devt, 1);
    if (ret) {
        dev_err(dev, "cdev_add failed: %d\n", ret);
        unregister_chrdev_region(c->devt, 1);
        return ret;
    }

    c->class = class_create(DEVICE_NAME);
    if (IS_ERR(c->class)) {
        dev_err(dev, "class_create failed\n");
        ret = PTR_ERR(c->class);
        goto err_cdev;
    }

    if (!device_create(c->class, dev, c->devt, NULL, DEVICE_NAME)) {
        dev_err(dev, "device_create failed\n");
        ret = -ENODEV;
        goto err_class;
    }

    platform_set_drvdata(pdev, c);
    dev_info(dev, "cafe_com probed, device /dev/%s\n", DEVICE_NAME);
    return 0;

err_class:
    class_destroy(c->class);
err_cdev:
    cdev_del(&c->cdev);
    unregister_chrdev_region(c->devt, 1);
    return ret;
}

static int cafe_remove(struct platform_device *pdev)
{
    struct cafe_dev *c = platform_get_drvdata(pdev);

    if (!c)
        return 0;

    device_destroy(c->class, c->devt);
    class_destroy(c->class);
    cdev_del(&c->cdev);
    unregister_chrdev_region(c->devt, 1);

    dev_info(c->dev, "cafe_com removed\n");
    return 0;
}

static const struct of_device_id cafe_of_match[] = {
    { .compatible = "devlinux,cafecom" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, cafe_of_match);

static struct platform_driver cafe_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = cafe_of_match,
    },
    .probe = cafe_probe,
    .remove = cafe_remove,
};

module_platform_driver(cafe_driver);

MODULE_AUTHOR("DevLinux");
MODULE_DESCRIPTION("Cafe COM GPIO control char device");
MODULE_LICENSE("GPL");
