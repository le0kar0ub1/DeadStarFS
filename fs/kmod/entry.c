#include "dsfs.h"

MODULE_LICENSE("GPL3");
MODULE_AUTHOR("Leo Karoubi");
MODULE_DESCRIPTION("DeadStar fileSystem");

static int __init dsfs_module_init(void)
{
    int ret = 0x0;

    printk(KERN_DEBUG "DeadStar init\n");
    // ret = register_filesystem(&ft_type);
    return ret;
}

static void __exit dsfs_module_exit(void)
{
    printk(KERN_DEBUG "DeadStar exit\n");
    // unregister_filesystem(&ft_type);
    return;
}

module_init(dsfs_module_init);
module_exit(dsfs_module_exit);
