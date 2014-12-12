/*
 * ParticleUpdater.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEUPDATER_H_
#define PARTICLEUPDATER_H_

#include "ElementCollection.hpp"
#include "ParticleEmitter.h"

using namespace utils;

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
    vector<int>* refPrototypes;

    vector<EmissionNode*>* emissionNodes;
    vector<int>* refEmissionNodes;

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

    ParticleUpdater(const ParticleCollection& particlesArray);

    virtual ~ParticleUpdater();

    //! Updates collection particles with the given deltaTime value.
    /*!
     *
     */
    virtual int Update(float deltaTime);

    //! Update a single particle by the given particle collection index
    /*!
     *
     */
    virtual void Update(const tparticle_ptr current, float deltaTime);

  };
}



#endif /* PARTICLEUPDATER_H_ */
