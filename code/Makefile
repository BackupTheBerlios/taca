# See Rules.conf for configuration params

include Rules.conf

# global flags definitions
CFLAGS  += -Wall -O2 \
	   $(DEBUG) \
	   -D__KERNEL__ -DMODULE \
	   -inostdinc \
	   -I$(TOPDIR)/backend

#LDFLAGS += 

TOPDIR = $(shell /bin/pwd)

export CFLAGS TOPDIR # LDFLAGS

all:
	make -C frontend $(FRONTEND)
	make -C backend $(BACKEND)
	$(LD) -r -o $(TARGET) frontend.o backend.o

re-all: clean all

run: $(TARGET)
	insmod $(TARGET)
	if [ ! -d $(MNTPOINT) ]; then mkdir -p $(MNTPOINT); fi
	mount -t $(FSNAME) none $(MNTPOINT)

re-run: $(TARGET)
	-umount $(MNTPOINT)
	-rmmod $(FSNAME)
	make run

tags:
	find `pwd` -type f -name *.[ch] > cscope.files
	cscope -q -b

clean:
	make -C frontend clean
	make -C backend clean
	rm -f $(TARGET) frontend.o backend.o

