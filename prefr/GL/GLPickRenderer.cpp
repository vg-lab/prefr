/*
 * Copyright (c) 2014-2019 GMRV/URJC.
 *
 * Authors: Cristian Rodríguez <cristian.rodriguez@urjc.es>
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
#include "GLPickRenderer.h"

#include <iostream>

#include "../utils/Log.h"
#include <string>

namespace prefr
{
  GLPickRenderer::~GLPickRenderer( void )
  {
    if( framebuffer != uint32_t( -1 ) )
    {
      glDeleteFramebuffers( 1, &framebuffer );
    }
    if( textureColorbuffer != uint32_t( -1 ) )
    {
      glDeleteTextures( 1, &textureColorbuffer );
    }
    if( rbo != uint32_t( -1 ) )
    {
      glDeleteRenderbuffers( 1, &rbo );
    }
  }
  void GLPickRenderer::glPickProgram( IGLRenderProgram* pickProgram )
  {
    assert( pickProgram );
    _glPickProgram = pickProgram;
  }

  void GLPickRenderer::setWindowSize( uint32_t w, uint32_t h )
  {
    width = w;
    height = h;
    recreateFBO = true;
  }

  uint32_t GLPickRenderer::pick( int posX, int posY )
  {
    GLint defaultFBO;
    glGetIntegerv( GL_FRAMEBUFFER_BINDING, &defaultFBO);

    GLfloat bkColor[ 4 ];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);

    recreateFBOFunc( );
    glScissor( posX, posY, 1, 1 );
    
    drawFunc( );

    GLubyte color[ 4 ];
    glReadPixels( posX, posY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color );
    unsigned int value = color[0] + color[1] * 255 + color[2] * 255 * 255;

    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer( GL_FRAMEBUFFER, defaultFBO );
    glClearColor( bkColor[ 0 ], bkColor[ 1 ], bkColor[ 2 ], bkColor[ 3 ] );

    if (value == BACKGROUND_VALUE )
    {
      return 0;
    }

    value = _distances->getID( value );

    if( value >= _glRenderConfig->_aliveParticles )
    {
      return 0;
    }

    return value + 1;
  }

  void GLPickRenderer::recreateFBOFunc( void )
  {
    GLint defaultFBO;
    glGetIntegerv( GL_FRAMEBUFFER_BINDING, &defaultFBO );

    bool completeFBO = true;
    if( framebuffer == uint32_t( -1 ) )
    {
      completeFBO = false;

      glGenFramebuffers(1, &framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

      // create a color attachment texture
      glGenTextures(1, &textureColorbuffer);
      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
        GL_TEXTURE_2D, textureColorbuffer, 0 );

      // create a renderbuffer object for depth and stencil attachment
      glGenRenderbuffers(1, &rbo);
    }

    if( recreateFBO )
    {
      glBindTexture( GL_TEXTURE_2D, textureColorbuffer );
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
        GL_UNSIGNED_BYTE, nullptr );
      
      glBindRenderbuffer( GL_RENDERBUFFER, rbo );
      glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height );
    }

    if( completeFBO )
    {
      glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
        GL_RENDERBUFFER, rbo );
      if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
      {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
      }
      glBindFramebuffer( GL_FRAMEBUFFER, defaultFBO );
    }
  }

  void GLPickRenderer::drawFunc( void )
  {
    glBindVertexArray( _glRenderConfig->_vao );

    glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable(GL_SCISSOR_TEST);

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
    glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);

    recreateFBOFunc( );
    glScissor( minPointX, minPointY, maxPointX, maxPointY );

    drawFunc( );

    std::vector< uint32_t > particles;

    GLubyte color[4];
    for( auto x = minPointX; x < maxPointX; ++x )
    {
      for( auto y = minPointY; y < maxPointY; ++y )
      {
        glReadPixels( x, y, 1, 1,
          GL_RGBA, GL_UNSIGNED_BYTE, color);
        unsigned int value = color[0] + color[1] * 255 + color[2] * 255 * 255;

        value = _distances->getID( value );

        if( value == BACKGROUND_VALUE )
        {
          continue;
        }

        value = _distances->getID( value );

        if( value >= _glRenderConfig->_aliveParticles )
        {
          continue;
        }

        particles.push_back( value + 1);
      }
    }
    
    
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glDisable(GL_SCISSOR_TEST);
    glClearColor( bkColor[ 0 ], bkColor[ 1 ], bkColor[ 2 ], bkColor[ 3 ] );

    return particles;
  }
}
