<p align="center">
<img src=".github/logo.png" height=120>
<h1 align=center>DragonHeartOS</h1>
<p align=center>Blazingly fast desktop general purpose operating system</p>
</p>

## What's this?

The operating system is made to give you as much control over your hardware from the display, audio or even serial. Currently, the operating system is in a very primitive state.

## Building

First, you need to compile the toolchain:

```
$ cd toolchain
$ ./install.sh
```

Then, you can use CMake to build and run the operating system:

```
$ mkdir -p build && cd build 
$ cmake -GNinja ..
$ ninja && ninja image && ninja run
```

## License

This software is distributed under the MIT license. Find more information [here](LICENSE).
