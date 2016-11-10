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
#include "Sampler.h"

#include "../utils/types.h"

namespace prefr
{
  static float invRandMax = 1.0f / RAND_MAX;
  static float pi2 = 2.0f * float( M_PI );

  glm::vec3 randomSphericalDirection( float thetaAngle = pi2 )
  {
    float theta, phi, vxz;

    theta = glm::clamp( rand( ) * invRandMax, 0.0f, 1.0f ) * thetaAngle;
    phi = glm::clamp( rand( ) * invRandMax, 0.0f, 1.0f ) * pi2;
    vxz = sinf( theta );

    return glm::vec3( cosf( phi ) * vxz, cosf( theta ), sinf( phi ) * vxz );
  }

  void PointSampler::sample( const Source& source,
                             SampledValues* values ) const
  {
    values->position = source.position( );
    values->direction = randomSphericalDirection( );
    // Identity matrix
    values->rotation = glm::mat4( 1.0f );
  }

  SphereSampler::SphereSampler( float radius_, float angleTheta_ )
  : Sampler( )
  , _radius( radius_ )
  , _angleTheta( angleTheta_ )
  { }

  void SphereSampler::sample( const Source& source,
                              SampledValues* values ) const
  {
    values->direction = randomSphericalDirection( );

    values->position = source.position( ) + values->direction * _radius;

    // TODO calculate rotation
    values->rotation = glm::mat4( 1.0f );
  }

  void SphereSampler::angleTheta( float angle )
  {
    _angleTheta = angle;
  }

  float SphereSampler::angleTheta( void ) const
  {
    return _angleTheta;
  }

  void SphereSampler::radius( float radiusDegrees )
  {
    _radius = radiusDegrees;
  }

  float SphereSampler::radius( void ) const
  {
    return _radius;
  }

}

