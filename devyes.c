#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define BUF_SIZE (1UL<<17)
char yes_buf[BUF_SIZE+2];

static dev_t dev;
static struct cdev cdev;
static struct class *class;

static ssize_t devyes_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
	len = min(len, BUF_SIZE);
	if (copy_to_user(buf, &yes_buf[*off % 2], len) != 0) {
		return -EFAULT;
	}
	*off += len;
	return len;
}

static struct file_operations devyes_fops = {
	.owner = THIS_MODULE,
	.read = devyes_read,
};

int __init devyes_module_init(void) {
	int result = 0;
	size_t i;
	struct device *device;

	for (i = 0; i < sizeof(yes_buf); i++) {
		yes_buf[i] = (i % 2 == 0) ? 'y' : '\n';
	}

	result = alloc_chrdev_region(&dev, 0, 1, "devyes");
	if (result < 0) {
		goto fail;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
	class = class_create("devyes");
#else
	class = class_create(THIS_MODULE, "devyes");
#endif
	if (IS_ERR(class)) {
		result = PTR_ERR(class);
		goto fail_chrdev_region;
	}

	if (IS_ERR(device = device_create(class, NULL, dev, NULL, "yes"))) {
		result = PTR_ERR(device);
		goto fail_class;
	}

	cdev_init(&cdev, &devyes_fops);
	result = cdev_add(&cdev, dev, 1);
	if (result < 0) {
		goto fail_device;
	}

	return 0;

fail_device:
	device_destroy(class, dev);
fail_class:
	class_destroy(class);
fail_chrdev_region:
	unregister_chrdev_region(dev, 1);
fail:
	return result;
}

void __exit devyes_module_exit(void) {
	cdev_del(&cdev);
	device_destroy(class, dev);
	class_destroy(class);
	unregister_chrdev_region(dev, 1);
}

module_init(devyes_module_init);
module_exit(devyes_module_exit);
MODULE_AUTHOR("Julian Brost <julian@0x4a42.net>");
MODULE_DESCRIPTION("/dev/yes");
MODULE_LICENSE("GPL");
