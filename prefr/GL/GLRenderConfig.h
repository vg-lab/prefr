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

#ifndef __PREFR__GLRENDERCONFIG__
#define __PREFR__GLRENDERCONFIG__

#ifndef PREFR_SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif

#include <vector>

#include "../core/RenderConfig.h"
#include "IGLRenderProgram.h"
#include <prefr/api.h>

namespace prefr
{
  class PREFR_API GLRenderConfig : public RenderConfig
  {
    friend class GLRenderer;
    friend class GLPickRenderer;

  public:

    GLRenderConfig( unsigned int size )
    : RenderConfig( size )
    , _billboardVertices( new std::vector< GLfloat >( size ) )
    , _particlePositions( new std::vector< GLfloat >( size ) )
    , _particleColors( new std::vector< GLfloat >( size ))
    , _vao( 0 )
    , _boBillboardVertex( 0 )
    , _vboParticlesPositions( 0 )
    , _vboParticlesColors( 0 )
    , _camera( nullptr )
    , _glRenderProgram( nullptr )
    {}

    virtual ~GLRenderConfig( )
    {
      delete( _billboardVertices );
      delete( _particlePositions );
      delete( _particleColors );
      _vao = 0;
      _boBillboardVertex = 0;
      _vboParticlesPositions = 0;
      _vboParticlesColors = 0;
    }

  protected:

    // Triangles vertices
    std::vector< GLfloat >* _billboardVertices;
    std::vector< GLfloat >* _particlePositions;
    std::vector< GLfloat >* _particleColors;

    // OpenGL pointers
    GLuint _vao;
    GLuint _boBillboardVertex;
    GLuint _vboParticlesPositions;
    GLuint _vboParticlesColors;

    ICamera* _camera;
    IGLRenderProgram* _glRenderProgram;
  };
}

#endif /* __PREFR__GLRENDERCONFIG__ */
