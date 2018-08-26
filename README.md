shmcat
======

Print SysV IPCS information on the standard output

Build
-----

Clone this repository.

`git clone https://github.com/gustavorv86/shmcat`

Go to repository folder and run make command.

```
cd shmcat
make
```

Usage
-----

Type `ipcs -m` to view all shared memories.

Type `./dist/shmcat <shmid> [length]` to view the shared memory content.
