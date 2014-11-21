/*
 * GLDefaultParticleSystem.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef GLDEFAULTPARTICLESYSTEM_H_
#define GLDEFAULTPARTICLESYSTEM_H_

#include "../DefaultParticleSystem.h"

#include "GLDefaultParticleRenderer.h"
#include "GLDefaultParticleSorter.h"


namespace particles
{
  namespace defaultParticleSystem
  {

    namespace GL
    {

      class GLDefaultParticleSystem : public DefaultParticleSystem
      {
      public:
        distanceArray* distances;

        RenderConfig* renderConfig;

        GLDefaultParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate
                     , bool _loop = true);

        virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);
        virtual void UpdateRender();
        virtual void Render() const;
      };

    }
  }

}



#endif /* GLDEFAULTPARTICLESYSTEM_H_ */
