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
    Cluster( unsigned int lowerBound, unsigned int upperBound );

    EmissionNode* source( void );
    ParticlePrototype* model( void );
    ParticleEmitter* emitter( void );
    ParticleUpdater* updater( void );

    void source( EmissionNode* source );
    void prototype( ParticlePrototype* model );
    void emitter( ParticleEmitter* emitter );
    void updater( ParticleUpdater* updater);


    TBoundsIt bounds( void );
    void bounds( TBoundsUI bounds_ );
    void bounds( TBoundsIt bounds_ );
    void bounds( unsigned int lowerBound, unsigned int upperBound );

    bool active( void );
    void active( bool active_ );

  protected:

    Particles::iterator _begin;
    Particles::iterator _end;

    unsigned int _lowerBound;
    unsigned int _upperbound;

    bool _active;
  };

}

#endif /* PREFR_CLUSTER_H_ */
