# Linux Character Device Driver

An out-of-tree Linux kernel module that registers a character device and a
procfs entry.

- **`/dev/DRIVER`**: reading it returns how many times it has been opened;
  writing to it logs the text to the kernel log.
- **`/proc/heavy_driver`**: a read-only status page (`seq_file`) showing the
  device path and open count.

It covers `alloc_chrdev_region`, `cdev`, `class_create`/`device_create`
(so udev creates the node), `copy_to_user`/`copy_from_user` with offset
handling, and cleanup in reverse order when setup fails partway.

## Layout

```
src/main.c   module init/exit, wires dev + proc together
src/dev.c    char device + file_operations
src/proc.c   /proc entry
src/Kbuild   builds heavy_module.ko from the three objects
include/     headers
```

## Build & use

Needs kernel headers for your running kernel (`linux-headers`).

```bash
make            # build -> modules/heavy_module.ko
make add        # build + insmod
sudo cat /dev/DRIVER
echo hello | sudo tee /dev/DRIVER
cat /proc/heavy_driver
make msg        # tail dmesg
make rm         # rmmod
make bear       # generate compile_commands.json for clangd
```
