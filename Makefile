mkfs:
	@make -C mkfs -s --no-print-directory

mkfs-clean:
	@make clean -C mkfs -s --no-print-directory

fs:
	@make -C fs -s --no-print-directory

fs-clean:
	@make clean -C fs -s --no-print-directory

all: fs all

clean: fs-clean mkfs-clean