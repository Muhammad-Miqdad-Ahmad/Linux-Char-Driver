#include "dev.h"

static dev_t dev_num;
static struct cdev driver_cdev;
static struct class *driver_class;

static int visitor_count = 0;

// HELPER FUNCTIONS
static void destroy_chrdev(void);
static void destroy_cdev(void);

// FILE OPERATIONS
static int dev_open(struct inode *inode, struct file *file)
{
    pr_info("%s: Calling the open function on the dev file %s", DEV_NAME, DEV_NAME);
    visitor_count++;
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    pr_info("%s: Calling the release function on the dev file %s", DEV_NAME, DEV_NAME);
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    char message[64];
    const int msg_len = scnprintf(message, sizeof(message), "The file has been opened %d times\n", visitor_count);

    if (msg_len < 0)
    {
        pr_err("%s: sprintf() err", DEV_NAME);
        return msg_len;
    }

    if (*offset >= msg_len)
        return 0;

    size_t to_copy = min(len, (size_t)(msg_len - *offset));

    if (copy_to_user(buf, message + *offset, to_copy))
    {
        pr_err("%s: copy_to_user() failed", DEV_NAME);
        return -EFAULT;
    }

    *offset += to_copy;

    return to_copy;
}

static ssize_t dev_write(
    struct file *file,
    const char __user *buffer,
    size_t length,
    loff_t *offset)
{
    char buf[52];
    size_t to_copy;

    to_copy = min(length, sizeof(buf) - 1);

    if (copy_from_user(buf, buffer, to_copy))
        return -EFAULT;

    buf[to_copy] = '\0';

    pr_alert("User wrote: %s\n", buf);

    *offset += to_copy;

    return to_copy;
}

static const struct file_operations dev_fpos = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

int dev_init(void)
{
    int err = alloc_chrdev_region(&dev_num, 0, DEV_MINORS, DEV_NAME);
    if (err < 0)
    {
        pr_err("%s: alloc_chrdev_region() failed", DEV_NAME);
        return err;
    }

    cdev_init(&driver_cdev, &dev_fpos);
    err = cdev_add(&driver_cdev, dev_num, DEV_MINORS);
    if (err < 0)
    {
        pr_err("%s: cdev_add() failed for MAJOR %u", DEV_NAME, MAJOR(dev_num));
        destroy_chrdev();
        return err;
    }

    // Now we have to create the dev file
    driver_class = class_create(DEV_CLASS);
    if (IS_ERR(driver_class))
    {
        pr_err("%s: class_create() failed for MAJOR %u", DEV_NAME, MAJOR(dev_num));
        destroy_cdev();
        destroy_chrdev();
        return PTR_ERR(driver_class);
    }

    struct device *device_err = device_create(driver_class, NULL, dev_num, NULL, DEV_NAME);
    if (IS_ERR(device_err))
    {
        pr_err("%s: device_create() failed for MAJOR %u", DEV_NAME, MAJOR(dev_num));
        class_destroy(driver_class);
        destroy_cdev();
        destroy_chrdev();
        return PTR_ERR(device_err);
    }
    pr_info("%s: INIT successful", DEV_NAME);
    return 0;
}

void dev_exit(void)
{
    device_destroy(driver_class, dev_num);
    class_destroy(driver_class);
    destroy_cdev();
    destroy_chrdev();
}

// HELPERS
static void destroy_cdev(void)
{
    cdev_del(&driver_cdev);
}

static void destroy_chrdev(void)
{
    unregister_chrdev_region(dev_num, DEV_MINORS);
}

int dev_get_visitor_count(void)
{
    return visitor_count;
}