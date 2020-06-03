#ifndef __DSFS_FS_H
#define __DSFS_FS_H

#include "mkfs.h"
#include <stdint.h>

#define DSFS_MAGIC      0xD5F5

#define DSFS_BLOCK_SIZE 1024

#define DSFS_NAME_LEN   255

#define DSFS_GOOD_OLD_REV        0    /* The good old (original) format */
#define DSFS_DYNAMIC_REV         1    /* V2 format w/ dynamic inode sizes */
#define DSFS_GOOD_OLD_INODE_SIZE 128

#define DSFS_BAD_INO         1  /* Bad blocks inode */
#define DSFS_ROOT_INO        2  /* Root inode */
#define DSFS_BOOT_LOADER_INO 5  /* Boot loader inode */
#define DSFS_UNDEL_DIR_INO   6  /* Undelete directory inode */
#define EXT3_RESIZE_INO      7  /* Reserved group descriptors inode */
#define EXT3_JOURNAL_INO     8  /* Journal inode */

/*
** File types and file modes
*/
#define FS_IFDIR     0040000    /* Directory */
#define FS_IFCHR     0020000    /* Character device */
#define FS_IFBLK     0060000    /* Block device */
#define FS_IFREG     0100000    /* Regular file */
#define FS_IFIFO     0010000    /* FIFO */
#define FS_IFLNK     0120000    /* Symbolic link */
#define FS_IFSOCK    0140000    /* Socket */

#define FS_IFSHIFT   12

#define T_IFDIR   (FS_IFDIR  >> FS_IFSHIFT)
#define T_IFCHR   (FS_IFCHR  >> FS_IFSHIFT)
#define T_IFBLK   (FS_IFBLK  >> FS_IFSHIFT)
#define T_IFREG   (FS_IFREG  >> FS_IFSHIFT)
#define T_IFIFO   (FS_IFIFO  >> FS_IFSHIFT)
#define T_IFLNK   (FS_IFLNK  >> FS_IFSHIFT)
#define T_IFSOCK  (FS_IFSOCK >> FS_IFSHIFT)

/*
** super block
*/
struct super_block
{
    uint32_t inodes_count;            /* Inodes count */
    uint32_t blocks_count;            /* Blocks count */
    uint32_t r_blocks_count;          /* Reserved blocks count */
    uint32_t free_blocks_count;       /* Free blocks count */
    uint32_t free_inodes_count;       /* Free inodes count */
    uint32_t first_data_block;        /* First Data Block */
    uint32_t log_block_size;          /* Block size */
    uint32_t log_frag_size;           /* Fragment size */
    uint32_t blocks_per_group;        /* # Blocks per group */
    uint32_t frags_per_group;         /* # Fragments per group */
    uint32_t inodes_per_group;        /* # Inodes per group */
    uint32_t mtime;                   /* Mount time */
    uint32_t wtime;                   /* Write time */
    uint16_t mnt_count;               /* Mount count */
    int16_t  max_mnt_count;           /* Maximal mount count */
    uint16_t magic;                   /* Magic signature */
    uint16_t state;                   /* File system state */
    uint16_t errors;                  /* Behaviour when detecting errors */
    uint16_t minor_rev_level;
    uint32_t lastcheck;               /* time of last check */
    uint32_t checkinterval;           /* max. time between checks */
    uint32_t creator_os;              /* OS */
    uint32_t rev_level;               /* Revision level */
    uint16_t def_resuid;              /* Default uid for reserved blocks */
    uint16_t def_resgid;              /* Default gid for reserved blocks */

    uint32_t first_ino;               /* First non-reserved inode */
    uint16_t inode_size;              /* size of inode structure */
    uint16_t block_group_nr;          /* block group # of this superblock */
    uint32_t feature_compat;          /* compatible feature set */
    uint32_t feature_incompat;        /* incompatible feature set */
    uint32_t feature_ro_compat;       /* readonly-compatible feature set */
    uint8_t  uuid[16];                /* 128-bit uuid for volume */
    char     volume_name[16];         /* volume name */
    char     last_mounted[64];        /* directory where last mounted */
    uint32_t algorithm_usage_bitmap;  /* For compression */
    uint8_t  prealloc_blocks;         /* Nr of blocks to try to preallocate*/
};

typedef struct super_block dsfs_superblock_t;

/*
**  dsfs group desc structure
*/
struct dsfs_group_desc
{
    uint32_t bg_block_bitmap;       /* Blocks bitmap block */
    uint32_t bg_inode_bitmap;       /* Inodes bitmap block */
    uint32_t bg_inode_table;        /* Inodes table block */
    uint16_t bg_free_blocks_count;  /* Free blocks count */
    uint16_t bg_free_inodes_count;  /* Free inodes count */
    uint16_t bg_used_dirs_count;    /* Directories count */
    uint16_t bg_pad;
    uint32_t bg_reserved[3];
};

static_assert(((sizeof(struct dsfs_group_desc) % 8) == 0));

/*
** dsfs inode structure
*/
struct inode
{
    uint16_t mode;          /* File mode */
    uint16_t uid;           /* Owner Uid */
    uint32_t size;          /* Size in bytes */
    uint32_t atime;         /* Access time */
    uint32_t ctime;         /* Creation time */
    uint32_t mtime;         /* Modification time */
    uint32_t dtime;         /* Deletion Time */
    uint16_t gid;           /* Group Id */
    uint16_t links_count;   /* Links count */
    uint32_t block_count;   /* Blocks count */
    uint32_t flags;         /* File flags */
    uint32_t _reserved1;
    uint32_t blocks[0x80];  /* Pointers to blocks */
    uint32_t version;       /* File version (for NFS) */
    uint32_t file_acl;      /* File ACL */
    uint32_t dir_acl;       /* Directory ACL */
    uint32_t _reserved2;
};

static_assert(((sizeof(struct inode) % 8) == 0));

typedef struct inode dsfs_inode_t;

struct dsfs_directory
{
    unsigned int    inode;        /* Inode number */
    unsigned short  rec_len;      /* Directory entry length */
    unsigned char   name_len;     /* Name length */
    unsigned char   file_type;
    char     name[DSFS_NAME_LEN]; /* File name */
};

/*
** A DSFS datablock
*/
struct dsfs_datablock_t
{
    uint8   data[DSFS_BLOCK_SIZE - sizeof(uint64)];
    uint64  next;
};

static_assert(sizeof(struct dsfs_datablock_t) == DSFS_BLOCK_SIZE);

#endif /* dsfs_fs.h */