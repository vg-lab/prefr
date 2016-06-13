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

  ParticleSystem::ParticleSystem( unsigned int _maxParticles )
  : _sorter( nullptr )
  , _renderer( nullptr )
  , maxParticles (_maxParticles)
  , renderDeadParticles( false )
  , run( false )
  {

    _particles.resize( _maxParticles );

    _clusterReference.resize( maxParticles, -1 );

    auto particle = _particles.begin( );
    for( unsigned int i = 0; i < maxParticles; i++ )
    {
      particle.id( i );
      particle.alive( false );

//      if( i < initialParticlesNumber )
//        particle.alive( true );

      ++particle;
    }

    _aliveParticles = 0;
  }


  ParticleSystem::~ParticleSystem()
  {
    for( Source* emissionNode : _sources )
      delete( emissionNode );

//    delete( _sources );

    for( Model* prototype : prototypes )
      delete( prototype );

    for( Updater* updater : updaters )
      delete( updater );

//    delete( updaters );
    delete( _sorter );
    delete( _renderer );

//    for( tparticleContainer::iterator it = particles->start; it != particles->end; it++)
//      delete( *it );

//    delete( particles );
  }

  void ParticleSystem::AddCluster( Cluster* cluster,
                                   unsigned int start,
                                   unsigned int size_ )
  {

    assert( start + size_ <= maxParticles );

    cluster->particles( ParticleRange( this->_particles, start, start + size_ ));

    this->_clusters.push_back( cluster );

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

  void ParticleSystem::AddEmissionNode( Source* node )
  {
    this->_sources.push_back( node );
  }

  void ParticleSystem::AddPrototype(Model* prototype)
  {
    this->prototypes.push_back(prototype);
  }

  void ParticleSystem::AddUpdater(Updater* updater)
  {
    this->updaters.push_back(updater);
  }

  void ParticleSystem::sorter( Sorter* sorter_ )
  {
    this->_sorter = sorter_;

    this->_sorter->clusters( &_clusters );
    this->_sorter->particles( ParticleRange( _particles ));

    this->_sorter->InitDistanceArray( );

  }

  Sorter* ParticleSystem::sorter( void )
  {
    return _sorter;
  }

  void ParticleSystem::renderer( Renderer* renderer_ )
  {
    this->_renderer = renderer_ ;
    this->_renderer->particles( ParticleRange( _particles ));

    PREFR_DEBUG_CHECK( this->_sorter->distances, "distances is null" );
    this->_renderer->distances = this->_sorter->distances;

    this->_renderer->init( );
  }

  Renderer* ParticleSystem::renderer( void )
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



    run = true;

  }

  void ParticleSystem::Update( const float& deltaTime)
  {
    if( !run )
      return;

//    unsigned int i = 0;
    this->_aliveParticles = 0;
    // Set emitter delta time to calculate the number of particles to emit this frame
//    for( Source* source : _sources )
#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for
#endif
    for( unsigned int s = 0; s < _sources.size( ); ++s )
    {
      Source* source = _sources[ s ];
      source->PrepareFrame( deltaTime );
    }

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for
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
    #pragma omp parallel for
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
//          if( !particle.alive( ) && cluster->source( )->Emits( ))
//          {
//            cluster->updater( )->Emit( *cluster, &particle );
//          }

          // Update
          cluster->updater( )->Update( *cluster, &particle, deltaTime );

          cluster->aliveParticles += particle.alive( );

//          #pragma omp atomic
//          i += particle.alive( );
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
      this->_aliveParticles += cluster->aliveParticles;
    }

    this->sorter( )->_aliveParticles = _aliveParticles;

    // For each source...
    for( Source* source : _sources )
    {
      // Finish frame
      source->CloseFrame( );
    }

//    this->_aliveParticles = i;
  }

  void ParticleSystem::UpdateCameraDistances(const glm::vec3& cameraPosition)
  {

    this->_sorter->UpdateCameraDistance( cameraPosition, renderDeadParticles );

  }

  void ParticleSystem::UpdateRender()
  {
   this->_sorter->Sort();

   this->_renderer->SetupRender(this->_aliveParticles);
  }

  void ParticleSystem::Render() const
  {
   this->_renderer->Paint(_aliveParticles);
  }

  void ParticleSystem::Run( bool run_ )
  {
    run = run_;
  }

  bool ParticleSystem::Run( void )
  {
    return run;
  }

  unsigned int ParticleSystem::aliveParticles( void )
  {
    return _aliveParticles;
  }

}


