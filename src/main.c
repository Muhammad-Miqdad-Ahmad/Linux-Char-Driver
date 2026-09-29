#include "main.h"

static int __init heavy_driver_init(void)
{
    int err_check = dev_init();

    if (err_check != 0)
        return err_check;

    err_check = proc_init();

    if (err_check != 0)
    {
        dev_exit();
        return err_check;
    }

    return 0;
}

static void __exit heavy_driver_exit(void)
{
    proc_exit();
    dev_exit();
}

module_init(heavy_driver_init);
module_exit(heavy_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Muhammad Miqdad Ahmad");
MODULE_DESCRIPTION("Major Driver");
