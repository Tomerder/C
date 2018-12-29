#include <linux/module.h>
#include <linux/version.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fs.h>		/* for file_operations */
#include <linux/uaccess.h>	/* for copy_from_user , copy_to_user */

#define MODULE_NAME "myslab "

#define MSG_LEN 32

/*--------------------------------------------------------------*/

int nodeSeq = 0;

int readFlag = 1;
int globSize;

/*--------------------------------------------------------------*/

struct list_head obj_list;

struct my_obj{
	struct list_head node;
	int seq;
	int size;
	char msg[MSG_LEN];
};

static struct kmem_cache *myobj_cachep = NULL;

/*--------------------------------------------------------------*/

void obj_ctor(void *p)
{
}

/*--------------------------------------------------------------*/

int myobj_create_cache(void)
{
	myobj_cachep = kmem_cache_create("MY_OBJ",
		       		sizeof(struct my_obj),
		       		0, /* override hw aligment if needed */
				SLAB_HWCACHE_ALIGN,
				obj_ctor); /* constrcutor per object */
	if (!myobj_cachep) {
		printk("failed to createk my obj cache\n");
		return -1;
	}
	return (0);
}

/*--------------------------------------------------------------*/

void myobj_cache_destroy(void)
{
	if (myobj_cachep)
		kmem_cache_destroy(myobj_cachep);
	myobj_cachep = NULL;
}

/*--------------------------------------------------------------*/

void myobj_release(struct my_obj * myobj)
{
	kmem_cache_free(myobj_cachep, myobj);
}

/*--------------------------------------------------------------*/

struct my_obj *myobj_get(void)
{
	struct my_obj *myobj =
		(struct my_obj *) kmem_cache_alloc(myobj_cachep, GFP_KERNEL);
	if (!myobj)
		return NULL;
	memset(myobj, 0, sizeof(*myobj));
	return myobj;
}

/*--------------------------------------------------------------*/

static ssize_t driver_write(struct file *filp,const char __user *umem, size_t size, loff_t *off)
{
	printk(MODULE_NAME "write\n");
		
	/*allocate obj on mempool(slab)*/
	struct my_obj *obj = myobj_get();
	if (!obj) {
		printk("Failed to create myobj\n");
		return 0;
	}
	
	/*copy msg to object*/	
	memset(obj->msg, 0 , MSG_LEN);
	if(copy_from_user(obj->msg , umem ,  size )){
		printk(MODULE_NAME "copy from user failed\n" );
	}
		
	/*add obj to list*/
	list_add_tail(&obj->node,&obj_list);
	obj->seq = nodeSeq++;
	obj->size = size;
	
	/*add dummy obj to list*/
	struct my_obj *objDummy = myobj_get();
	objDummy->size = 0;
	list_add_tail(&objDummy->node,&obj_list);
	

	return size;
}

static ssize_t driver_read(struct file *filp, char __user *umem, size_t size, loff_t *off)
{
	struct list_head *temp, *temp2;	
	struct my_obj* obj;
	int msgSize;

	printk(MODULE_NAME "read \n");
	
	
	/*get object from list*/
	//struct my_obj  *obj = list_first_entry (&temp, struct my_obj, node);

	/*get object from list*/
	list_for_each_safe(temp,temp2,&obj_list){
		obj = list_entry(temp,struct my_obj,node);
		break;
	}
		
	printk(MODULE_NAME "my_obj adr : %u\n" , (unsigned)obj );
	
	if(obj == 0){
		printk(MODULE_NAME "nothing to read\n");
		return 0;
	}	
	
	printk(MODULE_NAME "my_obj size : %d\n" , obj->size );
	
	msgSize = obj->size;
	
	if(msgSize == 0){
		/*dummy msg!!!*/	
		printk(MODULE_NAME "dummy msg!!!\n");
	}
	else
	{	
		/*copy msg from object*/	
		if(copy_to_user((void *)umem , obj->msg ,  msgSize)){
			printk(MODULE_NAME "copy to user failed\n" );
		}
	}
	
	/*pop list and free object*/
	list_del( &obj->node);
	myobj_release(obj);	

	return msgSize;
}

/*--------------------------------------------------------------*/

int driver_open(struct inode *inode, struct file *filp)
{
	printk(MODULE_NAME "open\n");
	
	printk("open major =%d minor %d\n", imajor(inode),  iminor(inode));

	return 0;
}

int driver_close(struct inode *inode, struct file *filp)
{
	printk(MODULE_NAME "close \n");
	
	return 0;
}

/*--------------------------------------------------------------*/

struct file_operations driver_ops = {
	open: driver_open,
	write: driver_write,
	read:  driver_read,
	release: driver_close
};


int driver_major;

/*--------------------------------------------------------------*/

static int driver_init(void)
{
	//int i = 0;
	printk(MODULE_NAME "slab allocation\n");
	
	/*init list*/
	INIT_LIST_HEAD(&obj_list);
	
	/*init mem pool*/
	if (myobj_create_cache() ){
		printk("Failed to create slab\n");
		return -1;
	}
	
	/*allocate objects on memory pool and insert to list*/
	/*for (i = 0; i < 9;i++) {
		struct my_obj *obj= myobj_get();
		if (!obj) {
			//fail the entire process 
			printk("Failed to create myobj\n");
			return 0;
		}
		//add obj to list
		list_add_tail(&obj->node,&obj_list);
		obj->seq = i;
	}*/
	
	/*register driver => slab*/
	driver_major = register_chrdev(0, "slab",&driver_ops);
	if (driver_major<0){
		printk(MODULE_NAME "failed to register char driver\n");
		return -1;
	}
	
	printk("driver_major : %d\n", driver_major);
	
	return 0;
}

/*--------------------------------------------------------------*/

static void driver_cleanup(void) 
{
	struct list_head *temp, *temp2;	

	list_for_each_safe(temp,temp2,&obj_list){
		struct my_obj  *obj = list_entry(temp,struct my_obj,node);
		list_del(&obj->node);
		myobj_release(obj);	
	}
	
	myobj_cache_destroy();
	
	unregister_chrdev(driver_major, "slab");
	
	printk(MODULE_NAME "slab exit\n");
}

/*--------------------------------------------------------------*/

module_init(driver_init);
module_exit(driver_cleanup);

MODULE_DESCRIPTION("slab");
MODULE_AUTHOR("Raz Ben Jehuda");
MODULE_LICENSE("GPL");
