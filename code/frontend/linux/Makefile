
CFLAGS += $(LINUX_CFLAGS)

TARGET = linux.o
OBJS   = linux_common.o root.o

all: $(TARGET)
	cp $(TARGET) $(TOPDIR)/frontend.o

#$(TARGET): $(OBJS)
#	ld -r -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)

