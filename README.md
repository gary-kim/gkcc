# GKCC - C Compiler for ECE-466 at The Cooper Union

**Note: In order to keep up with a busy school schedule and still make good
progress on this compiler, I have neglected a lot of good practice. There are a
ton of  bodge fixes and terrible good commit messages. Do as I say and not as I
do :).**

This is the C compiler that I am writing for the ECE-466: Compilers
course at The Cooper Union.

The [course page](http://faculty.cooper.edu/hak/ece466) lists the
assignments and general instructions for the compiler.

### Building


#### Dependencies

The following can be used to install all dependencies on Fedora based distros:

```
sudo dnf install -y bison bison-devel bison-runtime flex cmake gcc g++ libasan libubsan libubsan-static libxslt
```

#### Building

```
mkdir tmp
cd tmp
cmake ..
make -j $(nproc)
```

### License

Copyright (c) 2023 Gary Kim <<gary@garykim.dev>>;

Licensed under [AGPL-3.0-or-later](./LICENSE)

