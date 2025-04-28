# ComputerGraphics-UI-Course

## Introduction

This repo is for storing my home works of the CG development course by Professor Mahvash.
This is not a professional work or project.

## Compiling 

### Requirements 

The sources are written in `C` and they do not have any extra dependencies on other libraries 
but [GCC](https://gcc.gnu.org/), [freeglut](https://github.com/freeglut/freeglut) and
[GLEW](https://glew.sourceforge.net/). Also, the [GNU Make](https://www.gnu.org/software/make/) is used for building the
sources (also, you can build the sources without that).

### Building

You can install these packages in [Debian GNU/Linux](https://www.debian.org/) by entering the command blow in command
line:

```bash
# apt install freeglut3-dev libglew-dev gcc make build-essential
```

then you can navigate to any folder and simply run:

```bash
$ make
```

and then run the application.
