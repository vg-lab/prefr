# PReFr - Particle Rendering Framework
(c) 2015-2016. GMRV / URJC

www.gmrv.es
gmrv@gmrv.es

## Introduction

PReFr (Particle Rendering Framework) is a modular and highly configurable 
framework providing the functionality for creating, managing and rendering
particles within several environments. PReFr allows to deploy particle systems with low effort by simply using the provided modules.

There are basic implementations for all involved modules as well as two 
renderers (OpenGL and OpenSceneGraph 3.2) that can be easily reimplemented for
different behaviors. A basic multi-core implementation based on OpenMP is also provided.


## Dependencies

### Strong dependences:
* OpenGL
* GLEW
* GLM
* Boost

### Weak dependences
* ReTo: external camera and shaders handling.
* OpenMP: multi-core functioning. 
* GLUT: OpenGL example.
* OpenSceneGraph (v3.2): OpenSceneGraph functionality.
* CUDA/CUDA Thrust!: WIP.

## Building

PReFr has been succesfully built and used on Ubuntu 14.04/16.04, Mac OSX
Yosemite and Windows 7/8 (Visual Studio 2013 Win64). The following steps
should be enough to build it:

```bash
git clone https://gitlab.gmrv.es/nsviz/prefr.git PReFr
mkdir PReFr/build && cd PReFr/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DPREFR_WITH_EXAMPLES=ON
make
```
