/*
 * GLDefaultParticleSystem.cpp
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#include "GLDefaultParticleSystem.h"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace GL
    {
      GLDefaultParticleSystem::GLDefaultParticleSystem (int initialParticlesNumber, int _maxParticles
                                                        , float _emissionRate, bool _loop)
      : DefaultParticleSystem( initialParticlesNumber, _maxParticles, _emissionRate, _loop )
      {
        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();

      }



      void GLDefaultParticleSystem::UpdateCameraDistances(const vec3& cameraPosition)
      {
        unsigned int i = 0;
        for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
        {
         i = ((tparticle_ptr) *it)->id;
         static_cast<GLDefaultParticleSorter*>(sorter)->UpdateCameraDistance(i, cameraPosition);
        }
      }

      void GLDefaultParticleSystem::UpdateRender()
      {
       this->sorter->Sort();

       static_cast<GLDefaultParticleRenderer*>(this->renderer)->SetupRender(this->aliveParticles);
      }

      void GLDefaultParticleSystem::Render()
      {
       static_cast<GLDefaultParticleRenderer*>(this->renderer)->Paint(aliveParticles);
      }

     }

  }

}


