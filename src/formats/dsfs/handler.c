#include "formats/dsfs.h"

static off_t checkup(char const *disk)
{
    struct stat st;

    if (stat(disk, &st) == -1)
        pexit("stat image failed");
    if (st.st_size < DSFS_BLOCK_SIZE)
        pexit("Image must have a size minimal of %ld", 0);
    return (st.st_size);
}

void dsfs_handler(char const *disk)
{
}