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
#ifndef __PREFR__SAMPLER__
#define __PREFR__SAMPLER__

#include "Source.h"

namespace prefr
{
  class Source;
  class SampledValues;

  class Sampler
  {
  public:

    PREFR_API
    Sampler( ){ }

    PREFR_API
    virtual ~Sampler( ){ }

    PREFR_API
    virtual void sample( const Source& source,
                         SampledValues* values ) const = 0;

  };


  class PointSampler : public Sampler
  {
  public:

    PREFR_API
    PointSampler( ): Sampler( ){ }

    PREFR_API
    virtual ~PointSampler( ){ }

    PREFR_API
    virtual void sample( const Source& source,
                         SampledValues* values ) const;
  };

  class SphereSampler : public Sampler
  {
  public:

    PREFR_API
    SphereSampler( float radius = 0.0f, float angleTheta = 360 );

    PREFR_API
    virtual ~SphereSampler( ){ }

    PREFR_API
    virtual void sample( const Source& source,
                         SampledValues* values ) const;

    PREFR_API
    void angleTheta( float angle );

    PREFR_API
    float angleTheta( void ) const;

    PREFR_API
    void radius( float radiusDegrees );

    PREFR_API
    float radius( void ) const;

  protected:

    float _radius;
    float _angleTheta;

  };

}

#endif /* __PREFR__SAMPLER__ */
