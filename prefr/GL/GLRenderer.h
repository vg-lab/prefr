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

#ifndef __PREFR__GL_RENDERER__
#define __PREFR__GL_RENDERER__

#include <prefr/api.h>

#include "../core/Renderer.h"
#include "GLRenderConfig.h"

namespace prefr
{

  class GLRenderer : public Renderer
  {

  public:

    enum BlendFunc
    {
      ONE_MINUS_SRC_ALPHA = 0,
      ONE_MINUS_CONSTANT_ALPHA = 1
    };

    PREFR_API
    GLRenderer( );

    PREFR_API
    virtual ~GLRenderer( );

    PREFR_API
    virtual void setupRender( void );

    PREFR_API
    virtual void paint( void ) const;

    PREFR_API
    virtual void glRenderProgram( IGLRenderProgram* renderProgram );

    PREFR_API
    virtual void distanceArray( DistanceArray* distances );

    virtual void alphaBlendingFunc( BlendFunc blendFunc );
    BlendFunc alphaBlendingFunc( void );

  protected:

    void _init( void );

    GLRenderConfig* _glRenderConfig;
    IGLRenderProgram* _glRenderProgram;

    unsigned int _blendFuncValue;
    BlendFunc _blendFunc;
  };


}




#endif /* __PREFR__GL_RENDERER__ */
