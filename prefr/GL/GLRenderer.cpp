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
 *
 */
#include "GLRenderer.h"

#include <iostream>

namespace prefr
{

  GLRenderer::GLRenderer( )
  : Renderer( )
  { }

  GLRenderer::~GLRenderer( )
  { }

  void GLRenderer::init( void )
  {
    _renderConfig = new RenderConfig( _particles.size );

    GLfloat b[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
                    -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f };

    _renderConfig->billboardVertices = new std::vector<GLfloat>(12);
    _renderConfig->particlePositions =
      new std::vector<GLfloat>(_particles.size * 4);
    _renderConfig->particleColors =
      new std::vector<GLfloat>(_particles.size * 4);

    for (unsigned int i = 0; i < _renderConfig->billboardVertices->size(); i++)
    {
      _renderConfig->billboardVertices->at(i) = b[i];
    }

    glGenVertexArrays(1, &_renderConfig->vao);
    glBindVertexArray(_renderConfig->vao);

    GLuint buffersGL[3];
    glGenBuffers(3, buffersGL);

    _renderConfig->vboBillboardVertex = buffersGL[0];
    _renderConfig->vboParticlesPositions = buffersGL[1];
    _renderConfig->vboParticlesColors = buffersGL[2];

    // Assign billboard vertices
    glBindBuffer( GL_ARRAY_BUFFER, _renderConfig->vboBillboardVertex);
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof(GLfloat) *
                  _renderConfig->billboardVertices->size(),
                  &_renderConfig->billboardVertices->front(),
                  GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, _renderConfig->vboParticlesPositions);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *
                 _renderConfig->particlePositions->size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, _renderConfig->vboParticlesColors);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *
                 _renderConfig->particleColors->size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // Bind vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _renderConfig->vboBillboardVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _renderConfig->vboParticlesPositions);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _renderConfig->vboParticlesColors);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, (void *) 0);


    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
  }

  void GLRenderer::SetupRender( void )
  {
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for
#endif
    for( int i = 0; i < ( int ) _renderConfig->aliveParticles; ++i )
    {
      tparticle currentParticle = _particles.GetElement( _distances->getID( i ));

      unsigned int idx = i * 4;

      std::vector< GLfloat >::iterator posit =
          _renderConfig->particlePositions->begin( ) + idx;

      *posit = currentParticle.position( ).x;
      ++posit;

      *posit = currentParticle.position( ).y;
      ++posit;

      *posit = currentParticle.position( ).z;
      ++posit;

      *posit = currentParticle.size( );
      ++posit;

      std::vector< GLfloat >::iterator colorit =
          _renderConfig->particleColors->begin( ) + idx;

      *colorit = currentParticle.color( ).x;
      ++colorit;

      *colorit = currentParticle.color( ).y;
      ++colorit;

      *colorit = currentParticle.color( ).z;
      ++colorit;

      *colorit = currentParticle.color( ).w;
      ++colorit;

    }

    glBindVertexArray( _renderConfig->vao );

    // Update positions buffer
    glBindBuffer( GL_ARRAY_BUFFER, _renderConfig->vboParticlesPositions );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _renderConfig->aliveParticles * 4,
                     &_renderConfig->particlePositions->front( ));

    // Update colors buffer
    glBindBuffer( GL_ARRAY_BUFFER, _renderConfig->vboParticlesColors );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _renderConfig->aliveParticles * 4,
                     &_renderConfig->particleColors->front( ));

    glBindVertexArray( 0 );
  }

  void GLRenderer::Paint( void ) const
  {
    glBindVertexArray( _renderConfig->vao );
    glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4,
                           _renderConfig->aliveParticles );
    glBindVertexArray( 0 );
  }

}
