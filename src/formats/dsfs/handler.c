#include "formats/dsfs.h"

#define DSFS_MINIMAL_DISK_SIZE 0x4000

struct mkfs_t
{
    int   fd;
    off_t size;
};

static off_t checkup(char const *disk)
{
    struct stat st;

    if (stat(disk, &st) == -1)
        pexit("stat image failed");
    if (st.st_size < DSFS_MINIMAL_DISK_SIZE)
        pexit("Image must have a size minimal of %ld", DSFS_MINIMAL_DISK_SIZE);
    return (st.st_size);
}

struct super_block superblk =
{
    .inodes_count = 0x1,
    .blocks_count = 0x1,
    .first_data_block = DSFS_FIRST_DATABLK_OFF,
    .first_inode = DSFS_FIRST_INODE_OFF,
    .block_size = DSFS_BLOCK_SIZE,
    .mtime = 0x0,
    .wtime = 0x0,
    .magic = DSFS_MAGIC,
    .state = DSFS_STATE_NORMAL,
    .errors = 0x0,
    .minor_rev_level = DSFS_GOOD_OLD_REV,
    .lastcheck = 0x0,
    .checkinterval = 0x0,
    .creator_os = 0x0,
    .rev_level = DSFS_GOOD_OLD_REV,
    .def_resuid = 0x0,
    .def_resgid = 0x0
};

static void dsfs_mk_superblock(struct mkfs_t *mkfs)
{
    safe_lseekset(mkfs->fd, DSFS_SUPERBLOCK_OFF);
    write(mkfs->fd, &superblk, sizeof(struct super_block));
}

void dsfs_handler(char const *disk)
{
    struct mkfs_t *mkfs = safe_calloc(sizeof(struct mkfs_t));

    mkfs->fd = open(disk, O_WRONLY);
    if (!mkfs->fd)
        pexit("Can't open disk img");
    mkfs->size = checkup(disk);
    dsfs_mk_superblock(mkfs);
}