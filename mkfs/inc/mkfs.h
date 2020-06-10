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

void ext2_handler(char const *disk);
void dsfs_handler(char const *disk);

#define ERROR_MSG_LSEEK "lseek failed"
#define ERROR_MSG_WRITE "write failed"
#define ERROR_MSG_ALLOC "out of memory"

#define safe_lseekset(x, y)                 \
{                                           \
        if (lseek(x, y , SEEK_SET) == - 1)  \
            pexit(ERROR_MSG_LSEEK);         \
}

#define safe_write(x, y, z)         \
{                                   \
        if (write(x, y , z) == - 1) \
            pexit(ERROR_MSG_WRITE); \
}

static inline void *safe_calloc(size_t x)
{
    void *ptr = calloc(x, 1);

    if (!ptr)
        pexit(ERROR_MSG_ALLOC);
    return (ptr);
}

#endif