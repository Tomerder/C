#include <linux/module.h>
#include <linux/fs.h>		/* for file_operations */
#include <linux/version.h>	/* versioning */
#include <linux/uaccess.h>	/* copy from & copy to */
#include <linux/mm.h>
#include <linux/pagemap.h>	/* for kmap */
#include <linux/kthread.h>	/* for kthread */
#include <linux/semaphore.h>
#include "../user-kernel.h"


#define MODULE_NAME "mem : "

struct semaphore sem;

/*global struct contains user pointer*/
struct memory_descriptor user_mem;

/*current task TCB*/
//void* current_mm = 0;
struct task_struct* user_task = 0;
void* user_mm;

/*user pointer*/
//void* uaddr;

/*---------------------------------------------------------------------*/

typedef struct Pages{
	struct page* m_page[2];
	int m_len;
	int m_offset;
}Pages;

Pages pages; 

/*---------------------------------------------------------------------*/

int driver_thread(void *arg)
{
	char* kaddr;
	int nr_pages;
	int offset;
	struct page *pages[2];
	
	/*wait for notification that pointer from user has received*/
	down(&sem);
	
	/* get page from user */ 
	nr_pages  = get_user_pages(user_task, 
			user_mm, 
			(unsigned long) user_mem.uaddr,
			2, /* nr pages */
			1, /* write */
			0, /* force */
			(struct page **)&pages,
			0);

	if (nr_pages <= 0 ){
		printk("bad address: "
			"%d %p\n", nr_pages,user_mem.uaddr);
		return 0;
	}
	
	/*get page of user pointer => mapped to kernel vir address*/
	kaddr =	kmap(pages[0]);
	
	/*get offset on this page : lower 12 bits of address */
	offset = (long )user_mem.uaddr &  ~PAGE_MASK;	
	
	/*write msg to user pointer => by writing to mapped kernel page + page offset */
	strcpy((kaddr + offset),"hello from kernel");
	
	/*release page for future use*/
	kunmap(pages[0]);

	return 0; 
}

/*---------------------------------------------------------------------*/

int driver_open(struct inode *inode, struct file *filp)
{
	/*void *t;*/

	printk(MODULE_NAME  "open \n");
	
	/*create thread*/
	/*t = kthread_create(driver_thread, NULL, "memtimer");
	if (t == ERR_PTR(-ENOMEM)){
		printk(MODULE_NAME  
			"failed to invoke thread\n");
		return -1;
	}*/
	
	/*memset(&user_mem, 0x00, sizeof(user_mem));*/
	
	/*start thread*/
	/*wake_up_process(t);*/
	
	return 0;
}

/*---------------------------------------------------------------------*/

long my_driver_ioctl(struct file *filp, unsigned int opt, unsigned long arg)
{
	switch(opt){
		case REGISTER_MEM:
		{
			printk(MODULE_NAME  "ioctl: register mem" );
		
			/*  copy_to_user accesss the user's memory when user's process in kernel context */
			if (!access_ok(VERIFY_READ,arg,sizeof(user_mem))) {
				printk(MODULE_NAME   
					"ilegal ioctl parms ");
				return -EFAULT;
			}
		
			/*get pointer from user*/
			if (copy_from_user(&user_mem,
				(long *)arg, sizeof(user_mem))){
				printk(MODULE_NAME  
					"ioctl: register mem" );
				return -1;
			}
		
			/*get TCB - of task that called ioctl*/
			user_task = current;
		
			/* memory layout of task that called ioctl */
			user_mm = current->mm;
		
			/*notify thread : can get user pointer*/
			up(&sem);
		
			printk(MODULE_NAME  "ioctl: register mem %p" " successfull\n", user_mem.uaddr);
			
			return 0;
			
		}

		break;
	
		default:
			printk(MODULE_NAME  "unknown command %d\n",opt);
		
	}

	printk(MODULE_NAME  "ioctl");
	
	return -1;
}

/*---------------------------------------------------------------------*/

/*get pointer and offset of txt in file that has been copyed to mem + msg len*/
ssize_t driver_write(struct file *filp, const char __user *umem, size_t size, loff_t *off)
{
	int nr_pages;
	
	printk(MODULE_NAME  "write\n");
	
	printk(MODULE_NAME  "user pointer : %u , size : %d\n" , (unsigned)umem, size);
	
	/*get TCB - of task that called ioctl*/
	user_task = current;

	/* memory layout of task that called ioctl */
	user_mm = current->mm;	
	
	/* get page from user mapped to kernel page num*/ 
	nr_pages  = get_user_pages(user_task, 
			user_mm, 
			(unsigned long) umem,
			2, /* nr pages */
			1, /* write */
			0, /* force */
			(struct page **) &(pages.m_page),
			0);
			
	if (nr_pages <= 0 ){
		printk("bad address: " "%d %p\n", nr_pages,user_mem.uaddr);
		return 0;
	}
				
	/*get offset on this page : lower 12 bits of address */
	pages.m_offset = (long )umem & ~PAGE_MASK;	
	
	pages.m_len = size;
	
	return size;
}

/*---------------------------------------------------------------------*/

ssize_t driver_read(struct file *filp, char __user *umem, size_t size, loff_t *off)
{
	char* kaddr;
	int len = pages.m_len;

	printk(MODULE_NAME  "read\n");
	
	if(len == 0){
		return 0;
	}
	
	/*get page of user pointer => mapped to kernel vir address*/
	kaddr =	kmap(pages.m_page[0]);
	
	printk(MODULE_NAME  "kernel adr : %u\n" , (unsigned)(kaddr + pages.m_offset));
	
	/*print msg from user pointer*/	
	memcpy(umem, (kaddr + pages.m_offset) , len);

	/*release page for future use*/
	kunmap(pages.m_page[0]);
		
	/*set len to 0 for next read*/
	pages.m_len = 0;
		
	return len;
}

/*---------------------------------------------------------------------*/

int driver_close(struct inode *inode, struct file *filp)
{
	printk(MODULE_NAME  "close\n");
	return 0;
}

/*---------------------------------------------------------------------*/

struct file_operations driver_ops = {
	open: driver_open,
	//unlocked_ioctl: my_driver_ioctl,
	write: driver_write,
	read:  driver_read,
	release: driver_close
};

static int driver_major = 0;

/*---------------------------------------------------------------------*/

static void driver_cleanup(void) 
{
	printk(MODULE_NAME  "driver exit\n");
	
	unregister_chrdev(driver_major, DRIVER);
}

/*---------------------------------------------------------------------*/

static int driver_init(void)
{
	printk(MODULE_NAME  "driver init\n");
	
	/*reg driver*/
	driver_major = register_chrdev(0, DRIVER, &driver_ops);
	if (driver_major<0){
		printk(MODULE_NAME  
			"failed to register char driver\n");
		return -1;
	}
	
	printk(MODULE_NAME "driver major : %d\n", driver_major);
	
	/*init sem*/
	sema_init(&sem,0);
	
	return 0;
}

/*---------------------------------------------------------------------*/

module_init(driver_init);
module_exit(driver_cleanup);

MODULE_DESCRIPTION("user memory access");
MODULE_AUTHOR("Raz Ben Jehuda");
MODULE_LICENSE("GPL");
