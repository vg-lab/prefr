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
  , _parallel( true )
#else
  , _parallel( false )
#endif
  {
    _particles.resize( _maxParticles );

    // Initialize used and unused particles.
    _used = ParticleCollection( _particles, ParticleIndices( ));
    _unused = ParticleCollection( _particles, _particles.begin( ), _particles.end( ) );

    _referenceModels.resize( _maxParticles, nullptr );
    _referenceSources.resize( _maxParticles, nullptr );
    _referenceUpdaters.resize( _maxParticles, nullptr );

    _flagsEmitted.resize( _maxParticles, false );
    _flagsDead.resize( _maxParticles, false );

    _updateConfig._refModels = &_referenceModels;
    _updateConfig._refSources = &_referenceSources;
    _updateConfig._refUpdaters = &_referenceUpdaters;

    _updateConfig._emitted = &_flagsEmitted;
    _updateConfig._dead = &_flagsDead;

    _updateConfig._used = &_used;
    _updateConfig._unused = &_unused;

    auto particle = _particles.begin( );
    for( unsigned int i = 0; i < _maxParticles; i++ )
    {
      particle.set_id( i );
      particle.set_alive( false );

      ++particle;
    }

    _aliveParticles = 0;
    _lastAlive = 0;
    _noVariationFrames = 0;
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

  void ParticleSystem::resize( unsigned int newSize )
  {
    _particles.resize( newSize );

    _referenceModels.resize( newSize, nullptr );
    _referenceSources.resize( newSize, nullptr );
    _referenceUpdaters.resize( newSize, nullptr );

    _flagsEmitted.resize( newSize, false );
    _flagsDead.resize( newSize, false );

  }

  void ParticleSystem::addCluster( Cluster* cluster,
                                   const ParticleSet& indices )
  {
    cluster->_updateConfig = &_updateConfig;
    cluster->particles( ParticleCollection( _particles, indices));

    _clusters.push_back( cluster );
  }

  void ParticleSystem::detachCluster( Cluster* cluster )
  {
    _clusters.remove( cluster );
  }

  void ParticleSystem::addSource( Source* source,
                                  const ParticleSet& indices )
  {
    assert( source );

    source->_updateConfig = &_updateConfig;

    source->_particles = ParticleCollection( _particles, indices );

    if( indices.size( ) > 0 )
    {
      for( unsigned int idx : indices )
      {
        if( !_unused.hasElement( idx ))
        {
          Source* previous = _referenceSources[ idx ];
          if( previous )
            previous->particles( ).removeIndex( idx );
        }

        _flagsDead[ idx ] = true;
        _flagsEmitted[ idx ] = false;

        _referenceSources[ idx ] = source;
      }

      _unused.removeIndices( source->particles( ).indices( ));
      _used.addIndices( indices );
    }

    _sources.push_back( source );

    source->active( true );

    source->_initializeParticles( );
  }

  void ParticleSystem::detachSource( Source* source )
  {
    assert( source );

    releaseParticles( source->particles( ));

    _sources.remove( source );

    source->active( false );
  }

  void ParticleSystem::addModel( Model* model )
  {
    assert( model );
    _models.push_back( model );
  }

  void ParticleSystem::detachModel( Model* model )
  {
    assert( model );
    _models.remove( model );

    for( auto const & particle : _used )
    {
      unsigned int idx = particle.id( );

      if( _referenceModels[ idx ] == model )
      {
        _referenceModels[ idx ] = nullptr;

        _flagsDead[ idx ] = false;
        _flagsEmitted[ idx ] = false;

        _used.removeIndex( idx );
        _unused.addIndex( idx );
      }
    }
  }

  void ParticleSystem::addUpdater( Updater* updater )
  {
    assert( updater );

    updater->_updateConfig = &_updateConfig;

    _updaters.push_back(updater);
  }

  void ParticleSystem::detachUpdater( Updater* updater )
  {
    assert( updater );

    for( auto const & particle : _used )
    {
      unsigned int idx = particle.id( );

      if( _referenceUpdaters[ idx ] == updater )
      {
        _referenceUpdaters[ idx ] = nullptr;

        _flagsDead[ idx ] = false;
        _flagsEmitted[ idx ] = false;

        _used.removeIndex( idx );
        _unused.addIndex( idx );
      }
    }
  }

  void ParticleSystem::sorter( Sorter* sorter_ )
  {
    assert( sorter_ );
    _sorter = sorter_;

    _sorter->particles( _particles );
    _sorter->sources( &_sourcesVec );

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

    _renderer->particles( _particles );

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
    if( !_run ) return;

    if( _lastAlive != 0  && _noVariationFrames >= 50 )
    {
      _run = false;
      return;

    }
     prepareFrame( deltaTime );

     updateFrame( deltaTime );

     finishFrame( );

     if( _lastAlive == _aliveParticles )
       _noVariationFrames++;
  }

  void ParticleSystem::prepareFrame( float deltaTime )
  {
    _aliveParticles = 0;


    _sourcesVec = _sources.vector( );
    _sorter->sources( &_sourcesVec );

#ifdef PREFR_USE_OPENMP

    #pragma omp parallel for if( _parallel )
    for( int s = 0; s < ( int ) _sources.size( ); ++s )
    {
      Source* source = _sourcesVec[ s ];
#else
    for( auto& source : _sources )
    {
#endif
      if( source->particles( ).empty( ) || !source->active( ))
        continue;

      // Set source's elapsed delta
      source->prepareFrame( deltaTime );
    }
  }

  void ParticleSystem::updateFrame( float deltaTime )
  {
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )
    for( int particleId = 0; particleId < ( int ) _used.size( ); ++particleId )
    {
      tparticle particle = _used[ particleId ];
#else
    for( auto particle : _used )
    {
#endif

      Source* source = _referenceSources[ particle.id( )];
      if( !source || !source->active( ) || source->particles( ).empty( ))
        continue;

      Updater* updater = _referenceUpdaters[ particle.id( )];
      if( updater )
        updater->updateParticle( particle, deltaTime );
    }
  }

  void ParticleSystem::finishFrame( void )
  {
    // For each source...
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )

    for( int s = 0; s < ( int ) _sources.size( ); ++s )
    {
      Source* source = _sourcesVec[ s ];
#else
    for( auto& source : _sources )
    {
#endif
      if( source->particles( ).empty( ) || !source->active( ))
        continue;

      // Finish frame
      source->closeFrame( );

      #pragma omp atomic
      _aliveParticles += source->aliveParticles( );

    }

    _sorter->_aliveParticles = _aliveParticles;
    _renderer->renderConfig( )->_aliveParticles = _aliveParticles;
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

  ParticleCollection ParticleSystem::createCollection( const ParticleSet& indices )
  {
    return ParticleCollection( _particles, indices );
  }

  void ParticleSystem::releaseParticles( const ParticleCollection& collection )
  {
    releaseParticles( collection.indices( ));
  }

  void ParticleSystem::releaseParticles( const ParticleSet& indices )
  {
    for( auto const & idx : indices )
    {
      _referenceModels[ idx ] = nullptr;
      _referenceSources[ idx ] = nullptr;
      _referenceUpdaters[ idx ] = nullptr;

      _flagsDead[ idx ] = false;
      _flagsEmitted[ idx ] = false;

    }

    _used.removeIndices( indices);
    _unused.addIndices( indices );
  }

  ParticleCollection ParticleSystem::retrieveUnused( unsigned int size )
  {
    if( size == 0  || size >= _maxParticles )
      size = _maxParticles;

    unsigned int count = 0;

    ParticleSet indices;

    for( auto particleId : _unused.indices( ) )
    {
      if( count >= size )
        break;

      indices.append( particleId );

      ++count;
    }
//
//    _unused.removeIndices( indices );
//    _used.addIndices( indices );

    return ParticleCollection( _particles, indices );
  }

  ParticleCollection ParticleSystem::retrieveActive( void )
  {
    return _used;
  }

  ParticleCollection ParticleSystem::particles( void )
  {
    return _particles;
  }
}
