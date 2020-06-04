#ifndef __DSFS_FS_H
#define __DSFS_FS_H

#include "mkfs.h"
#include <stdint.h>

#define DSFS_MAGIC      0xD5F5

#define DSFS_BLOCK_SIZE 1024

#define DSFS_NAME_LEN   255

#define DSFS_FIRST_INODE_OFF   2048
#define DSFS_FIRST_DATABLK_OFF 3072

#define DSFS_SUPERBLOCK_OFF 1024
#define DSFS_SUPERBLOCK_SZ  1024

#define DSFS_GOOD_OLD_REV        0    /* The good old (original) format */
#define DSFS_DYNAMIC_REV         1    /* V2 format w/ dynamic inode sizes */
#define DSFS_GOOD_OLD_INODE_SIZE 128

enum superblock_state
{
    DSFS_STATE_NORMAL    = 0x0,
    DSFS_STATE_CORRUPTED = 0x1,
};

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
    uint32_t first_data_block;        /* First Data Block */
    uint32_t first_inode;
    uint32_t block_size;              /* Block size */
    uint32_t mtime;                   /* Mount time */
    uint32_t wtime;                   /* Write time */
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
};

typedef struct super_block dsfs_superblock_t;

/*
** dsfs inode structure
*/

struct superinode
{
    uint16_t links_count;         /* Links count */
    uint32_t block_count;         /* Blocks count */
    uint32_t size;                /* Size in bytes */
    uint32_t dtime;               /* Deletion Time */
};

struct inode
{
    uint64_t prev;
    uint16_t mode;                /* File mode */
    uint32_t atime;               /* Access time */
    uint32_t ctime;               /* Creation time */
    uint32_t mtime;               /* Modification time */
    uint16_t uid;                 /* Owner Uid */
    uint16_t gid;                 /* Group Id */
    uint32_t flags;               /* File flags */
    uint64_t inode_id;            /* a uniqu inode id */
    char     name[DSFS_NAME_LEN]; /* File name */
    uint64_t next;
};

#define L sizeof(struct inode)

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
struct dsfs_datablock
{
    uint8_t   data[DSFS_BLOCK_SIZE - sizeof(uint64)];
    uint64_t  next;
};

static_assert(sizeof(struct dsfs_datablock) == DSFS_BLOCK_SIZE);

#endif /* dsfs_fs.h */