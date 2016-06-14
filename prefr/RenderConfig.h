/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
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
 */
#ifndef __PREFR__RENDER_CONFIG__
#define __PREFR__RENDER_CONFIG__

#ifndef PREFR_SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif

#include <vector>

namespace prefr
{

  class RenderConfig
  {
  public:


    RenderConfig(unsigned int size)
    : aliveParticles( 0 )
    , billboardVertices( new std::vector<GLfloat>( size ) )
    , particlePositions( new std::vector<GLfloat>( size ) )
    , particleColors( new std::vector<GLfloat>( size ))
    , vao( 0 )
    , vboBillboardVertex( 0 )
    , vboParticlesPositions( 0 )
    , vboParticlesColors( 0 )
    {}

    virtual ~RenderConfig()
    {
      delete( billboardVertices );
      delete( particlePositions );
      delete( particleColors );
      vao = 0;
      vboBillboardVertex = 0;
      vboParticlesPositions = 0;
      vboParticlesColors = 0;
    }

    unsigned int aliveParticles;

    // Triangles vertices
    std::vector<GLfloat>* billboardVertices;
    std::vector<GLfloat>* particlePositions;
    std::vector<GLfloat>* particleColors;

    // OpenGL pointers
    GLuint vao;
    GLuint vboBillboardVertex;
    GLuint vboParticlesPositions;
    GLuint vboParticlesColors;


  };



}


#endif /* RENDERCONFIG_H_ */
