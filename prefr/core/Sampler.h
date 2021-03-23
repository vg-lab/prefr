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

#ifndef __PREFR__SAMPLER__
#define __PREFR__SAMPLER__

#include "Source.h"

#include <prefr/api.h>

namespace prefr
{
  class Source;
  class SampledValues;

  class PREFR_API Sampler
  {
  public:
    Sampler( ){ }

    virtual ~Sampler( ){ }

    virtual void sample( const Source& source,
                         SampledValues* values ) const = 0;

  };
  
  class PREFR_API PointSampler : public Sampler
  {
  public:
    PointSampler( ): Sampler( ){ }

    virtual ~PointSampler( ){ }

    virtual void sample( const Source& source,
                         SampledValues* values ) const;
  };

  class PREFR_API SphereSampler : public Sampler
  {
  public:
    SphereSampler( float radius = 0.0f, float angleTheta = 360 );

    virtual ~SphereSampler( ){ }

    virtual void sample( const Source& source,
                         SampledValues* values ) const;

    void angleTheta( float angle );

    float angleTheta( void ) const;

    void radius( float radiusDegrees );

    float radius( void ) const;

  protected:

    float _radius;
    float _angleTheta;
  };
}

#endif /* __PREFR__SAMPLER__ */
