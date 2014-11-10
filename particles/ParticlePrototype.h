/*
 * ParticlePrototype.h
 *
 *  Created on: 03/11/2014
 *      Author: sgalindo
 */

#ifndef PARTICLEPROTOTYPE_H_
#define PARTICLEPROTOTYPE_H_

#include <particles/config.h>
#include "ElementCollection.hpp"
#include "InterpolationSet.hpp"

using namespace glm;
using namespace utils;

namespace particles
{

  typedef InterpolationSet<float> vectortfloat;
  typedef InterpolationSet<vec3> vectortvec3;
  typedef InterpolationSet<vec4> vectortvec4;

  class ParticlePrototype
  {
  public:
    float minLife = 0;
    float maxLife;

    float lifeInterval;
    float lifeNormalization;
    float dispersion;

    vectortfloat size;

    vec3 positionOffset;

    vectortfloat velocity;
    vectortvec4 color;

    ParticleCollection* particles;

    ParticlePrototype(void);
    ParticlePrototype(float min, float max);

    void SetLife(float min, float max);

  };

  typedef ParticlePrototype tprototype;
  typedef tprototype* tprototype_ptr;
  typedef vector<tprototype_ptr> PrototypesArray;


}

#endif /* PARTICLEPROTOTYPE_H_ */
