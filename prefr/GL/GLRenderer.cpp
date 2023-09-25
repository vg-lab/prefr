/*
 * Copyright (c) 2014-2022 VG-Lab/URJC.
 *
 * Authors:
 * - Sergio E. Galindo <sergio.galindo@urjc.es>
 * - Gael Rial Costas <g.rial.2018@alumnos.urjc.es>
 *
 * This file is part of PReFr <https://github.com/vg-lab/prefr>
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

#include <prefr/core/ParticleSystem.h>
#include <prefr/GL/IGLRenderProgram.h>

namespace prefr
{
  GLRenderer::GLRenderer( IGLRenderProgram* program )
    : GLAbstractRenderer( false )
    , _glRenderProgram( program )
    , _blendFuncValue( GL_ONE_MINUS_SRC_ALPHA )
    , _particleAmount( 0 )
    , _vao( 0 )
    , _vertexVBO( 0 )
    , _sizeVBO( 0 )
    , _positionVBO( 0 )
    , _colorVBO( 0 )
    , _uniformMatrix( 0 )
    , _uniformCameraUp( 0 )
    , _uniformCameraRight( 0 )
  {
  }

  GLRenderer::~GLRenderer( )
  {
    setRenderProgram( _glRenderProgram );
  }

  void GLRenderer::_init( unsigned int particleAmount )
  {

    _particleAmount = particleAmount;

    GLfloat vertices[] = { -0.5f , -0.5f , 0.0f , 0.5f , -0.5f , 0.0f ,
                           -0.5f , 0.5f , 0.0f , 0.5f , 0.5f , 0.0f };

    _sizes.resize( particleAmount );
    _positions.resize( particleAmount * 3 );
    _colors.resize( particleAmount * 4 );

    glGenVertexArrays( 1 , &_vao );
    glBindVertexArray( _vao );

    GLuint buffersGL[4];
    glGenBuffers( 4 , buffersGL );
    _vertexVBO = buffersGL[ 0 ];
    _sizeVBO = buffersGL[ 1 ];
    _positionVBO = buffersGL[ 2 ];
    _colorVBO = buffersGL[ 3 ];

    GLsizeiptr aux = static_cast<int>(sizeof( GLfloat )) * particleAmount;

    // Assign billboard vertices
    glBindBuffer( GL_ARRAY_BUFFER , _vertexVBO );
    glBufferData( GL_ARRAY_BUFFER ,
                  sizeof( vertices ) ,
                  &vertices ,
                  GL_STATIC_DRAW );


    glBindBuffer( GL_ARRAY_BUFFER , _sizeVBO );
    glBufferData( GL_ARRAY_BUFFER ,
                  aux ,
                  nullptr ,
                  GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER , _positionVBO );
    glBufferData( GL_ARRAY_BUFFER ,
                  aux * 3 ,
                  nullptr ,
                  GL_DYNAMIC_DRAW );


    glBindBuffer( GL_ARRAY_BUFFER , _colorVBO );
    glBufferData( GL_ARRAY_BUFFER ,
                  aux * 4 ,
                  nullptr ,
                  GL_DYNAMIC_DRAW );

    // Bind vertices
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER , _vertexVBO );
    glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , nullptr );

    glEnableVertexAttribArray( 1 );
    glBindBuffer( GL_ARRAY_BUFFER , _sizeVBO );
    glVertexAttribPointer( 1 , 1 , GL_FLOAT , GL_FALSE , 0 , nullptr );

    glEnableVertexAttribArray( 2 );
    glBindBuffer( GL_ARRAY_BUFFER , _positionVBO );
    glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_TRUE , 0 , nullptr );

    glEnableVertexAttribArray( 3 );
    glBindBuffer( GL_ARRAY_BUFFER , _colorVBO );
    glVertexAttribPointer( 3 , 4 , GL_FLOAT , GL_TRUE , 0 , nullptr );

    glVertexAttribDivisor( 0 , 0 );
    glVertexAttribDivisor( 1 , 1 );
    glVertexAttribDivisor( 2 , 1 );
    glVertexAttribDivisor( 3 , 1 );
  }

  void GLRenderer::enableAccumulativeMode( bool accumulativeMode )
  {
    _blendFuncValue = accumulativeMode
                      ? ( unsigned int ) GL_ONE_MINUS_CONSTANT_ALPHA
                      : ( unsigned int ) GL_ONE_MINUS_SRC_ALPHA;
  }

  void GLRenderer::updateRender( ParticleSystem& system )
  {
    auto aliveParticles = static_cast<int>(system.aliveParticles( ));
    auto& particles = system.particles( );
    auto distances = system.sorter( )->getDistanceArray( );

#ifdef PREFR_USE_OPENMP
#pragma omp parallel for if( system.isParallel())
#endif
    for ( int i = 0; i < aliveParticles; i++ )
    {
      tparticle currentParticle = particles[ distances->getID( i ) ];

      auto positionIterator = _positions.begin( ) + i * 3;
      auto colorIterator = _colors.begin( ) + i * 4;

      _sizes.at( i ) = currentParticle.size( );

      *positionIterator++ = currentParticle.position( ).x;
      *positionIterator++ = currentParticle.position( ).y;
      *positionIterator++ = currentParticle.position( ).z;

      *colorIterator++ = currentParticle.color( ).x;
      *colorIterator++ = currentParticle.color( ).y;
      *colorIterator++ = currentParticle.color( ).z;
      *colorIterator++ = currentParticle.color( ).w;
    }

    glBindVertexArray( _vao );

    GLsizeiptr aux = static_cast<int>(sizeof( GLfloat )) * aliveParticles;

    glBindBuffer( GL_ARRAY_BUFFER , _sizeVBO );
    glBufferSubData( GL_ARRAY_BUFFER , 0 , aux , _sizes.data( ));

    glBindBuffer( GL_ARRAY_BUFFER , _positionVBO );
    glBufferSubData( GL_ARRAY_BUFFER , 0 , aux * 3 , _positions.data( ));

    glBindBuffer( GL_ARRAY_BUFFER , _colorVBO );
    glBufferSubData( GL_ARRAY_BUFFER , 0 , aux * 4 , _colors.data( ));

    glBindVertexArray( 0 );
  }

  void GLRenderer::paint( const ParticleSystem& system ) const
  {
    glBindVertexArray( _vao );

    auto camera = system.getCamera( );

    if ( _glRenderProgram && camera )
    {
      glEnable( GL_DEPTH_TEST );
      glDepthMask( GL_FALSE );
      glDisable( GL_CULL_FACE );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA , _blendFuncValue );

      _glRenderProgram->prefrActivateGLProgram( );

      const glm::mat4x4 tmp = camera->PReFrCameraViewProjectionMatrix( );
      const glm::mat4x4& viewMatrix = camera->PReFrCameraViewMatrix( );

      glUniformMatrix4fv( _uniformMatrix , 1 , GL_FALSE ,
                          glm::value_ptr( tmp ));


      glUniform3f( _uniformCameraUp ,
                   viewMatrix[ 0 ][ 1 ] ,
                   viewMatrix[ 1 ][ 1 ] ,
                   viewMatrix[ 2 ][ 1 ] );

      glUniform3f( _uniformCameraRight ,
                   viewMatrix[ 0 ][ 0 ] ,
                   viewMatrix[ 1 ][ 0 ] ,
                   viewMatrix[ 2 ][ 0 ] );
    }

    glDrawArraysInstanced( GL_TRIANGLE_STRIP , 0 , 4 ,
                           system.aliveParticles( ));

    glBindVertexArray( 0 );

    glDepthMask( GL_TRUE );
    glEnable( GL_CULL_FACE );
  }

  void GLRenderer::_dispose( )
  {
    glDeleteBuffers( 1 , &_vertexVBO );
    glDeleteBuffers( 1 , &_sizeVBO );
    glDeleteBuffers( 1 , &_positionVBO );
    glDeleteBuffers( 1 , &_colorVBO );
    glDeleteVertexArrays( 1 , &_vao );
  }

  IGLRenderProgram* GLRenderer::getRenderProgram( )
  {
    return _glRenderProgram;
  }

  void GLRenderer::setRenderProgram( IGLRenderProgram* program )
  {
    _glRenderProgram = program;

    if ( _glRenderProgram != nullptr )
    {
      unsigned int programID = _glRenderProgram->prefrGLProgramID( );
      _uniformMatrix = glGetUniformLocation(
        programID , _glRenderProgram->prefrViewProjectionMatrixAlias( ));
      _uniformCameraUp = glGetUniformLocation(
        programID , _glRenderProgram->prefrViewMatrixUpComponentAlias( ));
      _uniformCameraRight = glGetUniformLocation(
        programID , _glRenderProgram->prefrViewMatrixRightComponentAlias( ));
    }
  }
}
