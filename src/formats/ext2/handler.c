#include "formats/ext2.h"

char zero_block[MKFS_BLOCK_SIZE] = {0};

struct ftfs_super_block ft_sb = {
    .inodes_count      = 0,
    .block_count       = 0,
    .free_blocks_count = 0,
    .free_inodes_count = 0,
    .log_block_size    = MKFS_BLOCK_SIZE_LOG,
    .blocks_per_group  = MKFS_BLOCKS_PER_GROUP,
    .inodes_per_group  = MKFS_INODES_PER_GROUP,
    .magic             = FORTYTWOFS_MAGIC,
};

struct ftfs_inode root_inode = {
    .mode        = S_IFDIR | S_IRWXU,
    .uid         = 0, /* superuser */
    .size        = MKFS_BLOCK_SIZE, /* take only 1 block */
    .atime       = 0,
    .ctime       = 0,
    .mtime       = 0,
    .gid         = 0, /* superuser */
    .nlinks      = 2, /* the '.' and '..' */
    .block_count = MKFS_BLOCK_SIZE / 512, /* because units of 512 -_- */
    .blocks      = { 0 }, /* this is dynamic */
};

struct mkfs_root_dir {
    struct ftfs_dir dot;
    char   dot_name[4];
    struct ftfs_dir dotdot;
    char   dotdot_name[4];
};

struct mkfs_root_dir root_dir = {
    .dot = {
        .inode      = 2,
        .len        = 12,
        .name_len   = 1,
    },
    .dot_name   = ".",
    .dotdot = {
        .inode      = 2,
        .len        = MKFS_BLOCK_SIZE - 12, /* take all the remaining space */
        .name_len   = 2,
    },
    .dotdot_name    = "..",
};

void ext2_handler(char const *disk __unused)
{
    
}