/*
 * GLParticleSystem.h
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#ifndef GLPARTICLESYSTEM_H_
#define GLPARTICLESYSTEM_H_

#include "../ParticleSystem.h"
#include "DefaultTypes.h"

#include "DefaultParticleUpdater.h"
#include "DefaultParticleRenderer.h"
#include "DefaultParticleSorter.h"

using namespace std;
using namespace particles;

namespace particles
{
  namespace defaultParticleSystem
  {

    class DefaultParticleUpdater;
    class DefaultParticleRenderer;

    class DefaultGLParticleSystem : public ParticleSystem
    {
    public:

      distanceArray* distances;

      RenderConfig* renderConfig;

      bool updateLoopUnified;

      DefaultGLParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate
                   , bool _loop = true);



      virtual void Start();
      virtual void Update(float deltaTime);
      virtual void UpdateUnified(float deltaTime);
      virtual void UpdateSeparated(float deltaTime);
      virtual void UpdateCameraDistances(vec3 cameraPosition);
      virtual void UpdateRender();
      virtual void Render();
    };

  }
}
#endif /* GLPARTICLESYSTEM_H_ */
