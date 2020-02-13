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

#ifndef __PREFR__CLUSTER__
#define __PREFR__CLUSTER__

#include <prefr/api.h>
#include "Particles.h"
#include "Source.h"
#include "Model.h"
#include "Updater.h"

namespace prefr
{

  class Source;
  class Model;
  class Emitter;
  class Updater;

  /*! \brief This class groups sets of particles that share aspects.
   *
   * This class groups sets of particles that share common aspects such as
   * the Model, Source and Updater classes i.e. their behavior and properties
   * along the simulation. These properties can be modified during runtime,
   * so it will affect the whole set of particles.
   *
   * @see Model
   * @see Source
   * @see Updater
   * @see Particles
   *
   */
  class Cluster
  {
    friend class ParticleSystem;

  public:

    /*! \brief Default constructor.
     *
     * Default constructor.
     *
     */
    PREFR_API Cluster( void );

    /*! \brief Default destructor.
     *
     * Default destructor.
     *
     */
    PREFR_API virtual ~Cluster( void );

    /*! \brief Returns the set of particles compounding the cluster.
     *
     * Returns the set of particles compounding the cluster.
     *
     * @return Set of particles compounding the cluster.
     */
    PREFR_API ParticleRange& particles( void );

    /*! \brief Sets the range of particles compounding the cluster.
     *
     * Sets the range of particles compounding the cluster.
     *
     * @param particleArray Set of particles compounding the cluster.
     */
    PREFR_API void particles( const ParticleRange& particleArray );

    /*! \brief Gets the state of the cluster.
     *
     * Gets the state of the cluster.
     *
     * @return State of the cluster, true if active.
     */
    PREFR_API bool active( void ) const;

    /*! \brief Sets the state of the cluster.
     *
     * Sets the state of the cluster.
     *
     * @param active State of the cluster, true to set active.
     */
    PREFR_API void active( bool active );

    /*! \brief Returns the flag indicating if particles will be killed
     * when deactivated.
     *
     * Returns the flag indicating if particles will be killed when deactivated.
     *
     * @return Flag of killing particles when deactivated. True if particles
     * will be killed.
     */
    PREFR_API bool inactiveKillParticles( void ) const;

    /*! \brief Sets the the flag indicating if particles will be killed
     * when deactivated.
     *
     * Sets the flag indicating if particles will be killed when deactivated.
     *
     * @param killParticles Flag of killing particles when deactivated. True
     * if particles will be killed.
     */
    PREFR_API void inactiveKillParticles( bool killParticles );

    /*! This method kills current cluster particles.
     *
     * This method actively kills current cluster particles. This process
     * by default will reduce particles' life to zero and modify the Particles#_alive
     * flag.
     *
     * @param changeState
     */
    PREFR_API virtual void killParticles( bool changeState = true );


    virtual void setSource( Source* source_, bool resetState = true );
    virtual void setModel( Model* model_ );
    virtual void setUpdater( Updater* updater_ );

  protected:

    /*! Range of particles compounding the cluster. */
    ParticleCollection _particles;

    UpdateConfig* _updateConfig;

    /*!
     *  Number of alive particles set after ParticleSystem#Update step by
     *  default.
     */
    unsigned int _aliveParticles;


    /*! Flag indicating whether cluster is active or not. */
    bool _active;

    /*! Flag indicating whether cluster's particles will be killed if cluster
     * is set as inactive. */
    bool _inactiveKillParticles;
  };

  typedef VectorizedSet< Cluster* > ClustersArray;

}

#endif /* __PREFR__CLUSTER__ */
