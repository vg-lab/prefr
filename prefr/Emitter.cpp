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



