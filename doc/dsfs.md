# DeadStarFS Documentation

DeadStar is a hand-made linux Virtual File System (VFS) compliant file system.

The linux VFS is an interface that allow the coexistence of multiple FS type.

![vfs](vfs.png)

DeadStar's model was inspired by the extends ones.

This is, by design, a non-scalable and light weight FS.

## Data model

| Offset      | size       | Field               | Description              |
|-------------|------------|---------------------|--------------------------|
| 0           | 1024       | Boot Record         | Boot Record              |
| 1024        | 1024       | Super Block         | Super Block              |
| 2048        | 

The boot record is fully empty and unused.

The Super Block is described below.

```c
struct dsfs_superblock
{
};
```