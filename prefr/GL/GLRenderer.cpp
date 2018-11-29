/*
 * Copyright (c) 2014-2018 GMRV/URJC.
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

#include "../utils/Log.h"
#include <string>

namespace prefr
{

  GLRenderer::GLRenderer(  )
  : Renderer( )
  , _glRenderConfig( nullptr )
  , _glRenderProgram( nullptr )
  {
    alphaBlendingFunc( ONE_MINUS_SRC_ALPHA );
  }

  GLRenderer::~GLRenderer( )
  { }

  void GLRenderer::_init( void )
  {
    _glRenderConfig = new GLRenderConfig( _particles.size( ));
    _renderConfig = _glRenderConfig;

    _glRenderConfig->_glRenderProgram = _glRenderProgram;

    GLfloat b[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
                    -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f };

    _glRenderConfig->_billboardVertices = new std::vector< GLfloat >( 12 );
    _glRenderConfig->_particlePositions =
        new std::vector< GLfloat >( _particles.size( ) * 4 );
    _glRenderConfig->_particleColors =
        new std::vector< GLfloat >( _particles.size( ) * 4 );

    for ( unsigned int i = 0;
          i < _glRenderConfig->_billboardVertices->size( );
          i++)
    {
      _glRenderConfig->_billboardVertices->at( i ) = b[ i ];
    }

    glGenVertexArrays( 1, &_glRenderConfig->_vao );
    glBindVertexArray( _glRenderConfig->_vao );

    GLuint buffersGL[ 3 ];
    glGenBuffers( 3, buffersGL );

    _glRenderConfig->_boBillboardVertex = buffersGL[ 0 ];
    _glRenderConfig->_vboParticlesPositions = buffersGL[ 1 ];
    _glRenderConfig->_vboParticlesColors = buffersGL[ 2 ];

    // Assign billboard vertices
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_boBillboardVertex );
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof( GLfloat ) *
                  _glRenderConfig->_billboardVertices->size( ),
                  &_glRenderConfig->_billboardVertices->front( ),
                  GL_STATIC_DRAW );


    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_vboParticlesPositions );
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof( GLfloat ) *
                  _glRenderConfig->_particlePositions->size( ),
                  nullptr,
                  GL_DYNAMIC_DRAW );


    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_vboParticlesColors );
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof( GLfloat ) *
                  _glRenderConfig->_particleColors->size( ),
                  nullptr,
                  GL_DYNAMIC_DRAW );

    // Bind vertices
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_boBillboardVertex );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );

    glEnableVertexAttribArray( 1 );
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_vboParticlesPositions );
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, ( void* ) 0 );

    glEnableVertexAttribArray( 2 );
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_vboParticlesColors );
    glVertexAttribPointer( 2, 4, GL_FLOAT, GL_TRUE, 0, ( void * ) 0 );

    glVertexAttribDivisor( 0, 0 );
    glVertexAttribDivisor( 1, 1 );
    glVertexAttribDivisor( 2, 1 );
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

  void GLRenderer::alphaBlendingFunc( BlendFunc blendFunc )
  {
    switch( blendFunc )
    {
      case ONE_MINUS_CONSTANT_ALPHA:

        _blendFunc = blendFunc;
        _blendFuncValue = ( unsigned int ) GL_ONE_MINUS_CONSTANT_ALPHA;

        break;

      case ONE_MINUS_SRC_ALPHA:
      default:

        _blendFunc = blendFunc;
        _blendFuncValue = ( unsigned int ) GL_ONE_MINUS_SRC_ALPHA;

        break;
    }
  }

  GLRenderer::BlendFunc GLRenderer::alphaBlendingFunc( void )
  {
    return _blendFunc;
  }

  void GLRenderer::setupRender( void )
  {
#ifdef PREFR_USE_OPENMP

    #pragma omp parallel for if( _parallel )

#endif
    for( int i = 0; i < ( int ) _glRenderConfig->_aliveParticles; ++i )
    {
      tparticle currentParticle = _particles[ _distances->getID( i )];

      unsigned int idx = i * 4;

      std::vector< GLfloat >::iterator posit =
          _glRenderConfig->_particlePositions->begin( ) + idx;

      *posit = currentParticle.position( ).x;
      ++posit;

      *posit = currentParticle.position( ).y;
      ++posit;

      *posit = currentParticle.position( ).z;
      ++posit;

      *posit = currentParticle.size( );
      ++posit;

      std::vector< GLfloat >::iterator colorit =
          _glRenderConfig->_particleColors->begin( ) + idx;

      *colorit = currentParticle.color( ).x;
      ++colorit;

      *colorit = currentParticle.color( ).y;
      ++colorit;

      *colorit = currentParticle.color( ).z;
      ++colorit;

      *colorit = currentParticle.color( ).w;
      ++colorit;

    }

    glBindVertexArray( _glRenderConfig->_vao );

    // Update positions buffer
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_vboParticlesPositions );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _glRenderConfig->_aliveParticles * 4,
                     &_glRenderConfig->_particlePositions->front( ));

    // Update colors buffer
    glBindBuffer( GL_ARRAY_BUFFER, _glRenderConfig->_vboParticlesColors );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _glRenderConfig->_aliveParticles * 4,
                     &_glRenderConfig->_particleColors->front( ));

    glBindVertexArray( 0 );

  }

  void GLRenderer::paint( void ) const
  {
    glBindVertexArray( _glRenderConfig->_vao );

    if( _glRenderConfig->_glRenderProgram && _glRenderConfig->_camera )
    {
      glEnable( GL_DEPTH_TEST );
      glDepthMask( GL_FALSE );
      glDisable( GL_CULL_FACE );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, _blendFuncValue );

      _glRenderProgram->prefrActivateGLProgram( );
      unsigned int programID = _glRenderProgram->prefrGLProgramID( );

      unsigned int mvpID = glGetUniformLocation(
          programID, _glRenderProgram->prefrViewProjectionMatrixAlias( ));

      glm::mat4x4 tmp =
          _glRenderConfig->_camera->PReFrCameraViewProjectionMatrix( );

      glUniformMatrix4fv( mvpID, 1, GL_FALSE, glm::value_ptr( tmp ));

      unsigned int cameraUpID = glGetUniformLocation(
          programID, _glRenderProgram->prefrViewMatrixUpComponentAlias( ));

      unsigned int cameraRightID = glGetUniformLocation(
          programID, _glRenderProgram->prefrViewMatrixRightComponentAlias( ));

      const glm::mat4x4& viewMatrix =
          _glRenderConfig->_camera->PReFrCameraViewMatrix( );

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
                           _glRenderConfig->_aliveParticles );

    glBindVertexArray( 0 );

    glDepthMask( GL_TRUE );
    glEnable( GL_CULL_FACE );

  }

}
