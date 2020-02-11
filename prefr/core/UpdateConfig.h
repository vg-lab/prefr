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

#ifndef __PREFR_UPDATECONFIG__
#define __PREFR_UPDATECONFIG__

#include <vector>
#include "Particles.h"

namespace prefr
{
  class Model;
  class Source;
  class Updater;

  typedef std::vector< bool > FlagsArray;

  class UpdateConfig
  {
    friend class ParticleSystem;

  public:

    virtual ~UpdateConfig( void );

    bool emitted( unsigned int idx ) const;
    void setEmitted( unsigned int idx, bool value );
    void setEmitted( const ParticleSet& indices, bool value );

    bool dead( unsigned int idx ) const;
    void setDead( unsigned int idx, bool value );
    void setDead( const ParticleSet& indices, bool value );

    Model* model( unsigned int idx ) const;
    void setModel( Model* model_, const ParticleSet& indices );

    Source* source( unsigned int idx ) const;
    void setSource( Source* source_, const ParticleSet& indices );
    void removeSourceIndices( Source* source_, const ParticleSet& indices );

    Updater* updater( unsigned int idx ) const;
    void setUpdater( Updater* updater_, const ParticleSet& indices );

  protected:

    UpdateConfig( void );

    std::vector< Model* >* _refModels;
    std::vector< Source* >* _refSources;
    std::vector< Updater* >* _refUpdaters;

    std::vector< bool >* _emitted;
    std::vector< bool >* _dead;

    ParticleCollection* _used;
    ParticleCollection* _unused;
  };
}



#endif /* __PREFR_UPDATECONFIG__ */
