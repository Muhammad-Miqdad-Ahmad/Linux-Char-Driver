#ifndef DEV_H
#define DEV_H

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

/* /dev/DEV_NAME */
#define DEV_NAME   "DRIVER"
#define DEV_CLASS  "DRIVER_CLASS"
#define DEV_MINORS 1

int dev_init(void);
void dev_exit(void);
int dev_get_visitor_count(void);

#endif /* DEV_H */
