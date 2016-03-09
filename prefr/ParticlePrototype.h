/*
 * ParticlePrototype.h
 *
 *  Created on: 03/11/2014
 *      Author: sgalindo
 */

#ifndef PARTICLEPROTOTYPE_H_
#define PARTICLEPROTOTYPE_H_

#include <prefr/api.h>
#include "ElementCollection.hpp"
#include "InterpolationSet.hpp"


namespace prefr
{

  typedef utils::InterpolationSet<float> vectortfloat;
  typedef utils::InterpolationSet<glm::vec3> vectortvec3;
  typedef utils::InterpolationSet<glm::vec4> vectortvec4;

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

    PREFR_API ParticlePrototype(void);
    PREFR_API ParticlePrototype(float min, float max, const ParticleCollection& particlesArray);

    PREFR_API virtual ~ParticlePrototype();

    PREFR_API void SetLife(float min, float max);

  };

  typedef ParticlePrototype tprototype;
  typedef tprototype* tprototype_ptr;
  typedef std::vector<tprototype_ptr> PrototypesArray;


}

#endif /* PARTICLEPROTOTYPE_H_ */
