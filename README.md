# MKFS

Linux MKFS re-implementation.

## Dependencies

* make
* gcc

## Build

`make`

## Handled format

A list of format handled by this mkfs implementation.

| Format   |
|----------|
|          |

## Use it

Firstly, create an empty image disk.

`dd if=/dev/zero of=disk.img bs=1024 count=4096`

then format it using our mkfs implementation.

`./mkfs --format=ext2 disk.img`

## Epilogue

Just for the fun.

Feel free to fork, use, improve.
