/*
 * @file  RenderProgram.h
 * @brief
 * @author Sergio E. Galindo <sergio.galindo@urjc.es>
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 *          Do not distribute without further notice.
 */
#ifndef PREFR_PREFR_GL_RENDERPROGRAM_H_
#define PREFR_PREFR_GL_RENDERPROGRAM_H_

#include <reto/reto.h>

#include "IGLRenderProgram.h"

namespace prefr
{
  class RenderProgram : public prefr::IGLRenderProgram,
                        public reto::ShaderProgram
  {
  public:

    PREFR_API
    RenderProgram( )
    : prefr::IGLRenderProgram( )
    , reto::ShaderProgram( )
    {
      _viewProjectionMatrixAlias = std::string( "modelViewProjM" );
      _viewMatrixUpComponentAlias = std::string( "cameraUp" );
      _viewMatrixRightComponentAlias = std::string( "cameraRight" );
    }

    PREFR_API
    virtual ~RenderProgram( ){ }

    PREFR_API
    void prefrActivateGLProgram( void ){ use( );}

    PREFR_API
    unsigned int prefrGLProgramID( void ){ return program( ); }
  };

}


#endif /* PREFR_PREFR_GL_RENDERPROGRAM_H_ */
