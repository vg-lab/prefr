/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLERENDERCONFIG_H_
#define DEFAULTPARTICLERENDERCONFIG_H_

#include <prefr/api.h>
#include "../ParticleRenderer.h"

namespace prefr
{

  class GLDefaultParticleRenderer : public ParticleRenderer
  {
  public:

    PREFR_API
    GLDefaultParticleRenderer( const ParticleCollection&particlesArray);
    
    PREFR_API
    virtual ~GLDefaultParticleRenderer();

    PREFR_API
    virtual void SetupRender(unsigned int aliveParticles = 0);

    PREFR_API
    virtual void Paint(unsigned int aliveParticles = 0) const;

  };


}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
