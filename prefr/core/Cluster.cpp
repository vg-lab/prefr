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
#include "Cluster.h"

namespace prefr
{

  Cluster::Cluster( )
  : aliveParticles( 0 )
  , _particles( )
  , _size( 0 )
  , _source( nullptr )
  , _model( nullptr )
  , _updater( nullptr )
  , _active( true )
  , _inactiveKillParticles( false )
  { }

  Cluster::~Cluster( void )
  {

  }

  Source* Cluster::source( void ) const
  {
    return _source;
  }

  void Cluster::source( Source* source_ )
  {
    _source = source_;
    _source->cluster( this );
  }

  Model* Cluster::model( void ) const
  {
    return _model;
  }

  void Cluster::model( Model* model_ )
  {
    _model = model_;
  }

  Updater* Cluster::updater( void ) const
  {
    return _updater;
  }

  void Cluster::updater( Updater* updater_)
  {
    _updater = updater_;
  }

  ParticleRange Cluster::particles( void ) const
  {
    return _particles;
  }

  void Cluster::particles( const ParticleRange& particleArray )
  {
    _particles = particleArray;
  }

  bool Cluster::active( void ) const
  {
    return _active;
  }

  void Cluster::active( bool active_ )
  {
    _active = active_;
  }

  bool Cluster::inactiveKillParticles( void ) const
  {
    return _inactiveKillParticles;
  }

  void Cluster::inactiveKillParticles( bool killParticles_ )
  {
    _inactiveKillParticles = killParticles_;
  }

  void Cluster::killParticles( bool changeState )
  {
    _source->_deadParticles.clear( );

    for( tparticle particle = _particles.begin( );
         particle != _particles.end( );
         particle++ )
    {
      particle.set_life( 0.0f );

      if( changeState )
      {
        particle.set_alive( false );

        #pragma omp critical
        {
          _source->_deadParticles.push_back( particle.id( ));
        }
      }
    }
  }

}


