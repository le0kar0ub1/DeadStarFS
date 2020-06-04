# DeadStarFS Documentation

DeadStar is a hand-made linux Virtual File System (VFS) compliant file system.

The linux VFS is an interface that allow the coexistence of multiple FS type.

![vfs](vfs.png)

DeadStar's model was inspired by the extends ones.

This is, by design, a non-scalable and light weight FS.

## Minimal Data model

| Offset      | size       | Field               | Description              |
|-------------|------------|---------------------|--------------------------|
| 0           | 1024       | Boot Record         | Boot Record              |
| 1024        | 1024       | Super Block         | Super Block              |
| 2048        | 4096       | Root Inode          | the '/'                  |
| ...         | ...        | Data Block          | A data block             |

The boot record is fully empty and unused.

The Super Block is described below.

The idea is here:
  - The model is linked chain based.
  - A list of inodes describe ONE directory and his content.
  - A list of data block describe whatever but only one context.

### Data Block

total size: 4096

| Offset |  size   | field   | Description                                            |
|--------|---------|---------|--------------------------------------------------------|
| 0      | 4088    | Data    | A part of the data                                     |
| 4088   | 8       | Next    | The offset of next data block describing the same data |

```c
struct dsfs_superblock
{
};
```