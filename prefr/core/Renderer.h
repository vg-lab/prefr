/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
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

#ifndef __PREFR__RENDERER__
#define __PREFR__RENDERER__

#include <prefr/api.h>
#include "../utils/types.h"

#include "Particles.h"

#include "DistanceArray.hpp"
#include "RenderConfig.h"

namespace prefr
{
  class PREFR_API Renderer
  {
    friend class ParticleSystem;

  public:
     Renderer( );

     virtual ~Renderer();

     virtual void setupRender( void ) = 0;

     virtual void paint( void ) const = 0;

     RenderConfig* renderConfig( void ) const;

     virtual void distanceArray( DistanceArray* distanceArray );

     void particles( const ParticleRange& particles );

  protected:
    virtual void _init( void ) = 0;

    ParticleCollection _particles;

    DistanceArray* _distances;
    RenderConfig* _renderConfig;

    bool _parallel;
  };
}

#endif /* __PREFR__RENDERER__ */
