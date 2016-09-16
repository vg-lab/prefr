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
#include "Updater.h"

namespace prefr
{

  static float invRandMax = 1.0f / RAND_MAX;

  Updater::Updater( void )
  { }

  Updater::~Updater( )
  { }

  void Updater::Emit( const Cluster& cluster,
                      const tparticle_ptr current )
  {
    Source* source = cluster.source( );
    Model* model = cluster.model( );

    current->life( glm::clamp( rand( ) * invRandMax, 0.0f, 1.0f ) *
                   model->_lifeInterval + model->_minLife );

    current->alive( true );

    SampledValues values;
    source->sample( &values );

    current->position( values.position );
    current->velocity( values.direction );

  }

  void Updater::Update( const Cluster& cluster,
                        const tparticle_ptr current,
                        float deltaTime )
  {
    Source* source = cluster.source( );
    Model* model = cluster.model( );

    if( !source || !model )
      return;

    current->life( current->life( ) - deltaTime );

    if( current->alive( ) && current->life( ) < 0.0f )
    {
      current->life( 0.0f );
      #pragma omp critical
      {
        source->_deadParticles.push_back( current->id( ));
      }
    }

    current->alive( current->life( ) > 0 );

    if( current->alive( ))
    {

      float refLife = 1.0f -
          glm::clamp( current->life( ) * ( model->_lifeNormalization ),
                      0.0f, 1.0f );

      current->color( model->color.GetValue( refLife ));

      current->size( model->size.GetValue( refLife ));
      current->velocityModule( model->velocity.GetValue( refLife ));

      current->position( current->position( ) +
                         current->velocity( ) *
                         current->velocityModule( ) * deltaTime );

    }

  }

}
