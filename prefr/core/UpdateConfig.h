/*
 * @file  UpdateConfig.h
 * @brief
 * @author Sergio E. Galindo <sergio.galindo@urjc.es>
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 *          Do not distribute without further notice.
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

    bool dead( unsigned int idx ) const;
    void setDead( unsigned int idx, bool value );

    Model* model( unsigned int idx ) const;
    void setModel( Model* model_, ParticleIndices indices );

    Source* source( unsigned int idx ) const;
    void setSource( Source* source_, ParticleIndices indices );

    Updater* updater( unsigned int idx ) const;
    void setUpdater( Updater* updater_, ParticleIndices indices );

  protected:

    UpdateConfig( void );

    std::vector< Model* >* _refModels;
    std::vector< Source* >* _refSources;
    std::vector< Updater* >* _refUpdaters;

    std::vector< bool >* _emitted;
    std::vector< bool >* _dead;
  };
}



#endif /* __PREFR_UPDATECONFIG__ */
