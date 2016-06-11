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
#ifndef PARTICLEUPDATER_H_
#define PARTICLEUPDATER_H_

#include <type_traits>

#include <prefr/api.h>

#include "Particles.h"

#include "Cluster.h"
#include "Model.h"
#include "Source.h"

namespace prefr
{

  class Cluster;

  //! This class updates the particles through the particle life cycle.
  /*!
   *
   */
  class Updater
  {


public:

    PREFR_API Updater( void );

    PREFR_API virtual ~Updater();

    PREFR_API virtual void Emit( const Cluster& cluster,
                                 const tparticle_ptr current );

    //! Update a single particle by the given particle collection index
    /*!
     *
     */
    PREFR_API virtual void Update( const Cluster& cluster,
                                   const tparticle_ptr current,
                                   float deltaTime );
protected:

    template < class T >
//    typename std::enable_if<std::is_base_of< ParticlePrototype, T>::value, void>::type
    void UpdateLife( const T& model, const tparticle_ptr current );

  };
}



#endif /* PARTICLEUPDATER_H_ */
