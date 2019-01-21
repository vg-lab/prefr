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
#include "GLPickRenderer.h"

#include <iostream>

#include "../utils/Log.h"
#include <string>

namespace prefr
{
  void GLPickRenderer::glPickProgram( IGLRenderProgram* pickProgram )
  {
    assert( pickProgram );
    _glPickProgram = pickProgram;

    //if( _glRenderConfig )
    //{
    //  _glRenderConfig->_glPickProgram = _glPickProgram;
    //}
  }

  void GLPickRenderer::setWindowSize( uint32_t w, uint32_t h )
  {
    width = w;
    height = h;
    recreateFBO = true;
  }

  tparticle GLPickRenderer::pick( int posX, int posY )
  {
    if( framebuffer == uint32_t( -1 ) )
    {
      glGenFramebuffers(1, &framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

      // create a color attachment texture
      glGenTextures(1, &textureColorbuffer);
      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
      glGenRenderbuffers(1, &rbo);
    }

    if( recreateFBO )
    {
      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
      
      glBindRenderbuffer(GL_RENDERBUFFER, rbo);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
      // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
      }
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glBindVertexArray( _glRenderConfig->_vao );

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

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
                           _glRenderConfig->aliveParticles );

    glBindVertexArray( 0 );

    glEnable( GL_CULL_FACE );

    glFlush( );
    glFinish( );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    
    GLubyte color[4];
    glReadPixels(posX, posY, 1, 1,
      GL_RGBA, GL_UNSIGNED_BYTE, color);
    unsigned int value = color[0] + color[1] * 255 + color[2] * 255 * 255;

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    value = _distances->getID( value );

    if (value < uint32_t( -1 ) )
    {
      std::cout << value << std::endl;
    }
    std::cout << "R: " << (int)color[0] << ", G: " << (int)color[1] << ", B: " << (int)color[2] << std::endl;
        
    return _particles.GetElement( value );
  }

}
