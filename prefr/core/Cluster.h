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
#ifndef __PREFR__CLUSTER__
#define __PREFR__CLUSTER__

#include <prefr/api.h>
#include "Particles.h"
#include "Source.h"
#include "Model.h"
#include "Updater.h"

#include "../utils/ElementCollection.hpp"

namespace prefr
{

  class Source;
  class Model;
  class Emitter;
  class Updater;

  class Cluster
  {

  public:

    typedef std::pair< unsigned int, unsigned int > TBoundsUI;
    typedef std::pair< Particles::iterator, Particles::iterator > TBoundsIt;

    PREFR_API Cluster( );

    PREFR_API virtual ~Cluster( void );

    PREFR_API Source* source( void ) const;
    PREFR_API void source( Source* source );

    PREFR_API Model* model( void ) const;
    PREFR_API void model( Model* model );

    PREFR_API Updater* updater( void ) const;
    PREFR_API void updater( Updater* updater);

    PREFR_API ParticleRange particles( void ) const;
    PREFR_API void particles( const ParticleRange& particleArray );

    PREFR_API bool active( void ) const;
    PREFR_API void active( bool active_ );

    PREFR_API bool inactiveKillParticles( void ) const;
    PREFR_API void inactiveKillParticles( bool killParticles );

    virtual void killParticles( void );

    unsigned int aliveParticles;

  protected:

    ParticleRange _particlesArray;

    unsigned int _size;

    Source* _source;

    Model* _model;

    Updater* _updater;

    bool _active;

    bool _inactiveKillParticles;
  };

  typedef std::vector< Cluster* > ClustersArray;

}

#endif /* __PREFR__CLUSTER__ */
