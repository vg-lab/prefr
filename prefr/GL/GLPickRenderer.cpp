/*
 * Copyright (c) 2014-2020 GMRV/URJC.
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

#include "GLPickRenderer.h"

#include <iostream>

#include "../utils/Log.h"
#include <string>

#define BACKGROUND_VALUE 16646655

namespace prefr
{
  GLPickRenderer::GLPickRenderer( void )
  : _glPickProgram( nullptr )
  , _framebuffer( -1 )
  , _textureColorbuffer( -1 )
  , _rbo( -1 )
  , _width( -1 )
  , _height( -1 )
  , _recreateFBO( true )
  , _defaultFBO( -1 )
  {

  }
  GLPickRenderer::~GLPickRenderer( void )
  {
    if( _framebuffer != uint32_t( -1 ))
    {
      glDeleteFramebuffers( 1, &_framebuffer );
    }

    if( _textureColorbuffer != uint32_t( -1 ))
    {
      glDeleteTextures( 1, &_textureColorbuffer );
    }

    if( _rbo != uint32_t( -1 ))
    {
      glDeleteRenderbuffers( 1, &_rbo );
    }
  }
  void GLPickRenderer::glPickProgram( IGLRenderProgram* pickProgram )
  {
    assert( pickProgram );
    _glPickProgram = pickProgram;
  }

  void GLPickRenderer::setDefaultFBO( int defaultFBO )
  {
    _defaultFBO = defaultFBO;
  }

  void GLPickRenderer::setWindowSize( uint32_t w, uint32_t h )
  {
    _width = w;
    _height = h;
    _recreateFBO = true;
  }

  uint32_t GLPickRenderer::pick( int posX, int posY )
  {
    glViewport( 0, 0, _width, _height );
    GLint defaultFBO;
    glGetIntegerv( GL_FRAMEBUFFER_BINDING, &defaultFBO );

    GLfloat bkColor[ 4 ];
    glGetFloatv( GL_COLOR_CLEAR_VALUE, bkColor );

    _recreateFBOFunc( );
    glScissor( posX, posY, 1, 1 );
    
    _drawFunc( );

    GLubyte color[ 4 ];
    glReadPixels( posX, posY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color );
    unsigned int idx = color[ 0 ] + color[ 1 ] * 255 + color[ 2 ] * 255 * 255;

    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer( GL_FRAMEBUFFER, defaultFBO );
    glClearColor( bkColor[ 0 ], bkColor[ 1 ], bkColor[ 2 ], bkColor[ 3 ]);

    if( idx == BACKGROUND_VALUE || idx >= _glRenderConfig->_aliveParticles )
      return 0;

    unsigned int value = _distances->getID( idx );

    return value + 1;
  }

  void GLPickRenderer::_recreateFBOFunc( void )
  {
    GLint defaultFBO;
    glGetIntegerv( GL_FRAMEBUFFER_BINDING, &defaultFBO );

    bool completeFBO = true;
    if( _framebuffer == uint32_t( -1 ))
    {
      completeFBO = false;

      glGenFramebuffers( 1, &_framebuffer );
      glBindFramebuffer( GL_FRAMEBUFFER, _framebuffer );

      // create a color attachment texture
      glGenTextures( 1, &_textureColorbuffer );
      glBindTexture( GL_TEXTURE_2D, _textureColorbuffer );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                              GL_TEXTURE_2D, _textureColorbuffer, 0 );

      // create a renderbuffer object for depth and stencil attachment
      glGenRenderbuffers( 1, &_rbo );
    }

    if( _recreateFBO )
    {
      glBindTexture( GL_TEXTURE_2D, _textureColorbuffer );
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, 
                    GL_UNSIGNED_BYTE, nullptr );
      
      glBindRenderbuffer( GL_RENDERBUFFER, _rbo );
      glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height );
    }

    if( completeFBO )
    {
      glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
                                 GL_RENDERBUFFER, _rbo );

      if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
      {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
      }
      glBindFramebuffer( GL_FRAMEBUFFER, defaultFBO );
    }
  }

  void GLPickRenderer::_drawFunc( void )
  {
    glBindVertexArray( _glRenderConfig->_vao );

    glBindFramebuffer( GL_FRAMEBUFFER, _framebuffer );

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_SCISSOR_TEST );

    if( _glPickProgram && _glRenderConfig->_camera )
    {
      glEnable( GL_DEPTH_TEST );
      glDisable( GL_CULL_FACE );
      glDisable( GL_BLEND );

      _glPickProgram->prefrActivateGLProgram( );
      unsigned int programID = _glPickProgram->prefrGLProgramID( );

      unsigned int mvpID = glGetUniformLocation(
          programID, _glPickProgram->prefrViewProjectionMatrixAlias( ));

      glm::mat4x4 tmp =
          _glRenderConfig->_camera->PReFrCameraViewProjectionMatrix( );

      glUniformMatrix4fv( mvpID, 1, GL_FALSE, glm::value_ptr( tmp ));

      unsigned int cameraUpID = glGetUniformLocation(
          programID, _glPickProgram->prefrViewMatrixUpComponentAlias( ));

      unsigned int cameraRightID = glGetUniformLocation(
          programID, _glPickProgram->prefrViewMatrixRightComponentAlias( ));

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
    else
      std::cout << "Render error: Shader " << _glPickProgram
                << " or camera" << _glRenderConfig->_camera
                << " is null." << std::endl;

    glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4,
                           _glRenderConfig->_aliveParticles );

    glBindVertexArray( 0 );

    glEnable( GL_CULL_FACE );

    glFlush( );
    glFinish( );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  }

  std::vector< uint32_t > GLPickRenderer::pickArea( int minPointX, int minPointY, 
                                                    int maxPointX, int maxPointY )
  {
    GLfloat bkColor[ 4 ];
    glGetFloatv( GL_COLOR_CLEAR_VALUE, bkColor );

    _recreateFBOFunc( );
    glScissor( minPointX, minPointY, maxPointX, maxPointY );

    _drawFunc( );

    std::vector< uint32_t > particles;

    GLubyte color[ 4 ];
    for( auto x = minPointX; x < maxPointX; ++x )
    {
      for( auto y = minPointY; y < maxPointY; ++y )
      {
        glReadPixels( x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color );
        unsigned int idx =
            color[ 0 ] + color[ 1 ] * 255 + color[ 2 ] * 65025; // 255 * 255

        if( idx == BACKGROUND_VALUE || idx >= _glRenderConfig->_aliveParticles )
        {
          continue;
        }

        unsigned int value = _distances->getID( idx );

        value = _distances->getID( value );

        particles.push_back( value + 1 );
      }
    }
    
    
    glBindFramebuffer( GL_FRAMEBUFFER, _defaultFBO );
    glDisable( GL_SCISSOR_TEST );
    glClearColor( bkColor[ 0 ], bkColor[ 1 ], bkColor[ 2 ], bkColor[ 3 ]);

    return particles;
  }
}
