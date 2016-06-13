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

#include "Particles.h"
#include "Source.h"
#include "ElementCollection.hpp"
#include "Model.h"
#include "Updater.h"

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

    Cluster( );

    virtual ~Cluster( void );

    Source* source( void ) const;
    void source( Source* source );

    Model* model( void ) const;
    void model( Model* model );

    Updater* updater( void ) const;
    void updater( Updater* updater);

    ParticleRange particles( void ) const;
    void particles( const ParticleRange& particleArray );

    bool active( void ) const;
    void active( bool active_ );

    bool inactiveKillParticles( void ) const;
    void inactiveKillParticles( bool killParticles );

    virtual void KillParticles( void );

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
