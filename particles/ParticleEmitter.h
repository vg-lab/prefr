/*
 * ParticleEmitter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include <particles/config.h>
#include "InterpolationSet.hpp"
#include "ElementCollection.hpp"

using namespace utils;

namespace particles
{
  static glm::vec4 RGBToHSV(const glm::vec4 RGB)
  {
     float max;
     float min;
     float Chroma;
     glm::vec4 HSV;

    min = std::fmin(std::fmin(RGB.x, RGB.y), RGB.z);
    max = std::fmax(std::fmax(RGB.x, RGB.y), RGB.z);
    Chroma = max - min;

    //If Chroma is 0, then S is 0 by definition, and H is undefined but 0 by convention.
    if(Chroma != 0)
    {
      if(RGB.x == max)
      {
        HSV.x = (RGB.y - RGB.z) / Chroma;

        if(HSV.x < 0.0)
        {
          HSV.x += 6.0;
        }
      }
      else if(RGB.y == max)
      {
        HSV.x = ((RGB.z - RGB.x) / Chroma) + 2.0;
      }
      else //RGB.z == max
      {
        HSV.x = ((RGB.x - RGB.y) / Chroma) + 4.0;
      }

      HSV.x *= 60.0;
      HSV.y = Chroma / max;
    }

    HSV.z = max;
    HSV.a = RGB.a;

    return HSV;
  }


  static glm::vec4 HSVToRGB(const glm::vec4 HSV)
  {
     float Min;
     float Chroma;
     float Hdash;
     float X;
     glm::vec4 RGB;

    Chroma = HSV.y * HSV.z;
    Hdash = HSV.x / 60.0;
    X = Chroma * (1.0f - std::abs((std::fmod(Hdash,2.0f)) - 1.0f));

    if(Hdash < 1.0)
    {
      RGB.x = Chroma;
      RGB.g = X;
    }
    else if(Hdash < 2.0)
    {
      RGB.x = X;
      RGB.g = Chroma;
    }
    else if(Hdash < 3.0)
    {
      RGB.g = Chroma;
      RGB.z = X;
    }
    else if(Hdash < 4.0)
    {
      RGB.g= X;
      RGB.z = Chroma;
    }
    else if(Hdash < 5.0)
    {
      RGB.x = X;
      RGB.z = Chroma;
    }
    else if(Hdash <= 6.0)
    {
      RGB.x = Chroma;
      RGB.z = X;
    }

    Min = HSV.z - Chroma;

    RGB.x += Min;
    RGB.g += Min;
    RGB.z += Min;
    RGB.a = HSV.a;

    return RGB;
  }

  typedef InterpolationSet<float> vectortfloat;
  typedef InterpolationSet<vec3> vectortvec3;
  typedef InterpolationSet<vec4> vectortvec4;

  class ParticlePrototype
  {
  public:
    float minLife = 0;
    float maxLife;
    float dispersion;

    vectortfloat size;

    vec3 positionOffset;

    vectortfloat velocity;
    vectortvec4 color;

  };


  class ParticleEmitter
  {
  public:

    ParticleCollection* particles;

    ParticlePrototype* particleBase;

    int maxParticles;
    int particlesPerCycle;
    float emissionRate;
    bool loop;

    ParticleEmitter(ParticleCollection* particlesArray, ParticlePrototype* particlePrototype, float _emissionRate, bool _loop)
    : particles(particlesArray)
    , particleBase(particlePrototype)
    , particlesPerCycle(0)
    , emissionRate(_emissionRate)
    , loop (_loop)
    {
      maxParticles = particlesArray->size;
    }

    virtual ~ParticleEmitter()
    {
      delete( particles );
      delete( particleBase );

    }



    virtual void StartEmission(float deltaTime){ particlesPerCycle = emissionRate * maxParticles;}
    virtual int EmitSingle(unsigned int i) = 0;

    virtual void EmitAll(float deltaTime) = 0;

    virtual void EmitFunction(unsigned int i, bool override = false) = 0;

  };


}



#endif /* PARTICLEEMITTER_H_ */
