/*
 * Copyright (c) 2014-2022 VG-Lab/URJC.
 *
 * Authors:
 * - Sergio E. Galindo <sergio.galindo@urjc.es>
 * - Gael Rial Costas <g.rial.2018@alumnos.urjc.es>
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

#ifndef __PREFR__GL_PICKRENDERER__
#define __PREFR__GL_PICKRENDERER__

#include <prefr/api.h>

#include "GLRenderer.h"

#include <vector>

namespace prefr
{
  class PREFR_API GLPickRenderer : public GLRenderer
  {
  public:


    explicit GLPickRenderer( IGLRenderProgram* program = nullptr );

    ~GLPickRenderer( ) override;


    virtual uint32_t pick( const ParticleSystem& system , int posX , int posY );


    virtual std::vector< uint32_t > pickArea(
      const ParticleSystem& system ,
      int minPointX , int minPointY ,
      int maxPointX , int maxPointY );

    void setDefaultFBO( int defaultFBO );

    void setWindowSize( uint32_t w , uint32_t h );

    void glPickProgram( IGLRenderProgram* renderProgram );

  protected:

    void _dispose () override;

    void _recreateFBOFunc( );

    void _drawFunc( const ParticleSystem& system );

    IGLRenderProgram* _glPickProgram;
    uint32_t _framebuffer;
    uint32_t _textureColorbuffer;
    uint32_t _rbo;
    uint32_t _width;
    uint32_t _height;
    bool _recreateFBO;

    int _defaultFBO;
  };


}


#endif /* __PREFR__GL_PICKRENDERER__ */
