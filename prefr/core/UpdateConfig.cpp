/*
 * @file  UpdateConfig.cpp
 * @brief
 * @author Sergio E. Galindo <sergio.galindo@urjc.es>
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 *          Do not distribute without further notice.
 */

#include "UpdateConfig.h"

#include <map>
#include "Source.h"

namespace prefr
{

  UpdateConfig::UpdateConfig( void )
  : _refModels( nullptr )
  , _refSources( nullptr )
  , _refUpdaters( nullptr )
  , _emitted( nullptr )
  , _dead( nullptr )
  { }

  UpdateConfig::~UpdateConfig( void )
  { }

  bool UpdateConfig::emitted( unsigned int idx ) const
  {
    assert( idx < _emitted->size( ));
    return (*_emitted)[ idx ];
  }

  void UpdateConfig::setEmitted( unsigned int idx, bool value )
  {
    assert( idx < _emitted->size( ));
    (* _emitted)[ idx ] = value;
  }


  bool UpdateConfig::dead( unsigned int idx ) const
  {
    assert( idx < _dead->size( ));
    return (*_dead)[ idx ];
  }

  void UpdateConfig::setDead( unsigned int idx, bool value )
  {
    assert( idx < _dead->size( ));
    (* _dead)[ idx ] = value;
  }

  Model* UpdateConfig::model( unsigned int idx ) const
  {
    assert( idx < _refModels->size( ));
    return (*_refModels)[ idx ];
  }

  void UpdateConfig::setModel( Model* model_, ParticleIndices indices )
  {
    for( auto idx : indices )
    {
      (*_refModels)[ idx ] = model_;
    }
  }

  Source* UpdateConfig::source( unsigned int idx ) const
  {
    assert( idx < _refSources->size( ));
    return (*_refSources)[ idx ];
  }

  void UpdateConfig::setSource( Source* source_, ParticleIndices indices )
  {
    std::set< Source* > sources;
    std::multimap< Source*, unsigned int > sourceIds;

    for( auto idx : indices )
    {
      // Remove indices from original source.
      //TODO Check algorithm efficiency.
      Source* auxSource = source( idx );
      auxSource->particles( ).removeIndex( idx );
      // Change source reference.
      (*_refSources )[ idx ] = source_;
    }

  }

  Updater* UpdateConfig::updater( unsigned int idx ) const
  {
      assert( idx < _refUpdaters->size( ));
      return (*_refUpdaters)[ idx ];
  }

  void UpdateConfig::setUpdater( Updater* updater_, ParticleIndices indices )
  {
    for( auto idx : indices )
    {
      (*_refUpdaters)[ idx ] = updater_;
    }
  }



}

