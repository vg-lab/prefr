/*
 * GLCUDAParticleRenderer.h
 *
 *  Created on: 31/10/2014
 *      Author: sgalindo
 */

#ifndef GLCUDAPARTICLERENDERER_H_
#define GLCUDAPARTICLERENDERER_H_

#include <particles/config.h>
#include "../../ParticleRenderer.h"
#include "CUDAThrustTypes.cuh"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace CUDATHRUST
    {
      class GLCUDAParticleRenderer : public ParticleRenderer
      {
      public:

        distanceArray* distances;

        RenderConfig* renderConfig;

        GLCUDAParticleRenderer(ParticleCollection* particlesArray, distanceArray* distancesArray
                               , RenderConfig* renderConfiguration);

        virtual ~GLCUDAParticleRenderer();

        virtual void SetupRender(unsigned int aliveParticles = 0);

        virtual void Paint(unsigned int aliveParticles = 0) const;

      };
    }
  }

}



#endif /* GLCUDAPARTICLERENDERER_H_ */
