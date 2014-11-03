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
#include "ParticlePrototype.h"

using namespace utils;

namespace particles
{
//  static glm::vec4 RGBToHSV(glm::vec4 RGB)
//{
//   float max;
//   float min;
//   float Chroma;
//   glm::vec4 HSV;
//
//   RGB.x /= 255.0f;
//   RGB.y /= 255.0f;
//   RGB.z /= 255.0f;
//
//    min = std::fmin(std::fmin(RGB.x, RGB.y), RGB.z);
//    max = std::fmax(std::fmax(RGB.x, RGB.y), RGB.z);
//    Chroma = max - min;
//
//    //If Chroma is 0, then S is 0 by definition, and H is undefined but 0 by convention.
//    if(Chroma != 0)
//    {
//      if(RGB.x == max)
//      {
//        HSV.x = (RGB.y - RGB.z) / Chroma;
//
//        if(HSV.x < 0.0)
//        {
//          HSV.x += 6.0;
//        }
//      }
//      else if(RGB.y == max)
//      {
//        HSV.x = ((RGB.z - RGB.x) / Chroma) + 2.0;
//      }
//      else //RGB.z == max
//      {
//        HSV.x = ((RGB.x - RGB.y) / Chroma) + 4.0;
//      }
//
//      HSV.x *= 60.0;
//      HSV.y = Chroma / max;
//    }
//
//    HSV.z = max;
//    HSV.a = RGB.a;
//
//    return HSV;
//  }
//
//
//  static glm::vec4 HSVToRGB(glm::vec4 HSV)
//  {
//     float Min;
//     float Chroma;
//     float Hdash;
//     float X;
//     glm::vec4 RGB(0,0,0,0);
//
//    Chroma = HSV.y * HSV.z;
//    Hdash = HSV.x / 60.0;
//    X = Chroma * (1.0f - std::fabs((std::fmod(Hdash,2.0f)) - 1.0f));
//
//    if(Hdash < 1.0)
//    {
//      RGB.x = Chroma;
//      RGB.g = X;
//    }
//    else if(Hdash < 2.0)
//    {
//      RGB.x = X;
//      RGB.g = Chroma;
//    }
//    else if(Hdash < 3.0)
//    {
//      RGB.g = Chroma;
//      RGB.z = X;
//    }
//    else if(Hdash < 4.0)
//    {
//      RGB.g= X;
//      RGB.z = Chroma;
//    }
//    else if(Hdash < 5.0)
//    {
//      RGB.x = X;
//      RGB.z = Chroma;
//    }
//    else if(Hdash <= 6.0)
//    {
//      RGB.x = Chroma;
//      RGB.z = X;
//    }
//
//    Min = HSV.z - Chroma;
//
//    RGB.x += Min;
//    RGB.g += Min;
//    RGB.z += Min;
//    RGB.a = HSV.a;
//
////    float c = HSV.y * HSV.z;
////    float x = c * (1.0f - fabs(fmod(HSV.x / 60.0f, 2)-1.0));
////    float m = HSV.z - c;
////
////    if (H >= )
//
//    RGB.x *= 255;
//    RGB.y *= 255;
//    RGB.z *= 255;
////    RGB.a *= 255;
//
////    std::cout << RGB.x << " " << RGB.y << " " << RGB.z << " " << RGB.a << " " << std::endl;
//
//    return RGB;
//  }




  class ParticleEmitter
  {
  public:

    ParticleCollection* particles;

    PrototypesArray* prototypes;
    vector<int>* refPrototypes;

    int maxParticles;
    int particlesPerCycle;
    float emissionRate;
    bool loop;

    ParticleEmitter(ParticleCollection* particlesArray, PrototypesArray* particlePrototypes, vector<int>* _refPrototypes, float _emissionRate, bool _loop)
    : particles( particlesArray )
    , prototypes( particlePrototypes )
    , refPrototypes( _refPrototypes )
    , particlesPerCycle(0)
    , emissionRate(_emissionRate)
    , loop (_loop)
    {
      maxParticles = particlesArray->size;
    }

    virtual ~ParticleEmitter()
    {
      delete( particles );
      delete( prototypes );

    }



    virtual void StartEmission(float deltaTime){ particlesPerCycle = emissionRate * maxParticles;}
    virtual int EmitSingle(unsigned int i) = 0;

    virtual void EmitAll(float deltaTime) = 0;

    virtual void EmitFunction(unsigned int i, bool override = false) = 0;

  };


}



#endif /* PARTICLEEMITTER_H_ */
