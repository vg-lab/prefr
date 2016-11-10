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
#ifndef __PREFR__OSG_RENDERER__
#define __PREFR__OSG_RENDERER__

#include <prefr/api.h>

#include "../core/Renderer.h"
#include "OSGRenderConfig.h"

#ifdef PREFR_USE_OPENSCENEGRAPH

namespace prefr
{

  class OSGRenderer : public Renderer
  {
  public:

    PREFR_API
    OSGRenderer( );

    PREFR_API
    virtual ~OSGRenderer( );

    PREFR_API
    void _init( );

    PREFR_API
    virtual void setupRender( void );

    PREFR_API
    virtual void paint( void ) const;

  };


}

#endif


#endif /* __PREFR__OSG_RENDERER__ */
