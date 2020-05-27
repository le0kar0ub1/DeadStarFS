#include "formats/ext2.h"

#define MKFS_START_OFFSET   1024 /* Boot record */
#define MKFS_BLOCK_SIZE     1024
#define MKFS_BLOCK_SIZE_LOG 0   /* 1024 << 0 = 1024 */

/* To compute the size of the inodes table */
#define MKFS_INODES_PER_TABLE_BLOCK (MKFS_BLOCK_SIZE / sizeof(struct ext2_inode))

/* To compute the size of the block group descriptor table */
#define MKFS_GROUP_DESC_PER_TABLE_BLOCK (MKFS_BLOCK_SIZE / sizeof(struct ext2_group_desc))

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
#define MKFS_EXT2_MINIMAL_SIZE                     \
    ( MKFS_START_OFFSET /* Boot record */           \
    + MKFS_BLOCK_SIZE   /* Superblock */            \
    + MKFS_BLOCK_SIZE   /* Block group descriptor table */  \
    + MKFS_SMALLEST_GROUP_SIZE                  \
    )

/* Offset to the block group descriptors table from the start of the file */
#define MKFS_GROUP_TABLE_OFFSET                 \
    ( MKFS_START_OFFSET /* Boot record */       \
    + MKFS_BLOCK_SIZE /* Superblock */          \
    )

/*
** Get the image size
*/
static uint checkup(char const *disk)
{
    struct stat st;

    if (!stat(disk, &st))
        pexit("stat image failed");
    if (st.st_size < MKFS_EXT2_MINIMAL_SIZE)
        pexit("Image must have a size minimal of %ld", MKFS_EXT2_MINIMAL_SIZE);
    return (st.st_size);
}

struct mkfsext2_t {
    int fd;
    uint imgsize;
    uint blk_grp_blknbr;
    uint blk_grp_table_blknbr;
} __packed;

/*
** Calculate the number of group we can create.
** Being obviously limited by the given disk image.
*/
static void get_total_group(struct mkfsext2_t *mkfs)
{
    size_t sz = mkfs->imgsize;

    mkfs->blk_grp_blknbr = 0x0;
    mkfs->blk_grp_table_blknbr = 0x1;
    while (sz > MKFS_GROUP_SIZE)
    {
        mkfs->blk_grp_blknbr++;
        sz -= MKFS_GROUP_SIZE;
        if (mkfs->blk_grp_blknbr % MKFS_GROUP_DESC_PER_TABLE_BLOCK == 0)
        {
            if (sz < MKFS_GROUP_SIZE) {
                break;
            } else {
                mkfs->blk_grp_table_blknbr++;
                sz -= MKFS_GROUP_SIZE;
            }
        }
    }
}

uint8 zeroblk[MKFS_BLOCK_SIZE] = {0};

static void fillzero_block(int fd, int blk)
{
    off_t pos = lseek(fd, 0, SEEK_CUR);

    if (!pos || !lseek(fd, blk * MKFS_BLOCK_SIZE, SEEK_SET))
        pexit("lseek failed");
    if (!write(fd, &zeroblk, MKFS_BLOCK_SIZE))
        pexit("write failed");
    if (!lseek(fd, pos, SEEK_SET))
        pexit("lseek failed");
}

static void bitmap_setxbit(int fd, int blknbr, int bits)
{
    uint8 wr = 0x0;
    off_t pos = lseek(fd, 0, SEEK_CUR);

    if (!pos)
        pexit("lseek failed");
    if (!lseek(fd, blknbr * MKFS_BLOCK_SIZE, SEEK_SET))
        pexit("lseek failed");
    while (bits > 0)
    {
        wr = (1 << (bits % 9)) - 1;
        if (write(fd, &wr, 1) != 1)
            pexit("write failed");
        bits -= 8;
    }
    if (!lseek(fd, pos, SEEK_SET))
        pexit("lseek failed");
}


/*
** Generate all the block groups we can
*/
static void create_block_groups(struct mkfsext2_t *mkfs, struct ext2_super_block *super)
{
    struct ext2_group_desc bgdesc = { 0 };
    uint bginc = 0x0;
    uint curblk = 0x0;

    if (lseek(mkfs->fd, MKFS_GROUP_TABLE_OFFSET, SEEK_SET) == -1)
        pexit("lseek failed");
    curblk = (MKFS_GROUP_TABLE_OFFSET / MKFS_BLOCK_SIZE) + mkfs->blk_grp_table_blknbr;
    while (bginc < mkfs->blk_grp_blknbr)
    {
        bgdesc.bg_block_bitmap = curblk++;
        bgdesc.bg_inode_bitmap = curblk++;
        bgdesc.bg_inode_table  = curblk++;

        bgdesc.bg_free_blocks_count = MKFS_FREE_BLOCKS_PER_GROUP;
        bgdesc.bg_free_inodes_count = MKFS_INODES_PER_GROUP;
        fillzero_block(mkfs->fd, bgdesc.bg_block_bitmap);
        fillzero_block(mkfs->fd, bgdesc.bg_inode_bitmap);

        if (bginc > 0) {
            bitmap_setxbit(mkfs->fd, bgdesc.bg_block_bitmap, MKFS_GROUP_OVERHEAD_BLOCKS);
        } else {

        }
        if (!write(mkfs->fd, &bgdesc, sizeof(struct ext2_group_desc)))
            pexit("write failed");
        super->s_inodes_count += MKFS_INODES_PER_GROUP;
        super->s_free_inodes_count += bgdesc.bg_free_inodes_count;
        super->s_blocks_count += MKFS_BLOCKS_PER_GROUP;
        super->s_free_blocks_count += bgdesc.bg_free_blocks_count;
        bginc++;
        curblk = bginc * MKFS_BLOCKS_PER_GROUP;
    }
}

/*
** ext2 hookpoint
*/
void ext2_handler(char const *disk)
{
    struct mkfsext2_t *mkfs = calloc(sizeof(struct mkfsext2_t), 1);
    struct ext2_super_block *super = calloc(sizeof(struct ext2_super_block), 1);

    if ((mkfs->fd = open(disk, O_WRONLY)) == -1)
        pexit("Invalid given image disk");
    mkfs->imgsize = checkup(disk);
    get_total_group(mkfs);
    printf("Creating %u block groups...\n", mkfs->blk_grp_blknbr);
    create_block_groups(mkfs, super);

}