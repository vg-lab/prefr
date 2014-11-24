/*
 * CUDAParticleSystem.h
 *
 *  Created on: 31/10/2014
 *      Author: sgalindo
 */

#ifndef CUDAPARTICLESYSTEM_H_
#define CUDAPARTICLESYSTEM_H_


#include "../DefaultParticleSystem.h"
#include "CUDAThrustTypes.cuh"
#include "ThrustParticleSorter.cuh"
#include "GLCUDAParticleRenderer.cuh"

namespace particles
{
  namespace defaultParticleSystem
  {

    namespace CUDATHRUST
    {

      class CUDAParticleSystem : public DefaultParticleSystem
      {
      public:

        distanceArray* distances;
        RenderConfig* renderConfig;

        CUDAParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop = true);

        virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);
        virtual void UpdateRender();
        virtual void Render() const;

      };

    }
  }
}



#endif /* CUDAPARTICLESYSTEM_H_ */
