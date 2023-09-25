# PReFr - Particle Rendering Framework
(c) 2015-2020. VG-Lab / URJC

www.vg-lab.es
vg-lab@vg-lab.es

## Introduction

PReFr (Particle Rendering Framework) is a modular and highly configurable 
framework providing the functionality for creating, managing and rendering
particles within several environments. PReFr allows to deploy particle systems with low effort by simply using the provided modules.

There are basic implementations for all involved modules as well as an 
OpenGL renderer that can be easily reimplemented for
different behaviors. A complete multi-core implementation based on OpenMP is also provided for improving performance.


## Dependencies

### Strong dependences:

* OpenGL
* GLEW
* GLM
* Boost
* ReTo (*)

(*) Note: These dependencies will be automatically downloaded and compiled with
the project.

### Weak dependences

* OpenMP: multi-core functioning. 
* GLUT: OpenGL example.
* Eigen3: full ReTo compatibility
* CUDA/CUDA Thrust!: WIP.

## Building

PReFr has been succesfully built with GCC 6 and used on Ubuntu 14.04/16.04, Mac OSX
Yosemite and Windows 7/8 (Visual Studio 2013 Win64). The following steps
should be enough to build it:

```bash
git clone https://gitlab.vg-lab.es/nsviz/prefr.git PReFr
mkdir PReFr/build && cd PReFr/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DPREFR_WITH_EXAMPLES=ON
make
```