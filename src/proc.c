#include "proc.h"
#include "dev.h"

// PROC OPERATIONS
static int proc_show(struct seq_file *m, void *v);
static int driver_proc_open(struct inode *inode, struct file *file);

static const struct proc_ops pops = {
    .proc_open = driver_proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

int proc_init(void)
{
    if (proc_create(PROC_NAME, PROC_MODE, NULL, &pops) == NULL)
    {
        pr_err("%s: could not create /proc/%s\n", DEV_NAME, PROC_NAME);
        return -ENOMEM;
    }
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

// PROC OPERATIONS
static int proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "device:   /dev/%s\n", DEV_NAME);
    seq_printf(m, "visitors: %d\n", dev_get_visitor_count());
    return 0;
}

static int driver_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_show, NULL);
}
