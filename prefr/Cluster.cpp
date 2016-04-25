/*
 * @file	Cluster.cpp
 * @brief
 * @author Sergio E. Galindo <sergio.galindo@urjc.es> 
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 *					Do not distribute without further notice.
 */

#include "Cluster.h"

namespace prefr
{

  Cluster::Cluster( )
  : _particlesArray( )
  , _size( 0 )
  , _source( nullptr )
  , _model( nullptr )
  , _emitter( nullptr )
  , _updater( nullptr )
  , _active( false )
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
  }

  Model* Cluster::model( void ) const
  {
    return _model;
  }

  void Cluster::model( Model* model_ )
  {
    _model = model_;
  }


  Emitter* Cluster::emitter( void ) const
  {
    return _emitter;
  }

  void Cluster::emitter( Emitter* emitter_ )
  {
    _emitter = emitter_;
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
    return _particlesArray;
  }

  void Cluster::particles( const ParticleRange& particleArray )
  {
    _particlesArray = particleArray;
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

  void Cluster::inactiveKillParticles( bool killParticles )
  {
    _inactiveKillParticles = killParticles;
  }

  void Cluster::KillParticles( void )
  {
    for( tparticle particle = _particlesArray.begin( );
         particle != _particlesArray.end( );
         particle++ )
    {
      particle.life( 0.0f );
      particle.alive( false );
    }
  }

}


