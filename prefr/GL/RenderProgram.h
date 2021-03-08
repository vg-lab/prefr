/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/gmrvvis/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef PREFR_PREFR_GL_RENDERPROGRAM_H_
#define PREFR_PREFR_GL_RENDERPROGRAM_H_

#include <reto/reto.h>

#include "IGLRenderProgram.h"

#include <prefr/api.h>

namespace prefr
{
  class PREFR_API RenderProgram : public prefr::IGLRenderProgram,
                        public reto::ShaderProgram
  {
  public:

    
    RenderProgram( )
    : prefr::IGLRenderProgram( )
    , reto::ShaderProgram( )
    {
      _viewProjectionMatrixAlias = std::string( "modelViewProjM" );
      _viewMatrixUpComponentAlias = std::string( "cameraUp" );
      _viewMatrixRightComponentAlias = std::string( "cameraRight" );
    }

    
    virtual ~RenderProgram( ){ }

    
    void prefrActivateGLProgram( void ){ use( );}

    
    unsigned int prefrGLProgramID( void ){ return program( ); }
  };

}


#endif /* PREFR_PREFR_GL_RENDERPROGRAM_H_ */
