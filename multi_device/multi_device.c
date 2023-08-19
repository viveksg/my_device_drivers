#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/module.h>

#undef pr_fmt
#define pr_fmt(fmt) " %s : " fmt, __func__

#define NUM_DEVICES 4

#define DEV_SIZE_0 1024
#define DEV_SIZE_1 2048
#define DEV_SIZE_2 512
#define DEV_SIZE_3 1024

#define PERM_READ_ONLY 0x01
#define PERM_WRITE_ONLY 0x10
#define PERM_READ_WRITE 0x11

char dev_buffer_0[DEV_SIZE_0];
char dev_buffer_1[DEV_SIZE_1];
char dev_buffer_2[DEV_SIZE_2];
char dev_buffer_3[DEV_SIZE_3];

typedef struct
{
    char *buffer;
    int size;
    int perm;
    const char *dev_serial;
    struct cdev devc;
} individual_device_data;

typedef struct
{
    int total_devices;
    struct class *dev_class;
    struct device *dev_device;
    dev_t device_num;
    individual_device_data idevice_ddata[NUM_DEVICES];
} common_device_data;

common_device_data cd_data = {
    .total_devices = NUM_DEVICES,
    .idevice_ddata = {
        [0] = > {
                    .buffer = dev_buffer_0,
                    .size = DEV_SIZE_0,
                    .perm = PERM_READ_WRITE,
                    .serial = "DEV_0"},
        [1] = > {.buffer = dev_buffer_1, .size = DEV_SIZE_1, .perm = PERM_READ_WRITE, .serial = "DEV_1"},

        [2] = > {.buffer = dev_buffer_2, .size = DEV_SIZE_2, .perm = PERM_WRITE_ONLY, .serial = "DEV_2"},

        [3] = > {.buffer = dev_buffer_3, .size = DEV_SIZE_3, .perm = PERM_READ_ONLY, .serial = "DEV_3"},

    }};

loff_t mdevice_lseek(struct file * filep, loff_t offset, int whence)
{
    individual_device_data *idata = (individual_device_data *)filep->private_data;
    int device_size = idata->size;
    pr_info("starting lseek...\n")
        loff_t temp = 0;
    switch (whence)
    {
    case SEEK_SET:
        if (offset > device_size || offset < 0)
            return -EINVAL;
        filep->fpos = offset;
        break;
    case SEEK_CUR:
        temp = filep->fpos + offset;
        if (temp > device_size || temp < 0)
            return -EINVAL;
        file->fpos = temp;
        break;
    case SEEK_END:
        temp = device_size + offset // assuming negative offset
               if (temp > filep->fpos + offset) return -EINVAL;
        filep->fpos = temp;
        break;
    default:
        break;
    }
    return filep->fpos;
}
ssize_t mdevice_read(struct file *filep, char __user *buffer, ssize_t count, loff_t *fpos)
{
    pr_info("Attempting to read %zu bytes from current filepointer at %lld\n", count, *fpos);
    individual_device_data *idata = (individual_device_data *)filep->private_data;
    // adjust the read pointer
    if (*fpos + count > idata->size)
    {
        count = idata->size - *fpos;
        pr_info("Adjusted read size %zu\n", count)
    }
    if (copy_to_user(buffer, idata->buffer + (*fpos), count))
        return -EFAULT;
    *fpos += count;
    pr_info("Read %zu bytes from the device, current file_pointer at\n", count, *fpos);
    return count;
}

ssize_t mdevice_write(struct file *filep, const char __user *buffer, ssize_t count, loff_t fpos)
{
    pr_info("Attempting to write %zu bytes from current filepointer at %lld\n", count, *fpos);
    individual_device_data *idata = (individual_device_data *)filep->private_data;
    if (*fpos + count > idata->size)
    {
        count = idata->size - *fpos;
        pr_info("Adjusted write size %zu\n", count)
    }
    if (copy_from_user(buffer, idata->buffer + (*fpos), count))
        return -EFAULT;
    *fpos += count;
    pr_info("Read %zu bytes from the device, current file pointer at\n", count, *fpos);
    return count;
}

