#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

static int mdevice_prob(struct platform_device *pdev)
{
    pr_info("Probe initiated...\n");
    return 0;
}

static int mdevice_remove(struct platform_device *pdev)
{
    pr_info("device_removed called \n");
    return 0;
}

static struct platform_driver mp_drv = {
    .probe = mdevice_prob,
    .remove = mdevice_remove,
    .driver = {
        .name = "plat_ver1",
        .owner = THIS_MODULE,
    },
};

static int __init mdevice_init(void)
{
    pr_info("device init...\n");
    platform_driver_register(&mp_drv);
    return 0;
}

static void __exit mdevice_exit(void)
{   
    pr_info("device exit...\n");
    platform_driver_unregister(&mp_drv);
}

module_init(mdevice_init);
module_exit(mdevice_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple platform device deriver");
MODULE_AUTHOR("viveksg");