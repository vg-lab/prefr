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
  : _updateConfig( nullptr )
  , _aliveParticles( 0 )
  , _active( true )
  , _inactiveKillParticles( false )
  { }

  Cluster::~Cluster( void )
  { }

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
    for( auto particle : _particles )
    {
      particle.set_life( 0.0f );

      if( changeState )
      {
        particle.set_alive( false );

        _updateConfig->setDead( particle.id( ), true );
//        ( *_updateConfig->_dead )[ particle.id( )] = true;
      }
    }
  }

  void Cluster::setSource( Source* source_ )
  {
//    for( auto particle : _particles )
//    {
//      Source* source = _updateConfig->source( particle.id( ));
//      if( source )
//      {
//        source->particles( ).removeIndex( particle.id( ));
//      }
//
////      ( *_updateConfig->_refSources )[ particle.id( )] = source_;
//    }
//
//    _updateConfig->setSource( source_, )
//    source_->_particles.addIndices( _particles.indices( ));
    _updateConfig->setSource( source_ , _particles.indices( ));
  }

  void Cluster::setModel( Model* model_ )
  {
//    for( auto particle : _particles )
//    {
//      ( *_updateConfig->_refModels )[ particle.id( )] = model_;
//    }
    _updateConfig->setModel( model_, _particles.indices( ));
  }

  void Cluster::setUpdater( Updater* updater_ )
  {
//    for( auto particle : _particles )
//    {
//      ( *_updateConfig->_refUpdaters )[ particle.id( )] = updater_;
//    }
    _updateConfig->setUpdater( updater_, _particles.indices( ));
  }


}


