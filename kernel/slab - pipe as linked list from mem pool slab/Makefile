TARGET= slab
OBJS  = driver.o

CURRENT=`uname -r`
#KDIR = /lib/modules/$(CURRENT)/build/
KDIR = /home/student/lkc-code/qemu/linux-3.8/
PWD = $(shell pwd) 

obj-m      := $(TARGET).o
$(TARGET)-objs   := $(OBJS)

default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
	
$(TARGET).o: $(OBJS)
	$(LD) $(LD_RFLAG) -r -o $@ $(OBJS)
clean:
	-rm -f *.o .*.o.cmd .*.ko.cmd *.[ch]~ Modules.symvers *.order *.*.ko *.ko *.mod.c .*.c  Module.symvers
indent:
	indent -kr -i8 *.[ch]
