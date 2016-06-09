/*
 * DefaultParticleUpdater.cpp
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */

#include "Updater.h"

namespace prefr
{

  static float invRandMax = 1.0f / RAND_MAX;

  Updater::Updater( void )
  {}

  Updater::~Updater()
  {

  }

  void Updater::Emit( const Cluster& cluster,
                      const tparticle_ptr current )
  {
    Source* source = cluster.source( );
    Model* model = cluster.model( );

    current->life( glm::clamp(rand() * invRandMax, 0.0f, 1.0f) *
                   model->lifeInterval + model->minLife );

    current->alive( true );
    current->newborn( true );

    current->position( source->GetEmissionPosition( ));
    current->velocity( source->GetEmissionVelocityDirection( ));

//    source->ReduceBudgetBy( 1 );
  }

  void Updater::Update( const Cluster& cluster,
                        const tparticle_ptr current,
                        float deltaTime )
  {
    Source* source = cluster.source( );
    Model* model = cluster.model( );

    if (!source || !model)
      return;

    current->life( current->life( ) - deltaTime );

        if( current->alive( ) && current->life( ) < 0.0f)
    {
      current->life( 0.0f );
      #pragma omp critical
      {
        source->_deadParticles.push_back( current->id( ) );
      }
    }

    current->alive( ( current->life( ) > 0) );

    if (current->alive( ) /*&& !current->newborn( )*/)
    {

      float refLife = 1.0f - glm::clamp(( current->life( ) ) * (model->lifeNormalization), 0.0f, 1.0f);

      current->color( model->color.GetValue( refLife ));

      current->size( model->size.GetValue( refLife ));
      current->velocityModule( model->velocity.GetValue( refLife ));

      current->position( current->position( ) +
                         current->velocity( ) *
                         current->velocityModule( ) * deltaTime );

    }

    current->newborn( false );

  }

}
