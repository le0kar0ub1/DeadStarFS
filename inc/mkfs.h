#ifndef __MKFS__
#define __MKFS__

#include "def/keyword.h"
#include "def/macro.h"
#include "def/operator.h"
#include "def/typedef.h"
#include "def/bitfield.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FS_MAGIC 0xDFF5

void ext2_handler(char const *disk);

#endif