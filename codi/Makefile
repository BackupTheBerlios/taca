all: 
	gcc -Wall -O2 -D__KERNEL__ -DMODULE -I/lib/modules/`uname -r`/build/include -c rkfs.c mosix.c
	ld -r -o taca.o rkfs.o mosix.o

clean:
	rm -rf *.o
