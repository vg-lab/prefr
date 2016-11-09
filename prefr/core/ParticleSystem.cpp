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

#include "ParticleSystem.h"

#include "../utils/Log.h"

#ifdef PREFR_USE_OPENMP
#include <omp.h>
#endif

namespace prefr
{

  ParticleSystem::ParticleSystem( unsigned int maxParticles,
                                  ICamera* camera )
  : _sorter( nullptr )
  , _renderer( nullptr )
  , _maxParticles ( maxParticles )
  , _renderDeadParticles( false )
  , _run( false )
  , _camera( camera )
  , _useExternalCamera( camera ? true : false )
#ifdef PREFR_USE_OPENMP
  , _parallel( false )
#endif
  {

    _particles.resize( _maxParticles );

    _clusterReference.resize( _maxParticles, -1 );

    auto particle = _particles.begin( );
    for( unsigned int i = 0; i < _maxParticles; i++ )
    {
      particle.id( i );
      particle.alive( false );

      ++particle;
    }

    _aliveParticles = 0;
  }


  ParticleSystem::~ParticleSystem()
  {
    for( Source* source : _sources )
      delete( source );

    for( Model* model : _models )
      delete( model );

    for( Updater* updater : _updaters )
      delete( updater );

    for( Cluster* cluster : _clusters )
      delete( cluster );

    delete( _sorter );
    delete( _renderer );

  }

  void ParticleSystem::addCluster( Cluster* cluster,
                                   unsigned int start_,
                                   unsigned int size_ )
  {

    assert( start_ + size_ <= _maxParticles );

    cluster->particles( ParticleRange( _particles, start_, start_ + size_ ));

    _clusters.push_back( cluster );

    unsigned int reference = ( unsigned int ) _clusters.size( ) - 1;

    auto clusterIT = _clusterReference.begin( ) + start_;

    for( unsigned int i = 0; i < size_; i++ )
    {
      *clusterIT = reference;
      ++clusterIT;
    }

    if( cluster->source( ))
      cluster->source( )->_initializeParticles( );

  }

  void ParticleSystem::addSource( Source* source )
  {
    assert( source );
    _sources.push_back( source );
  }

  void ParticleSystem::addModel( Model* model )
  {
    assert( model );
    _models.push_back( model );
  }

  void ParticleSystem::addUpdater( Updater* updater )
  {
    assert( updater );
    _updaters.push_back(updater);
  }

  void ParticleSystem::sorter( Sorter* sorter_ )
  {
    assert( sorter_ );
    _sorter = sorter_;

    _sorter->clusters( &_clusters );
    _sorter->particles( ParticleRange( _particles ));

    _sorter->initDistanceArray( _camera );

#ifdef PREFR_USE_OPENMP
    _sorter->_parallel = _parallel;
#endif

  }

  Sorter* ParticleSystem::sorter( void ) const
  {
    return _sorter;
  }

  void ParticleSystem::renderer( Renderer* renderer_ )
  {
    assert( renderer_ );
    _renderer = renderer_ ;

    _renderer->particles( ParticleRange( _particles ));

    _renderer->_init( );

    assert( _sorter->_distances );
    _renderer->distanceArray( _sorter->_distances );

#ifdef PREFR_USE_OPENMP
    _renderer->_parallel = _parallel;
#endif
  }

  Renderer* ParticleSystem::renderer( void ) const
  {
    return _renderer;
  }

  void ParticleSystem::start()
  {
    _run = true;

  }

  void ParticleSystem::update( const float& deltaTime )
  {
    if( !_run )
      return;

    _aliveParticles = 0;

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )

    for( int s = 0; s < ( int ) _sources.size( ); ++s )
    {
      Source* source = _sources[ s ];
#else
    for( auto source : _sources )
    {
#endif
      // Set source's elapsed delta
      source->prepareFrame( deltaTime );
    }

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )
    for( int c = 0 ; c < ( int ) _clusters.size( ); ++c )
    {
      Cluster* cluster = _clusters[ c ];
#else
    for( auto cluster : _clusters )
    {
#endif
      Source* source = cluster->source( );

      if( source->emits( ))
      {
        for( auto emittedParticle : source->_particlesToEmit )
        {
          tparticle particle = _particles.at( emittedParticle );
          cluster->updater( )->emit( *cluster, &particle );
        }
      }
    }

    // For each cluster....
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )
    for( int c = 0 ; c < ( int ) _clusters.size( ); ++c )
    {
      Cluster* cluster = _clusters[ c ];
#else
    for( auto cluster : _clusters )
    {
#endif
      cluster->aliveParticles = 0;
      // If active
      if( cluster->active( ))
      {

        // For each particle of the cluster...
        for( tparticle particle = cluster->particles( ).begin( );
             particle != cluster->particles( ).end( );
             ++particle )
        {
          // Update
          cluster->updater( )->update( *cluster, &particle, deltaTime );

          cluster->aliveParticles += particle.alive( );

        }
      }
      // Else
      else
      {
        // If kill particles...
        if( cluster->inactiveKillParticles( ))
        {
          // Kill particles
          cluster->killParticles( );
        }
      }
    }

    for( Cluster* cluster : _clusters )
    {
      _aliveParticles += cluster->aliveParticles;
    }

    // For each source...
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )

    for( int s = 0; s < ( int ) _sources.size( ); ++s )
    {
      Source* source = _sources[ s ];
#else
    for( auto source : _sources )
    {
#endif
      // Finish frame
      source->closeFrame( );
    }

    _sorter->_aliveParticles = _aliveParticles;
    _renderer->renderConfig( )->aliveParticles = _aliveParticles;

  }

  void ParticleSystem::updateCameraDistances( const glm::vec3& cameraPosition )
  {
    if( _run )
      _sorter->updateCameraDistance( cameraPosition, _renderDeadParticles );
  }

  void ParticleSystem::updateCameraDistances( void )
  {
    if( _run )
        _sorter->updateCameraDistance( _renderDeadParticles );
  }

  void ParticleSystem::updateRender( )
  {
    if( _run )
    {
      _sorter->sort( );
      _renderer->setupRender( );
    }
  }

  void ParticleSystem::render( ) const
  {
    if( _run )
      _renderer->paint( );
  }

  void ParticleSystem::run( bool run_ )
  {
    _run = run_;
  }

  bool ParticleSystem::run( void ) const
  {
    return _run;
  }

  unsigned int ParticleSystem::aliveParticles( void ) const
  {
    return _aliveParticles;
  }

  void ParticleSystem::renderDeadParticles( bool renderDead )
  {
    _renderDeadParticles = renderDead;
  }

  void ParticleSystem::parallel( bool parallelProcessing )
  {
    _parallel = parallelProcessing;

    if( _sorter )
      _sorter->_parallel = parallelProcessing;

    if( _renderer )
      _renderer->_parallel = parallelProcessing;
  }

  const ClustersArray& ParticleSystem::clusters( void ) const
  {
    return _clusters;
  }

}


