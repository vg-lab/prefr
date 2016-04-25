/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLERENDERCONFIG_H_
#define DEFAULTPARTICLERENDERCONFIG_H_

#include <prefr/api.h>
#include "../Renderer.h"

namespace prefr
{

  class GLRenderer : public Renderer
  {
  public:

    PREFR_API
    GLRenderer( );
    
    PREFR_API
    virtual ~GLRenderer();

    PREFR_API
    virtual void SetupRender(unsigned int aliveParticles = 0);

    PREFR_API
    virtual void Paint(unsigned int aliveParticles = 0) const;

  };


}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
