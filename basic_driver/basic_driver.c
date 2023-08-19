#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt, __func__

#define DEV_MEM_SIZE 1024

char device_buffer[DEV_MEM_SIZE];

dev_t device_num;
struct cdev char_device;
struct class *dev_class;
struct device *char_dev;

/**
 * @brief op_lseek sets the file position based on the current value of the whence
 *
 * @param filep pointer to the device file;
 * @param offset the current file offset, based on whence value;
 * @param whence the identifier to tell in which direction the offset should be seeked
 *  SEEK_SET means set the passed offset as the current file pointer,
 *  SEEK_CUR means set the file pointer from the current value of file pointer
 *  SEEL_END meams set the file pointer from the END of the file
 * @return loff_t current file pointer
 */
loff_t op_lseek(struct file *filep, loff_t offset, int whence)
{
   loff_t ret_val;
   pr_info("In Lseek: \n");
   pr_info("Current value of file_pointer: %lld", filep->f_pos);
   switch (whence)
   {
   case SEEK_SET:
      if ((offset > DEV_MEM_SIZE) || (offset < 0))
         return -EINVAL;
      filep->f_pos = offset;
      break;
   case SEEK_CUR:
      ret_val = filep->f_pos + offset;
      if ((ret_val > DEV_MEM_SIZE) || (ret_val < 0))
         return -EINVAL;
      filep->f_pos = ret_val;
      break;
   case SEEK_END:
      ret_val = DEV_MEM_SIZE + offset;
      if((ret_val > DEV_MEM_SIZE) || (ret_val < 0))
         return -EINVAL;
      filep->f_pos = ret_val;
      break;
   default:
      return -EINVAL;
   }
   pr_info("New valye of the file position = %lld\n", filep->f_pos);
   return filep->f_pos;
}

ssize_t op_read(struct file *filep, char __user *buff, size_t count, loff_t *f_pos)
{
   pr_info("In Read op \n");
   pr_info("Current file position: %lld , bytes requested : %zu\n", *f_pos, count);

   if ((*f_pos + count) > DEV_MEM_SIZE)
      count = DEV_MEM_SIZE - *f_pos;
   if (copy_to_user(buff, &device_buffer[*f_pos], count))
   {
      return -EFAULT;
   }

   *f_pos += count;
   pr_info("Updated file_pointer %lld, bytes written %zu\n", *f_pos, count);
   return 0;
}

ssize_t op_write(struct file *filep, const char __user *buff, size_t count, loff_t *f_pos)
{
   pr_info("In Write op \n");
   pr_info("Current file pointer : %lld, bytes writtem %zu\n", *f_pos, count);

   if ((*f_pos + count) > DEV_MEM_SIZE)
      count = DEV_MEM_SIZE - *f_pos;
   if (!count)
   {
      pr_info("Not enough memory to write bytes \n");
      return -ENOMEM;
   }

   if (copy_from_user(&device_buffer[*f_pos], buff, count))
   {
      return -EFAULT;
   }

   *f_pos += count;
   pr_info("Number of bytes written %zu, current file pointer %lld\n",count, *f_pos);
   return 0;
}

int op_open(struct inode *inode, struct file *filp)
{
   pr_info("open called \n");
   return 0;
}

int op_release(struct inode *inode, struct file *filp)
{
   pr_info("closed called \n");
   return 0;
}
#include <linux/module.h>
struct file_operations op_fops = {
    .open = op_open,
    .release = op_release,
    .read = op_read,
    .write = op_write,
    .llseek = op_lseek,
    .owner = THIS_MODULE};
static int __init basic_driver_init(void)
{
   int ret = 0;
   /**1. obtain the device number*/
   ret = alloc_chrdev_region(&device_num, 0, 1, "basic_dev");
   if (ret < 0)
   {
      pr_err("Cannot obtain the device number\n");
      goto out;
   }
   pr_info("Device number <major>:<minor> =%d:%d", MAJOR(device_num), MINOR(device_num));

   /*2. Initialize the cdev structure with fileoperations*/
   cdev_init(&char_device, &op_fops);

   /*3. Register the device structure with VFS*/
   char_device.owner = THIS_MODULE;
   ret = cdev_add(&char_device, device_num, 1);
   if (ret < 0)
   {
      pr_err("Cannot register device to the VFS\n");
      goto unreg_chrdev;
   }

   /*4 create a device class under /sys/class/* */
   dev_class = class_create(THIS_MODULE, "basic_class");
   if (IS_ERR(dev_class))
   {
      pr_err("Device class creation failed\n");
      ret = PTR_ERR(dev_class);
      goto cdev_del;
   }

   /*5 populate the sysfs with device information*/
   char_dev = device_create(dev_class, NULL, device_num, NULL, "basic_dev");
   if (IS_ERR(char_dev))
   {
      pr_err("Cannot register device to sysfs\n");
      ret = PTR_ERR(char_dev);
      goto class_del;
   }
   pr_info("Module initialization successful\n");
class_del:
   class_destroy(dev_class);
cdev_del:
   cdev_del(&char_device);
unreg_chrdev:
   unregister_chrdev_region(device_num, 1);
out:
   pr_info("module insetion failed\n");
   return ret;
}

static void __exit basic_driver_exit(void)
{
}

module_init(basic_driver_init);
module_exit(basic_driver_exit);

MODULE_AUTHOR("viveksg");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("basic driver");