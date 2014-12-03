/*
 * ParticlePrototype.h
 *
 *  Created on: 03/11/2014
 *      Author: sgalindo
 */

#ifndef PARTICLEPROTOTYPE_H_
#define PARTICLEPROTOTYPE_H_

#include "ElementCollection.hpp"
#include "InterpolationSet.hpp"

using namespace utils;

namespace prefr
{

  typedef InterpolationSet<float> vectortfloat;
  typedef InterpolationSet<glm::vec3> vectortvec3;
  typedef InterpolationSet<glm::vec4> vectortvec4;

  class ParticlePrototype
  {
  public:
    float minLife;
    float maxLife;

    float lifeInterval;
    float lifeNormalization;
    float dispersion;

    vectortfloat size;

    glm::vec3 positionOffset;

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
