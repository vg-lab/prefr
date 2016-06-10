/*
 * ParticleUpdater.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEUPDATER_H_
#define PARTICLEUPDATER_H_

#include <prefr/api.h>
#include "Particles.h"
#include "ParticleEmitter.h"

namespace prefr
{
  //! This class updates the particles through the particle life cycle.
  /*!
   *
   */
  class ParticleUpdater
  {
public:

    ParticleCollection* particles;

    PrototypesArray* prototypes;
    std::vector<int>* refPrototypes;

    std::vector<EmissionNode*>* emissionNodes;
    std::vector<int>* refEmissionNodes;

protected:

    int lastParticleNodeID;
    int lastParticlePrototypeID;

    int currentNodeID;
    int currentPrototypeID;

    EmissionNode* currentNode;
    tprototype_ptr currentPrototype;

    EmissionNode* GetCurrentNode( const int& particleID );
    tprototype_ptr GetCurrentPrototype( const int& particleID );

public:

    PREFR_API ParticleUpdater(const ParticleCollection& particlesArray);

    PREFR_API virtual ~ParticleUpdater();

    //! Updates collection particles with the given deltaTime value.
    /*!
     *
     */
    PREFR_API virtual int Update(float deltaTime);

    //! Update a single particle by the given particle collection index
    /*!
     *
     */
    PREFR_API virtual void Update(const tparticle_ptr current, float deltaTime);

  };
}



#endif /* PARTICLEUPDATER_H_ */
