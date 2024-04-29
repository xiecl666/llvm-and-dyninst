obj-m = enpmu.o
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	# make -C /lib/modules/5.15.0-25-generic/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	# make -C /lib/modules/5.15.0-25-generic/build M=$(PWD) clean