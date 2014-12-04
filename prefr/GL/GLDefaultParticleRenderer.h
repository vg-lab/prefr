/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLERENDERCONFIG_H_
#define DEFAULTPARTICLERENDERCONFIG_H_

// #include <particles/config.h>
#include "../ParticleRenderer.h"

namespace prefr
{

  class GLDefaultParticleRenderer : public ParticleRenderer
  {
  public:

    GLDefaultParticleRenderer( const ParticleCollection&particlesArray);

    virtual ~GLDefaultParticleRenderer();

    virtual void SetupRender(unsigned int aliveParticles = 0);

    virtual void Paint(unsigned int aliveParticles = 0) const;

  };


}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
