# See Rules.conf for configuration params

include Rules.conf

# global flags definitions
CFLAGS  += -Wall -O2 \
	   -D__KERNEL__ -DMODULE \
	   -I$(TOPDIR)/backend

#LDFLAGS += 

TOPDIR = $(shell /bin/pwd)

export CFLAGS TOPDIR # LDFLAGS

all: $(TARGET)

$(TARGET):
	make -C frontend $(FRONTEND)
	make -C backend $(BACKEND)
	$(LD) -r -o $(TARGET) frontend.o backend.o

re-$(TARGET): clean $(TARGET)

run: $(TARGET)
	insmod $(TARGET)
	if [ ! -d $(MNTPOINT) ]; then mkdir -p $(MNTPOINT); fi
	mount -t $(FSNAME) none $(MNTPOINT)

re-run: re-$(TARGET)
	umount $(MNTPOINT)
	rmmod $(FSNAME)
	make run

clean:
	make -C frontend clean
	make -C backend clean
	rm -f $(TARGET) frontend.o backend.o

