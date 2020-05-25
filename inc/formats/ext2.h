#ifndef __EXT2_H
#define __EXT2_H

#include "mkfs.h"

#define MKFS_START_OFFSET   1024
#define MKFS_BLOCK_SIZE     1024
#define MKFS_BLOCK_SIZE_LOG 0

/* To compute the size of the inodes table */
#define MKFS_INODES_PER_TABLE_BLOCK (MKFS_BLOCK_SIZE / sizeof(struct ftfs_inode))

/* To compute the size of the block group descriptor table */
#define MKFS_GROUP_DESC_PER_TABLE_BLOCK (MKFS_BLOCK_SIZE / sizeof(struct ftfs_block_group))

#define MKFS_BLOCKS_PER_GROUP     (MKFS_BLOCK_SIZE * 8) /* max of block bitmap block */
#define MKFS_INODES_TABLE_BLOCKS  (MKFS_BLOCKS_PER_GROUP / 16) /* let's use 1/16 of the blocks for the inode table */
#define MKFS_INODES_PER_GROUP     (MKFS_INODES_TABLE_BLOCKS * MKFS_INODES_PER_TABLE_BLOCK)
#define MKFS_GROUP_SIZE           (MKFS_BLOCKS_PER_GROUP * MKFS_BLOCK_SIZE)

/* The smallest group we support : it has only 1 data block */
#define MKFS_SMALLEST_GROUP_SIZE    ( MKFS_BLOCK_SIZE /* block bitmap */ \
                    + MKFS_BLOCK_SIZE /* inode bitmap */ \
                    + MKFS_INODES_TABLE_BLOCKS * MKFS_BLOCK_SIZE /* inode table */ \
                    + MKFS_BLOCK_SIZE /* only 1 data block */ \
                    )

/* Number of blocks in a group that are not data */
#define MKFS_GROUP_OVERHEAD_BLOCKS  ( 1 /* block bitmap */  \
                    + 1 /* inode bitmap */  \
                    + MKFS_INODES_TABLE_BLOCKS /* inode table */    \
                    )

/* The number of data blocks in a simple empty group.
 * This value is not accurate for the first group, where you also need to remove the blocks for the sb and bgd */
#define MKFS_FREE_BLOCKS_PER_GROUP  (MKFS_BLOCKS_PER_GROUP - MKFS_GROUP_OVERHEAD_BLOCKS)


/* The minimum size of the image we have to format */
#define MKFS_MIN_IMAGE_SIZE                     \
    ( MKFS_START_OFFSET /* Boot record */           \
    + MKFS_BLOCK_SIZE   /* Superblock */            \
    + MKFS_BLOCK_SIZE   /* Block group descriptor table */  \
    + MKFS_SMALLEST_GROUP_SIZE                  \
    )

/* Offset to the block group descriptors table from the start of the file */
#define MKFS_GROUP_TABLE_OFFSET                 \
    ( MKFS_START_OFFSET /* Boot record */           \
    + MKFS_BLOCK_SIZE /* Superblock */          \
    )

void ext2_handler(char const *disk);

#endif