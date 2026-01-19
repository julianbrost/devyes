/dev/yes
========

This repository provides the source code for a Linux kernel module that
provides the character device `/dev/yes`, implementing the same functionality
as [`yes(1)`](https://man.archlinux.org/man/yes.1), so reading from that device
returns an endless stream of lines containing just `y`. In contrast to
`yes(1)`, the text cannot be customized.

But why?
--------

It's up to the reader to decide if this is useful. The practical uses for
`/dev/yes` are probably limited, but it might serve as an example how to
implement a character device in a Linux kernel module.

Installation
------------

```console
$ git clone https://github.com/julianbrost/devyes.git
$ cd devyes
$ make
# insmod devyes.ko
# chmod 444 /dev/yes
```

The `Makefile` assumes that the kernel header for building modules for the
currently running kernel are located in `/lib/modules/$(uname -r)/build`. If
they are located elsewhere, or you want to build the module for another kernel
version, a different path can be passed like this:

```console
$ make KDIR=/path/to/modules/6.18.5/build
```

Now, `/dev/yes` is ready to use. Instead of `yes | example_command`, simply do
`example_command < /dev/yes`.
