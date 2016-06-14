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
    renderConfig = new RenderConfig( _particles.size );

    GLfloat b[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
                    -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f };

    renderConfig->billboardVertices = new std::vector<GLfloat>(12);
    renderConfig->particlePositions =
      new std::vector<GLfloat>(_particles.size * 4);
    renderConfig->particleColors =
      new std::vector<GLfloat>(_particles.size * 4);

    for (unsigned int i = 0; i < renderConfig->billboardVertices->size(); i++)
    {
      renderConfig->billboardVertices->at(i) = b[i];
    }

    glGenVertexArrays(1, &renderConfig->vao);
    glBindVertexArray(renderConfig->vao);

    GLuint buffersGL[3];
    glGenBuffers(3, buffersGL);

    renderConfig->vboBillboardVertex = buffersGL[0];
    renderConfig->vboParticlesPositions = buffersGL[1];
    renderConfig->vboParticlesColors = buffersGL[2];

    // Assign billboard vertices
    glBindBuffer( GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof(GLfloat) *
                  renderConfig->billboardVertices->size(),
                  &renderConfig->billboardVertices->front(),
                  GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *
                 renderConfig->particlePositions->size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColors);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *
                 renderConfig->particleColors->size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // Bind vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColors);
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

    for (unsigned int i = 0; i < renderConfig->aliveParticles; ++i )
    {
      tparticle currentParticle = _particles.GetElement(distances->getID(i));

      unsigned int idx = i * 4;

      std::vector< GLfloat >::iterator posit =
          renderConfig->particlePositions->begin( ) + idx;

      *posit = currentParticle.position( ).x;
      ++posit;

      *posit = currentParticle.position( ).y;
      ++posit;

      *posit = currentParticle.position( ).z;
      ++posit;

      *posit = currentParticle.size( );
      ++posit;

      std::vector< GLfloat >::iterator colorit =
          renderConfig->particleColors->begin( ) + idx;

      *colorit = currentParticle.color( ).x;
      ++colorit;

      *colorit = currentParticle.color( ).y;
      ++colorit;

      *colorit = currentParticle.color( ).z;
      ++colorit;

      *colorit = currentParticle.color( ).w;
      ++colorit;

    }

    glBindVertexArray( renderConfig->vao );

    // Update positions buffer
    glBindBuffer( GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * renderConfig->aliveParticles * 4,
                     &renderConfig->particlePositions->front( ));

    // Update colors buffer
    glBindBuffer( GL_ARRAY_BUFFER, renderConfig->vboParticlesColors );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * renderConfig->aliveParticles * 4,
                     &renderConfig->particleColors->front( ));

    glBindVertexArray( 0 );
  }

  void GLRenderer::Paint( void ) const
  {
    glBindVertexArray( renderConfig->vao );
    glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4,
                           renderConfig->aliveParticles );
    glBindVertexArray( 0 );
  }

}
