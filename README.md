# PReFr - Particle Rendering Framework
(c) 2015. GMRV / URJC

www.gmrv.es
gmrv@gmrv.es

## Introduction

TODO

## Dependencies

OpenGL, GLEW, GLUT, GLM, Boost (Review)

## Building

PReFr has been succesfully built and used on Ubuntu 14.04/16.04, Mac OSX
Yosemite and Windows 7/8 (Visual Studio 2013 Win64). The following steps
should be enough to build it:

```bash
git clone https://gitlab.gmrv.es/nsviz/prefr.git PReFr
mkdir PReFr/build && cd PReFr/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```
