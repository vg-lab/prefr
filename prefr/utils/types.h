/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef __PREFR__TYPES__
#define __PREFR__TYPES__

// types.h should be the first file included to ensure
// GLEW is included before any other GL file
#ifndef PREFR_SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif

// CUDA include
#include <prefr/defines.h>
#if (PREFR_USE_CUDA)
  #include <cuda.h>
#endif

// std includes
#include <vector>
#include <algorithm>


// math for windows includes
#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// GLM includes
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

// PREFR include for type definitions
#include "ElementCollection.hpp"


#endif /* PREFR__TYPES__ */
