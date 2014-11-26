/*
 * types.h
 *
 *  Created on: 24/11/2014
 *      Author: sgalindo
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "Particle.h"
#include "ElementCollection.hpp"

#include <vector>


#define GLM_FORCE_RADIANS

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#endif

#include <algorithm>

#if (PREFR_WITH_CUDA == 1)
  #include <cuda.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace prefr
{
  typedef prefr::Particle tparticle;
  typedef tparticle* tparticle_ptr;
  typedef std::vector<tparticle_ptr> tparticleContainer;
  typedef utils::ElementCollection<tparticle> ParticleCollection;
}



#endif /* TYPES_H_ */
