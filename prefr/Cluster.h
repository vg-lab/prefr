/*
 * Cluster.h
 *
 *  Created on: 20 de ene. de 2016
 *      Author: sgalindo
 */

#ifndef PREFR_CLUSTER_H_
#define PREFR_CLUSTER_H_

#include "Particles.h"

namespace prefr
{

  class Cluster
  {

  public:

    typedef std::pair< unsigned int, unsigned int > TBoundsUI;
    typedef std::pair< Particles::iterator, Particles::iterator > TBoundsIt;

    Cluster( );

    Source* source( void );
    void source( Source* source );

    ParticlePrototype* model( void );
    void model( ParticlePrototype* model );

    Emitter* emitter( void );
    void emitter( Emitter* emitter );

    ParticleUpdater* updater( void );
    void updater( ParticleUpdater* updater);

    ParticleRange particles( void );
    void particles( const ParticleRange& particleArray );

    bool active( void );
    void active( bool active_ );

  protected:

    ParticleRange _particlesArray;

    unsigned int _size;

    Source* _source;

    ParticlePrototype* _model;

    Emitter* _emitter;

    ParticleUpdater* _updater;

    bool _active;
  };

}

#endif /* PREFR_CLUSTER_H_ */
