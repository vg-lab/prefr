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

#ifndef __PREFR__GL_PICKRENDERER__
#define __PREFR__GL_PICKRENDERER__

#include <prefr/api.h>

#include "GLRenderer.h"
#include "GLRenderConfig.h"

#include <vector>

namespace prefr
{
  class GLPickRenderer : public GLRenderer
  {
  public:

    PREFR_API
    GLPickRenderer( void );

    PREFR_API
    virtual ~GLPickRenderer( void );

    PREFR_API
    virtual uint32_t pick( int posX, int posY );

    PREFR_API
    virtual std::vector< uint32_t > pickArea( int minPointX, int minPointY, 
                                              int maxPointX, int maxPointY );

    void setDefaultFBO( int defaultFBO );

    PREFR_API
    void setWindowSize( uint32_t w, uint32_t h );

    PREFR_API
    void glPickProgram( IGLRenderProgram* renderProgram );

  protected:

    void _recreateFBOFunc( void );
    void _drawFunc( void );

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
