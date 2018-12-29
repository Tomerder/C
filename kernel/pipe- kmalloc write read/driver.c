#include <linux/module.h>
#include <linux/fs.h>		/* for file_operations */
#include <linux/version.h>	/* versioning */
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>	/* for copy_from_user , copy_to_user */

#define MODULE_NAME "pipeT: "

#define SIZE 100

/*-----------------------------------------------------------------------*/

void* myBuf;

int readFlag = 1;
int globSize;

/*-----------------------------------------------------------------------*/

/* return number of bytes done , negative value for error  */
int driver_open(struct inode *inode, struct file *filp)
{
	printk(MODULE_NAME KERN_INFO "open\n");
	
	printk("open major =%d minor %d\n", imajor(inode),  iminor(inode));

	return 0;
}

/* return number of bytes done , negative value for error  */
static ssize_t driver_write(struct file *filp,const char __user *umem, size_t size, loff_t *off)
{
	printk(MODULE_NAME KERN_INFO "write\n");
	
	if(myBuf == NULL){
		printk(MODULE_NAME "myBuf == NULL" );
		return -1;
	}
	
	if(copy_from_user((void *)myBuf , umem ,  size )){
		printk(MODULE_NAME "ioctl: copy from user failed" );
	}

	readFlag = 1;
	globSize = size;	

	return size;
}

static ssize_t driver_read(struct file *filp, char __user *umem, size_t size, loff_t *off)
{
	printk(MODULE_NAME KERN_INFO "read \n");
	
	if(!readFlag){
		return 0;
	}
	
	if(copy_to_user((void *)umem , myBuf ,  size )){
		printk(MODULE_NAME "ioctl: copy from user failed" );
	}
	
	readFlag = 0;

	return globSize;
}

int driver_close(struct inode *inode, struct file *filp)
{
	printk(MODULE_NAME KERN_INFO "close \n");
	
	return 0;
}

struct file_operations driver_ops = {
	open: driver_open,
	write: driver_write,
	read:  driver_read,
	release: driver_close
};

int driver_major ;

static void driver_cleanup(void) 
{
	printk(MODULE_NAME KERN_INFO "driver exit\n");
	
	kfree(myBuf);
	
	unregister_chrdev(driver_major, "pipeT");
	
}

static int driver_init(void)
{
	printk(MODULE_NAME KERN_INFO "driver init\n");

	driver_major = register_chrdev(0, "pipeT",&driver_ops);
	if (driver_major<0){
		printk(MODULE_NAME KERN_INFO "failed to register char driver\n");
		return -1;
	}
	
	myBuf = kmalloc(SIZE, GFP_KERNEL);
	if(myBuf == NULL){
		printk(MODULE_NAME "kmalloc failed" );
	}

	memset(myBuf, 0 , SIZE);
	
	return 0;
}

module_init(driver_init);
module_exit(driver_cleanup);

MODULE_DESCRIPTION("pipeT");
MODULE_AUTHOR("Raz Ben Jehuda");
MODULE_LICENSE("GPL");
