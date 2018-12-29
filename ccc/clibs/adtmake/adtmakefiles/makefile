SUBDIRS= ADT genericVector genericList genericStack genericHeap genericTree HashMap ALIB

all:
	for subdir in -C $(SUBDIRS); do (cd $$subdir; $(MAKE) exe );\
	done;

commitAll:
	for subdir in -C $(SUBDIRS); do (cd $$subdir; $(MAKE) commit );\
	done;

cleanAll:
	for subdir in -C $(SUBDIRS); do (cd $$subdir; $(MAKE) clean );\
	done;
