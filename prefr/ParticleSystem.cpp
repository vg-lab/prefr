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
#include "Log.h"

#ifdef PREFR_USE_OPENMP
#include <omp.h>
#endif

namespace prefr
{

  ParticleSystem::ParticleSystem( unsigned int maxParticles )
  : _sorter( nullptr )
  , _renderer( nullptr )
  , _maxParticles ( maxParticles )
  , _renderDeadParticles( false )
  , _run( false )
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

  void ParticleSystem::AddCluster( Cluster* cluster,
                                   unsigned int start,
                                   unsigned int size_ )
  {

    assert( start + size_ <= _maxParticles );

    cluster->particles( ParticleRange( _particles, start, start + size_ ));

    _clusters.push_back( cluster );

    unsigned int reference = _clusters.size( ) - 1;

    auto clusterIT = _clusterReference.begin( ) + start;

    for( unsigned int i = 0; i < size_; i++ )
    {
      *clusterIT = reference;
      ++clusterIT;
    }

    if( cluster->source( ))
      cluster->source( )->InitializeParticles( );

  }

  void ParticleSystem::AddSource( Source* source )
  {
    assert( source );
    _sources.push_back( source );
  }

  void ParticleSystem::AddModel( Model* model )
  {
    assert( model );
    _models.push_back( model );
  }

  void ParticleSystem::AddUpdater( Updater* updater )
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

    _sorter->InitDistanceArray( );

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

    PREFR_DEBUG_CHECK( _sorter->_distances, "distances is null" );
    _renderer->_distances = _sorter->_distances;

    _renderer->init( );
  }

  Renderer* ParticleSystem::renderer( void ) const
  {
    return _renderer;
  }

  void ParticleSystem::Start()
  {
//    tparticle current = _particles.begin( );
//    for (unsigned int i = 0; i < aliveParticles; i++)
//    {
//      (*emitters)[particleEmitter[i]]->EmitFunction( &current, true);
//      current++;
//    }



    _run = true;

  }

  void ParticleSystem::Update( const float& deltaTime )
  {
    if( !_run )
      return;

    _aliveParticles = 0;

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )
#endif
    for( unsigned int s = 0; s < _sources.size( ); ++s )
    {
      Source* source = _sources[ s ];

      // Set source's elapsed delta
      source->PrepareFrame( deltaTime );
    }

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )
#endif
    for( unsigned int c = 0 ; c < _clusters.size( ); ++c )
    {
      Cluster* cluster = _clusters[ c ];
      Source* source = cluster->source( );

      if( source->Emits( ))
      {
        for( auto emittedParticle : source->_particlesToEmit )
        {
          tparticle particle = _particles.at( emittedParticle );
          cluster->updater( )->Emit( *cluster, &particle );
        }
      }
    }

    // For each cluster....
//    for( auto cluster : _clusters )
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for if( _parallel )
#endif
    for( unsigned int c = 0 ; c < _clusters.size( ); ++c )
    {
      Cluster* cluster = _clusters[ c ];
      cluster->aliveParticles = 0;
      // If active
      if( cluster->active( ))
      {

        // For each particle of the cluster...
        for( tparticle particle = cluster->particles( ).begin( );
             particle != cluster->particles( ).end( );
             particle++ )
        {
          // Update
          cluster->updater( )->Update( *cluster, &particle, deltaTime );

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
          cluster->KillParticles( );
        }
      }
    }

    for( Cluster* cluster : _clusters )
    {
      _aliveParticles += cluster->aliveParticles;
    }

    // For each source...
    for( Source* source : _sources )
    {
      // Finish frame
      source->CloseFrame( );
    }

    _sorter->_aliveParticles = _aliveParticles;
    _renderer->renderConfig( )->aliveParticles = _aliveParticles;

    std::cout << "Alive particles: " << _aliveParticles << std::endl;
  }

  void ParticleSystem::UpdateCameraDistances( const glm::vec3& cameraPosition )
  {
    _sorter->UpdateCameraDistance( cameraPosition, _renderDeadParticles );
  }

  void ParticleSystem::UpdateRender( )
  {
    _sorter->Sort();
    _renderer->SetupRender( );
  }

  void ParticleSystem::Render( ) const
  {
    _renderer->Paint( );
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

#ifdef PREFR_USE_OPENMP
    void ParticleSystem::parallel( bool parallelProcessing )
    {
      _parallel = parallelProcessing;
    }
#endif

}


