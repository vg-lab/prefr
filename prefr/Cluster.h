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

    Source* source( void );
    Model* model( void );
    Updater* updater( void );

    void source( Source* source );
    void model( Model* model );
    void updater( Updater* updater);

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
