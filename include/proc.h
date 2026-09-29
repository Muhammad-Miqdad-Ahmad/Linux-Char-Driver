#ifndef PROC_H
#define PROC_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/* /proc/PROC_NAME */
#define PROC_NAME "heavy_driver"
#define PROC_MODE 0444

int proc_init(void);
void proc_exit(void);

#endif /* PROC_H */
