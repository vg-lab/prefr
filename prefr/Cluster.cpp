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
  : _size( 0 )
  , _source( nullptr )
  , _model( nullptr )
  , _emitter( nullptr )
  , _updater( nullptr )
  , _active( false )
  { }

  Source* Cluster::source( void )
  {
    return _source;
  }

  void Cluster::source( Source* source_ )
  {
    _source = source_;
  }

  ParticlePrototype* Cluster::model( void )
  {
    return _model;
  }

  void Cluster::model( ParticlePrototype* model_ )
  {
    _model = model_;
  }


  Emitter* Cluster::emitter( void )
  {
    return _emitter;
  }

  void Cluster::emitter( Emitter* emitter_ )
  {
    _emitter = emitter_;
  }

  ParticleUpdater* Cluster::updater( void )
  {
    return _updater;
  }

  void Cluster::updater( ParticleUpdater* updater_)
  {
    _updater = updater_;
  }

  ParticleRange Cluster::particles( void )
  {
    return _particlesArray;
  }

  void Cluster::particles( const ParticleRange& particleArray )
  {
    _particlesArray = particleArray;
  }

  bool Cluster::active( void )
  {
    return _active;
  }

  void Cluster::active( bool active_ )
  {
    _active = active_;
  }


}


