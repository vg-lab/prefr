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

  GLRenderer::GLRenderer(  )
  : Renderer( )
  , _glRenderConfig( nullptr )
  , _glRenderProgram( nullptr )
  { }

  GLRenderer::~GLRenderer( )
  { }

  void GLRenderer::init( void )
  {
    _glRenderConfig = new GLRenderConfig( _particles.size );
    _renderConfig = _glRenderConfig;

    _glRenderConfig->_glRenderProgram = _glRenderProgram;

    GLfloat b[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
                    -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f };

    _glRenderConfig->billboardVertices = new std::vector<GLfloat>(12);
    _glRenderConfig->particlePositions =
      new std::vector<GLfloat>(_particles.size * 4);
    _glRenderConfig->particleColors =
      new std::vector<GLfloat>(_particles.size * 4);

    for (unsigned int i = 0; i < _glRenderConfig->billboardVertices->size(); i++)
    {
      _glRenderConfig->billboardVertices->at(i) = b[i];
    }

    glGenVertexArrays(1, &_glRenderConfig->vao);
    glBindVertexArray(_glRenderConfig->vao);

    GLuint buffersGL[3];
    glGenBuffers(3, buffersGL);

    _glRenderConfig->vboBillboardVertex = buffersGL[0];
    _glRenderConfig->vboParticlesPositions = buffersGL[1];
    _glRenderConfig->vboParticlesColors = buffersGL[2];

    // Assign billboard vertices
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->vboBillboardVertex);
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof(GLfloat) *
                  _glRenderConfig->billboardVertices->size(),
                  &_glRenderConfig->billboardVertices->front(),
                  GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, _glRenderConfig->vboParticlesPositions);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *
                 _glRenderConfig->particlePositions->size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, _glRenderConfig->vboParticlesColors);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) *
                 _glRenderConfig->particleColors->size(),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // Bind vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _glRenderConfig->vboBillboardVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _glRenderConfig->vboParticlesPositions);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _glRenderConfig->vboParticlesColors);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, (void *) 0);


    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
  }

  void GLRenderer::glRenderProgram( IGLRenderProgram* renderProgram )
  {
    assert( renderProgram );
    _glRenderProgram = renderProgram;

    if( _glRenderConfig )
    {
      _glRenderConfig->_glRenderProgram = _glRenderProgram;
    }
  }

  void GLRenderer::distanceArray( DistanceArray* distances )
  {
    assert( distances );
    Renderer::distanceArray( distances );

    _glRenderConfig->_camera = distances->_camera;

  }

  void GLRenderer::SetupRender( void )
  {
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for
#endif
    for( int i = 0; i < ( int ) _glRenderConfig->aliveParticles; ++i )
    {
      tparticle currentParticle = _particles.GetElement( _distances->getID( i ));

      unsigned int idx = i * 4;

      std::vector< GLfloat >::iterator posit =
          _glRenderConfig->particlePositions->begin( ) + idx;

      *posit = currentParticle.position( ).x;
      ++posit;

      *posit = currentParticle.position( ).y;
      ++posit;

      *posit = currentParticle.position( ).z;
      ++posit;

      *posit = currentParticle.size( );
      ++posit;

      std::vector< GLfloat >::iterator colorit =
          _glRenderConfig->particleColors->begin( ) + idx;

      *colorit = currentParticle.color( ).x;
      ++colorit;

      *colorit = currentParticle.color( ).y;
      ++colorit;

      *colorit = currentParticle.color( ).z;
      ++colorit;

      *colorit = currentParticle.color( ).w;
      ++colorit;

    }

    glBindVertexArray( _glRenderConfig->vao );

    // Update positions buffer
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->vboParticlesPositions );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _glRenderConfig->aliveParticles * 4,
                     &_glRenderConfig->particlePositions->front( ));

    // Update colors buffer
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->vboParticlesColors );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _glRenderConfig->aliveParticles * 4,
                     &_glRenderConfig->particleColors->front( ));

    glBindVertexArray( 0 );
  }

  void GLRenderer::Paint( void ) const
  {
    glBindVertexArray( _glRenderConfig->vao );

    if( _glRenderConfig->_glRenderProgram && _glRenderConfig->_camera )
    {
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      _glRenderProgram->PReFrActivateGLProgram( );
      unsigned int programID = _glRenderProgram->PReFrGLProgramID( );

      unsigned int mvpID = glGetUniformLocation(
          programID, _glRenderProgram->PReFrViewProjectionMatrixAlias( ));

      glUniformMatrix4fv( mvpID, 1, GL_FALSE, glm::value_ptr(
          _glRenderConfig->_camera->PReFrCameraViewProjectionMatrix( )));

      unsigned int cameraUpID = glGetUniformLocation(
          programID, _glRenderProgram->PReFrViewMatrixUpComponentAlias( ));

      unsigned int cameraRightID = glGetUniformLocation(
          programID, _glRenderProgram->PReFrViewMatrixRightComponentAlias( ));

      const glm::mat4x4& viewMatrix =
          std::move( _glRenderConfig->_camera->PReFrCameraViewMatrix( ));

      glUniform3f( cameraUpID,
                   viewMatrix[ 0 ][ 1 ],
                   viewMatrix[ 1 ][ 1 ],
                   viewMatrix[ 2 ][ 1 ]);

      glUniform3f( cameraRightID,
                   viewMatrix[ 0 ][ 0 ],
                   viewMatrix[ 1 ][ 0 ],
                   viewMatrix[ 2 ][ 0 ]);

    }

    glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4,
                           _glRenderConfig->aliveParticles );
    glBindVertexArray( 0 );
  }

}
