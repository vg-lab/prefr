/*
 * Copyright (c) 2014-2018 GMRV/URJC.
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
#include "Model.h"

namespace prefr
{
  Model::Model()
  : _minLife( 0.0f )
  , _maxLife( 0.0f )
  , _lifeRange( 0.0f )
  , _lifeNormalization( 1.0f )
  { }

  Model::Model( float min, float max )
  {
    setLife( min, max );
  }

  Model::~Model( )
  { }

  void Model::setLife( float min, float max )
  {
    _minLife = min;
    _maxLife = max;
    _lifeRange = _maxLife - _minLife;
    _lifeNormalization = 1.0f / _maxLife;
  }

  float Model::minLife( void ) const
  {
    return _minLife;
  }

  float Model::maxLife( void ) const
  {
    return _maxLife;
  }

  float Model::inverseMaxLife( void )
  {
    return _lifeNormalization;
  }

  float Model::lifeInterval( void )
  {
    return _lifeRange;
  }
}

