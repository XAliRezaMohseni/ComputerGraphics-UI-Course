# The Ping-Pong game

## Description
This is a simple single player ping-pong game written in pure `c` using opengl and freeglut libraries.
This game has 3 different difficulty levels: Easy, Medium and hard and the speed of the ball will increase after `n` times hitting the racket.

## Building
The building process is same for *BSDs, microsoft windows and GNU/linux. You just have to use [meson build system](https://mesonbuild.com/) and [ninja](https://ninja-build.org/) to make it.

### Requirements
You need to install [freeglut](https://freeglut.sourceforge.net/) library from your package manager or build it form the source code.
Also you have to install an up-to-date `c` compiler compatible with standard `c`.

For building on windows you have to use [msys2/MINGW](https://www.msys2.org/) and install these things:
```bash
$ pacman -S mingw-w64-x86_64-freeglut \
            mingw-w64-x86_64-ninja \
            mingw-w64-x86_64-meson \
            gcc
```

### Making the game
After installing the requirements you can run meson on the `EX02` directory like this:
```bash
$ meson setup build_dir
```
and then enter the `build_dir` directory and run this (depending on you choice, it could be [make](https://en.wikipedia.org/wiki/Make_(software)) or something else):
```bash
$ ninja
```
and then run then game!:
```
$ ./002 # or .\002.exe if you are using the microsoft windows
```
