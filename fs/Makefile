MODULE=DeadStarFS

PWD=$(shell pwd)
IDENTITY=$(shell uname -r)
KERNEL_BUILD=/lib/modules/$(IDENTITY)/build

.PHONY: all clean

obj-m 		+= 	dsfs.o

dsfs-srcs 	+= $(addprefix kmod/,	\
						   entry.c	\
						   core.c	\
				)

dsfs-objs := ${dsfs-srcs:.c=.o}

all: dsfs

clean: dsfs-clean

dsfs:
	make -C $(KERNEL_BUILD) M=$(PWD) modules

dsfs-clean:
	make -C $(KERNEL_BUILD) M=$(PWD) clean