#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xbe3c12ff, "__class_create" },
	{ 0x2cfdf4d6, "cdev_init" },
	{ 0x93a12ffd, "cdev_add" },
	{ 0xa791088b, "device_create" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x2c3241c5, "device_destroy" },
	{ 0xff64f19a, "cdev_del" },
	{ 0x1c43c64d, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xb3d1bec3, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "5F790A21C42CDF5FC97EF32");
