/*
 * Copyright (c) 2014-2020 GMRV/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/gmrvvis/prefr>
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
#include "UpdateConfig.h"

namespace prefr
{

  class Cluster;

  /*! \class Updater
   *  \brief This class emits particles and updates their state over
   *  the execution.
   *
   *  Updater class is in charge of both, emit and update the particles
   *  along the system execution. For this, both Emit and Update methods
   *  can be re-implemented to fit the desired behavior. Both Updater#Emit and
   *  Updater#Update will be called from ParticleSystem#Update method each
   *  frame. Assign an Updater object to a Cluster to use it as the updater
   *  for cluster's particles.
   *
   *  @see ParticleSystem
   *  @see Particles
   *  @see Source
   *  @see Model
   *
   *  */
  class Updater
  {
    friend class ParticleSystem;

  public:

    /*! \brief Default constructor.
     *
     * Default constructor.
     *  */
    PREFR_API Updater( void );

    /*! \brief Default destructor.
     *
     * Default destructor.
     */
    PREFR_API virtual ~Updater( );

    /*! \brief Emit and Update method.
     *
     *  This method emits (when appropriate) and updates the state of particles
     *  each frame taking into account the Model established properties. This is,
     *  establishing their life values (and setting them as alive/dead if life
     *  is lower than 0) as well as other properties such as position, color,
     *  velocity, size and so on.
     *
     *  Re-implement for adding different behaviors as well as updating
     *  other non-default properties added or re-implemented Model objects.
     *
     * @param cluster Cluster owner of the particle to be updated.
     * @param current tparticle_ptr referencing the particle to be emitted/updated.
     * @param deltaTime Current delta time to compute attributes variations
     * according to elapsed time since last frame.
     */
    PREFR_API virtual void updateParticle( tparticle current,
                                           float deltaTime );

  protected:

    UpdateConfig* _updateConfig;
  };

  typedef VectorizedSet< Updater* > UpdatersArray;
}

#endif /* PARTICLEUPDATER_H_ */
