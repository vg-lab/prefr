/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/vg-lab/prefr>
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

  Cluster::Cluster( void )
  : _updateConfig( nullptr )
  , _aliveParticles( 0 )
  , _active( true )
  , _inactiveKillParticles( false )
  { }

  Cluster::~Cluster( void )
  { }

  ParticleRange& Cluster::particles( void )
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
    for( auto particle : _particles )
    {
      particle.set_life( 0.0f );

      if( changeState )
      {
        particle.set_alive( false );

        _updateConfig->setDead( particle.id( ), true );
      }
    }
  }

  void Cluster::setSource( Source* source_, bool resetState )
  {
    _updateConfig->setSource( source_ , _particles.indices( ));

    if( resetState )
    {
      _updateConfig->setEmitted( _particles.indices( ), false );
      _updateConfig->setDead( _particles.indices( ), true );
    }
  }

  void Cluster::setModel( Model* model_ )
  {
    _updateConfig->setModel( model_, _particles.indices( ));
  }

  void Cluster::setUpdater( Updater* updater_ )
  {
    _updateConfig->setUpdater( updater_, _particles.indices( ));
  }


}


