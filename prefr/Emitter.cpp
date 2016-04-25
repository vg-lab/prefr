/*
 * DefaultParticleEmitter.cpp
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */
#include "Emitter.h"

namespace prefr
{

//    static float invRandMax = 1.0f / RAND_MAX;

    //**********************************************************
    // Default Emitter
    //**********************************************************


    Emitter::Emitter ( float _emissionRate, bool _loop)
    : particlesBudget( 0 )
    , emissionRate( _emissionRate )
    , loop( _loop )
    , active( true )
    {

    }

    Emitter::~Emitter()
    {

    }


//
//    int Emitter::EmitSingle(const Cluster& cluster,
//                            const tparticle_ptr current)
//    {
////      if (!active)
////        return 0;
////
////      Source* source = cluster.source( );
////
////      if (source->Emits( ) && !current->alive( ))
////      {
////        this->EmitFunction( cluster, current);
////
////      }
////
////      // This might be used as signal to stop looping through this emitter, returning zero after the last particle emitted.
////      return source->_particlesBudget;
//
//      return 0;
//    }
//
//    void Emitter::Emit( const Cluster& cluster,
//                        const tparticle_ptr current,
//                        bool override )
//    {
//
////      Source* source = cluster.source( );
////      Model* model = cluster.model( );
////
////      if (!source || !model)
////        return;
////
////      if ((!current->alive() || override))
////      {
////       current->life( glm::clamp(rand() * invRandMax, 0.0f, 1.0f) *
////           model->lifeInterval + model->minLife );
////
////       current->velocity( source->GetEmissionVelocityDirection( ));
////       current->position( source->GetEmissionPosition( ));
////
////       current->velocityModule( model->velocity.GetFirstValue( ));
////       current->color( model->color.GetFirstValue( ));
////       current->size( model->size.GetFirstValue( ));
////
////       current->newborn( true );
////
////       source->ReduceBudgetBy(1);
////     }
//    }


}



