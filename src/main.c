#include "mkfs.h"

static void __noreturn usage(void)
{
    printf("./mkfs --format=FORMAT disk.img\n");
    exit(0);
}

struct formats_t
{
    char const *format;
    void (*handler)(char const *);
};

static const struct formats_t formats[] =
{
    {"ext2", ext2_handler},
    {NULL, NULL}
};

#define FORMATOPT "--format="

int main(int ac, char **av)
{
    char const *disk = NULL;
    int handleridx = -1;

    if ((ac == 2 && (!strcmp(av[1], "--help") || !strcmp(av[1], "-h"))) || ac != 3)
        usage();
    for (u32_t i = 0; av[i]; i++)
    {
        if (strlen(av[i]) > strlen(FORMATOPT) + 2 &&
        !strncmp(av[i], FORMATOPT, strlen(FORMATOPT))) {
            for (u32_t j = 0x0; formats[j].format; j++)
            {
                if (!strcmp(ADD_TO_PTR(av[i], strlen(FORMATOPT)), formats[j].format))
                    handleridx = j;
            }
        } else {
            if (disk)
                pexit("Can format only one image by run");
            disk = av[i];
        }
    }
    if (disk == NULL || handleridx == -1)
        usage();
    formats[handleridx].handler(disk);
    return (0);
}