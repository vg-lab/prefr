/*
 * ParticleSystem.cpp
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#include "ParticleSystem.h"
#include "log.h"

 #ifdef _OPENMP
 # include <omp.h>
 #endif

namespace prefr
{

  ParticleSystem::ParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop)
  : _sorter( nullptr )
  , _renderer( nullptr )
  , maxParticles (_maxParticles)
  , loop(_loop)
  , renderDeadParticles( false )
  , run( false )
  {

    if (initialParticlesNumber > maxParticles)
      initialParticlesNumber = maxParticles;

    _particles.resize( _maxParticles );

    _clusterReference.resize( maxParticles, -1 );

    auto particle = _particles.begin( );
    for( unsigned int i = 0; i < maxParticles; i++ )
    {
      particle.id( i );
      particle.alive( false );

//      if( i < initialParticlesNumber )
//        particle.alive( true );

      particle++;
    }

    _aliveParticles = initialParticlesNumber;
  }


  ParticleSystem::~ParticleSystem()
  {
    for( Source* emissionNode : _sources )
      delete( emissionNode );

//    delete( _sources );

    for( Model* prototype : prototypes )
      delete( prototype );

//    delete( prototypes);

    for( Emitter* emitter : emitters )
      delete( emitter );

//    delete( emitters );

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

  }

  void ParticleSystem::AddEmissionNode( Source* node )
  {
    this->_sources.push_back( node );
  }

  void ParticleSystem::AddPrototype(Model* prototype)
  {
    this->prototypes.push_back(prototype);
  }

  void ParticleSystem::AddEmitter(Emitter* /*emitter*/)
  {
//    this->emitters->push_back(emitter);
//
//    for (int i = start; i < end; i++)
//    {
//      this->particleEmitter[i] = size-1;
//    }


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

  void ParticleSystem::Update(float deltaTime )
  {
    if( !run )
     return;

    unsigned int i = 0;

    // Set emitter delta time to calculate the number of particles to emit this frame
    for( Source* source : _sources )
    {
      source->PrepareFrame( deltaTime );
    }

    #pragma omp parallel for
    // For each particle...
    for( unsigned int c = 0; c < _particles.numParticles( ); ++c )
    {
      tparticle particle = _particles.at( c );

      int ref = _clusterReference[ particle.id( )];
      if( ref < 0 )
      {
        std::cout << "Cluster reference not found " << ref << std::endl;
        continue;
      }
      Cluster* cluster = _clusters[ ref ];

      assert( cluster );

      if( cluster->active( ))
      {

        if( !particle.alive( ) && cluster->source( )->Emits( ))
        {
          cluster->updater( )->Emit( *cluster, &particle );

          if( particle.alive( ))
          {
            #pragma omp atomic
            cluster->source( )->_particlesBudget -= 1;
          }
        }

        // Update
        cluster->updater( )->Update( *cluster, &particle, deltaTime );

        #pragma omp atomic
        i += particle.alive( );

      }
      else
      {
        // If kill particles...
        if( cluster->inactiveKillParticles( ))
        {
          // Kill particles
          cluster->KillParticles( );
        }
      }

      // For each source...
      for( Source* source : _sources )
      {
       // Finish frame
       source->CloseFrame( );
      }

      this->_aliveParticles = i;

    }
  }

  void ParticleSystem::UpdateUnified( const float& deltaTime)
  {
    if( !run )
      return;

    unsigned int i = 0;

    // Set emitter delta time to calculate the number of particles to emit this frame
//    for( Source* source : _sources )
    #pragma omp parallel for
    for( unsigned int s = 0; s < _sources.size( ); ++s )
    {
      Source* source = _sources[ s ];
      source->PrepareFrame( deltaTime );
    }

    // For each cluster....
//    for( auto cluster : _clusters )
    #pragma omp parallel for
    for( unsigned int c = 0 ; c < _clusters.size( ); ++c )
    {
      Cluster* cluster = _clusters[ c ];
      // If active
      if( cluster->active( ))
      {

        // For each particle of the cluster...
        for( tparticle particle = cluster->particles( ).begin( );
             particle != cluster->particles( ).end( );
             particle++ )
        {
          if( !particle.alive( ) && cluster->source( )->Emits( ))
          {
            cluster->updater( )->Emit( *cluster, &particle );
          }

          // Update
          cluster->updater( )->Update( *cluster, &particle, deltaTime );

          #pragma omp atomic
          i += particle.alive( );
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

    // For each source...
    for( Source* source : _sources )
    {
      // Finish frame
      source->CloseFrame( );
    }

    this->_aliveParticles = i;
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


