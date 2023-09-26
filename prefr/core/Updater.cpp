/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/vg-lab/prefr>
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

  constexpr float invRandMax = 1.0f / RAND_MAX;

  Updater::Updater( void )
  : _updateConfig( nullptr )
  { }

  Updater::~Updater( )
  { }

  void Updater::updateParticle( tparticle current,
                                float deltaTime )
  {
    unsigned int id = current.id( );
    Source* source = _updateConfig->source( id );
    Model* model = _updateConfig->model( id );

    assert( model );
    assert( source );

//    std::cout << "updating particle " << id
//             << " " << current.alive( )
//             << " " << _updateConfig->dead( id )
//             << " " << _updateConfig->emitted( id )
//             << std::endl;

    if( _updateConfig->emitted( id ) && !current.alive( ))
    {
      current.set_life( glm::clamp( rand( ) * invRandMax, 0.0f, 1.0f ) *
                     model->_lifeRange + model->_minLife );

      current.set_alive( true );

      SampledValues values;
      values.index = current.id( );
      source->sample( &values );

      current.set_position( values.position );
      current.set_velocity( values.direction );

      _updateConfig->setEmitted( id, false );
    }
    else
    {
      current.set_life( current.life( ) - deltaTime );
    }

    if( current.alive( ))
    {
      if( current.life( ) <= 0.0f )
      {
        current.set_life( 0.0f );
        current.set_alive( false );

        _updateConfig->setDead( id, true );
      }

      const float refLife = 1.0f -
          glm::clamp( current.life( ) * ( model->_lifeNormalization ),
                      0.0f, 1.0f );

      current.set_color( model->color.GetValue( refLife ));

      current.set_size( model->size.GetValue( refLife ));
      current.set_velocityModule( model->velocity.GetValue( refLife ));

      current.set_position( current.position( ) + current.velocity( ) *
                             current.velocityModule( ) * deltaTime );
    }
  }
}
