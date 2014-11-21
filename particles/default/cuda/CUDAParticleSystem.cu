/*
 * CUDAParticleSystem.cpp
 *
 *  Created on: 31/10/2014
 *      Author: sgalindo
 */

#include "CUDAParticleSystem.cuh"


namespace particles
{
  namespace defaultParticleSystem
  {

    namespace CUDATHRUST
    {

      CUDAParticleSystem::CUDAParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles
                           , bool _loop)
      : DefaultParticleSystem(initialParticlesNumber, _maxParticles, _loop)
      {

        distances = new distanceArray;

        distances->hostID.resize(maxParticles);
        distances->hostDistances.resize(maxParticles);
        distances->deviceID.resize(maxParticles);
        distances->deviceDistances.resize(maxParticles);

        renderConfig = new RenderConfig();
      }

      void CUDAParticleSystem::UpdateCameraDistances(const glm::vec3& cameraPosition)
      {
        unsigned int i = 0;
        for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
        {
         i = ((tparticle_ptr) *it)->id;
         static_cast<ThrustParticleSorter*>(sorter)->UpdateCameraDistance(i, cameraPosition);
        }
      }
      void CUDAParticleSystem::UpdateRender()
      {
        this->sorter->Sort();

        static_cast<GLCUDAParticleRenderer*>(this->renderer)->SetupRender(this->aliveParticles);

      }
      void CUDAParticleSystem::Render() const
      {
        static_cast<GLCUDAParticleRenderer*>(this->renderer)->Paint(aliveParticles);
      }

    }

  }

}