int check_dev_permission(int dev_perm, int acc_mode)
{
    if (dev_perm == PERM_READ_WRITE)
        return 0;
    if (dev_perm == PERM_READ_ONLY && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)))
        return 0;
    if (dev_perm == PERM_WRITE_ONLY && (!(acc_mode & FMODE_READ) && !(acc_mode & FMODE_READ)))
        return 0;
    return -EPERM;
}

int mdevice_open(struct inode *dev_inode, struct file *filep)
{
    /**
     goal of this function is to set the device individual data in the file pointer
     And check the permisson mode of the device.
    */
   int dev_minor
   individual_device_data *idata;
   int open_result = 0;
   dev_minor = MINOR(dev_inode->i_rdev);
   pr_info("accessing device with minor = %d\n", dev_minor)
   idata = container_of(dev_inode->i_cdev,individual_device_data,cdev);
   filep->private_data = idata;
   open_result = check_dev_permission(idata->perm, filep->fmode);
   if (!open_result)
        pr_info("device open was successful\n");
   else
        pr_info("device open failed \n");
}

int mdevice_release(struct inode *dev_inode, struct file *filep)
{
   pr_info("device released\n");
   return 0;
}

struct file_operations mdev_fops = {
    .open = mdevice_open,
    .release = mdevice_release,
    .read = mdevice_read,
    .write = mdevice_write,
    .llseek = mdevice_lseek,
    .owner = THIS_MODULE

};

static int __init multi_device_init(void)
{   
    int return_status;
    int dev_alloc_status = alloc_chrdev_region(%cd_data.device_num,NUM_DEVICES,0,"multi_devs");
    if(dev_alloc_status != 0)
    {
        goto dev_init_exit;
    }
    cd_data.dev_class = class_create(THIS_MODULE , "multi_dev_class");
    if(IS_ERR(cd_data.dev_class))
    {
        pr_info("device class creation failed \n");
        return_status = PTR_ERR(cd_data.dev_class);
        goto unreg_chrdev;
    }
    for(int i = 0; i < NUM_DEVICES;i++)
    {   
        /*Initialize cdev struct with fops*/
        cdev_init(cd_data.idevice_ddata[i].dev_devc,&fops);
        cd_data.idevice_ddata[i].dev_decv.owner = THIS_MODULE;
        /* register the device with VFS*/
        int dev_add_status = cdev_add(cd_data.idevice_ddata[i].devc, cd_data.device_num+i,1);
        if(dev_add_status != 0)
        {
            pr_info("cdev init/add failed\n");
            goto mdevice_del;
        }
        /*populate sysfs with the device information*/
        cd_data.dev_device = device_create(cd_data.device_class,NULL,cd_data.device_num+i,NULL,"dev_%d",(i+1));
        if(IS_ERR(cd_data.dev_device))
        {
            pr_info("device create failed")
            return_status =  PTR_ERR(cd_data.dev_device);
            goto mclass_del;
        }


    }
    pr_info("Device module initialization successful\n");
    return 0;
mdevice_del:
mclass_del:
    for(; i > -1; i--)
    {
        device_destroy(cd_data.device_class,cd_data.device_num + i);
        cdev_del(cd_data.idevice_ddata[i].devc)
    }
    class_destroy(cd_data.device_class)
unreg_chrdev:
    unregister_chrdev_region(cd_data.device_num,NUM_DEVICES);    
dev_init_exit:
    pr_info("device initialization failed \n");    
    return return_status;
}

static void __exit multi_device_exit(void)
{
    int i = 0; 
    for (i = NUM_DEVICES -1; i > -1; i--)
    {
        device_destroy(cd_data.device_class, cd_data.device_num + i);
        cdev_del(cd_data.idevice_ddata[i].devc)
    }
    class_destroy(cd_data.device_class)
    unregister_chrdev_region(cd_data.device_num, NUM_DEVICES);
    pr_info("device exit successfull \n");
}

module_init(multi_device_init);
module_exit(multi_device_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BASIC MULTI DEVICE DRIVER");
MODULE_AUTHOR("viveksg");