# MKFS

Linux MKFS re-implementation.

Is a part of `DeadStarFS` project.

## Dependencies

* make
* gcc

## Build

`make`

## Handled format

A list of format handled by this mkfs implementation.

| Format   |    State                     | Documentation                                                  |
|----------|------------------------------|----------------------------------------------------------------|
|    ext2  |    non-functionnal           | [Extend FileSystem 2](https://wiki.osdev.org/Ext2)             |
|    dsfs  |    non-functionnal           | [DeadStarFS](doc/dsfs.md)                                      |

## Use it

Firstly, create an empty image disk.

`dd if=/dev/zero of=disk.img bs=1024 count=4096`

then format it using our mkfs.

`./mkfs --format=ext2 disk.img`

Finaly mount the image wherever you want.

`mount disk.img myMountPoint`

## Epilogue

Feel free to fork, use, improve.
