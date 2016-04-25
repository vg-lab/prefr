/*
 * ParticleUpdater.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEUPDATER_H_
#define PARTICLEUPDATER_H_

#include <type_traits>

#include <prefr/api.h>

#include "Particles.h"

#include "Cluster.h"
#include "Model.h"
#include "Source.h"

namespace prefr
{

  class Cluster;

  //! This class updates the particles through the particle life cycle.
  /*!
   *
   */
  class Updater
  {


public:

    PREFR_API Updater( void );

    PREFR_API virtual ~Updater();

    PREFR_API virtual void Emit( const Cluster& cluster,
                                 const tparticle_ptr current );

    //! Update a single particle by the given particle collection index
    /*!
     *
     */
    PREFR_API virtual void Update( const Cluster& cluster,
                                   const tparticle_ptr current,
                                   float deltaTime );
protected:

    template < class T >
//    typename std::enable_if<std::is_base_of< ParticlePrototype, T>::value, void>::type
    void UpdateLife( const T& model, const tparticle_ptr current );

  };
}



#endif /* PARTICLEUPDATER_H_ */
