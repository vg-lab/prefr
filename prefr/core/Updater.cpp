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

  void Updater::emitParticle( const Cluster& cluster,
                      const tparticle_ptr current )
  {
    if( current->alive( ))
      return;

    Source* source = cluster.source( );
    Model* model = cluster.model( );

    current->set_life( glm::clamp( rand( ) * invRandMax, 0.0f, 1.0f ) *
                   model->_lifeRange + model->_minLife );

    current->set_alive( true );

    SampledValues values;
    source->sample( &values );

    current->set_position( values.position );
    current->set_velocity( values.direction );

  }

  void Updater::updateParticle( const Cluster& cluster,
                        const tparticle_ptr current,
                        float deltaTime )
  {
    Source* source = cluster.source( );
    Model* model = cluster.model( );

    if( !source || !model )
      return;

    current->set_life( current->life( ) - deltaTime );

    if( current->alive( ) && current->life( ) <= 0.0f )
    {
      current->set_life( 0.0f );
      current->set_alive( false );
      #pragma omp critical
      {
        source->_deadParticles.push_back( current->id( ));
      }
    }

    if( current->alive( ))
    {

      float refLife = 1.0f -
          glm::clamp( current->life( ) * ( model->_lifeNormalization ),
                      0.0f, 1.0f );

      current->set_color( model->color.GetValue( refLife ));

      current->set_size( model->size.GetValue( refLife ));
      current->set_velocityModule( model->velocity.GetValue( refLife ));

      current->set_position( current->position( ) +
                         current->velocity( ) *
                         current->velocityModule( ) * deltaTime );

    }

  }

}
