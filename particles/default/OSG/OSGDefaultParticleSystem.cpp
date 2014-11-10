/*
 * OSGDefaultParticleSystem.cpp
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#include "OSGDefaultParticleSystem.h"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {
      OSGDefaultParticleSystem::OSGDefaultParticleSystem(int initialParticlesNumber, int _maxParticles
                                       , float _emissionRate, bool _loop
                                       , osgGA::StandardManipulator* cam)
      : DefaultParticleSystem(initialParticlesNumber, _maxParticles, _emissionRate, loop)
      {

      }

      void OSGDefaultParticleSystem::UpdateCameraDistances(const vec3& cameraPosition)
      {

      }
      void OSGDefaultParticleSystem::UpdateRender()
      {

      }
      void OSGDefaultParticleSystem::Render()
      {

      }

    }
  }
}


