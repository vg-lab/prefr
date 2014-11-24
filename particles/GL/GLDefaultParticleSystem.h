/*
 * GLDefaultParticleSystem.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef GLDEFAULTPARTICLESYSTEM_H_
#define GLDEFAULTPARTICLESYSTEM_H_

#include "../ParticleSystem.h"

#include "GLDefaultParticleRenderer.h"
#include "GLDefaultParticleSorter.h"


namespace particles
{
  namespace defaultParticleSystem
  {

    namespace GL
    {

      class GLDefaultParticleSystem : public ParticleSystem
      {
      public:
        distanceArray* distances;

        RenderConfig* renderConfig;

        GLDefaultParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop = true);

        virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);
        virtual void UpdateRender();
        virtual void Render() const;
      };

    }
  }

}



#endif /* GLDEFAULTPARTICLESYSTEM_H_ */
