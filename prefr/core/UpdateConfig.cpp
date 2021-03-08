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
  , _used( nullptr )
  , _unused( nullptr )
  { }

  UpdateConfig::~UpdateConfig( void )
  { }

  bool UpdateConfig::emitted( unsigned int idx ) const
  {
    assert( idx < _emitted->size( ));
    return ( *_emitted )[ idx ];
  }

  void UpdateConfig::setEmitted( unsigned int idx, bool value )
  {
    assert( idx < _emitted->size( ));
    ( *_emitted )[ idx ] = value;
  }

  void UpdateConfig::setEmitted( const ParticleSet& indices, bool value )
  {
    for( auto idx : indices )
    {
      assert( idx < _emitted->size( ));
      ( *_emitted )[ idx ] = value;
    }
  }

  bool UpdateConfig::dead( unsigned int idx ) const
  {
    assert( idx < _dead->size( ));
    return ( *_dead )[ idx ];
  }

  void UpdateConfig::setDead( unsigned int idx, bool value )
  {
    assert( idx < _dead->size( ));
    ( *_dead )[ idx ] = value;
  }

  void UpdateConfig::setDead( const ParticleSet& indices, bool value )
  {
    for( auto idx : indices )
    {
      assert( idx < _dead->size( ));
      ( *_dead )[ idx ] = value;
    }
  }

  Model* UpdateConfig::model( unsigned int idx ) const
  {
    assert( idx < _refModels->size( ));
    return ( *_refModels )[ idx ];
  }

  void UpdateConfig::setModel( Model* model_, const ParticleSet& indices )
  {
    for( auto idx : indices )
    {
      ( *_refModels )[ idx ] = model_;
    }
  }

  Source* UpdateConfig::source( unsigned int idx ) const
  {
    assert( idx < _refSources->size( ));
    return ( *_refSources )[ idx ];
  }

  void UpdateConfig::setSource( Source* source_, const ParticleSet& indices )
  {
    assert( source_ );

    std::set< Source* > sources;

    for( auto idx : indices )
    {
      // Remove indices from original source.
      //TODO Check algorithm efficiency.
      Source* auxSource = source( idx );
//      assert( auxSource );
      if( auxSource )
        sources.insert( auxSource );
//        auxSource->particles( ).removeIndex( idx );
        // Change source reference.

      ( *_refSources )[ idx ] = source_;

    }

    for( auto s : sources )
      s->particles( ).removeIndices( indices );

    _used->addIndices( indices );
    _unused->removeIndices( indices );

    source_->particles( ).addIndices( indices );

  }

  void UpdateConfig::removeSourceIndices( Source* source_,
                                          const ParticleSet& indices )
  {
    for( auto idx : indices )
    {
//      Source* auxSource = source( idx );
//      if( auxSource == source_ )
      {
        (*_refSources )[ idx ] = nullptr;
        setEmitted( idx, false );
        setDead( idx, true );
      }
    }

    _used->removeIndices( indices );
    _unused->addIndices( indices );

    source_->particles( ).removeIndices( indices );
  }

  Updater* UpdateConfig::updater( unsigned int idx ) const
  {
      assert( idx < _refUpdaters->size( ));
      return (*_refUpdaters)[ idx ];
  }

  void UpdateConfig::setUpdater( Updater* updater_, const ParticleSet& indices )
  {
    for( auto idx : indices )
    {
      (*_refUpdaters)[ idx ] = updater_;
    }
  }



}

