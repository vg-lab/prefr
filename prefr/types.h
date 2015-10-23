/*
 * types.h
 *
 *  Created on: 24/11/2014
 *      Author: sgalindo
 */
#ifndef TYPES_H_
#define TYPES_H_

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
#include "Particle.h"

// type definitions
namespace prefr
{
  typedef prefr::Particle tparticle;
  typedef tparticle* tparticle_ptr;
  typedef std::vector<tparticle_ptr> tparticleContainer;
  typedef utils::ElementCollection<tparticle> ParticleCollection;
}

#endif /* TYPES_H_ */
