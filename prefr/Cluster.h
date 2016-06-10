/*
 * Cluster.h
 *
 *  Created on: 20 de ene. de 2016
 *      Author: sgalindo
 */

#ifndef PREFR_CLUSTER_H_
#define PREFR_CLUSTER_H_

#include "Particles.h"

#include "Source.h"
#include "Emitter.h"
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

    Emitter* emitter( void ) const;
    void emitter( Emitter* emitter );

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

    Emitter* _emitter;

    Updater* _updater;

    bool _active;

    bool _inactiveKillParticles;
  };

  typedef std::vector< Cluster* > ClustersArray;

}

#endif /* PREFR_CLUSTER_H_ */
