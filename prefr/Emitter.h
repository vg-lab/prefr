/*
 * ParticleEmitter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef EMITTER_H_
#define EMITTER_H_

#include <prefr/api.h>
#include "InterpolationSet.hpp"
#include "Particles.h"
#include "ParticlePrototype.h"
#include "Source.h"


namespace prefr
{

  class Emitter
  {

  public:

    PREFR_API Emitter( const ParticleCollection& particlesArray, float _emissionRate, bool _loop);

    PREFR_API virtual ~Emitter();

    PREFR_API virtual void StartEmission(float deltaTime);
    PREFR_API virtual void EndEmission();
    PREFR_API virtual int EmitSingle(const tparticle_ptr i);

    PREFR_API virtual void EmitAll(float deltaTime);

    PREFR_API virtual void EmitFunction(const tparticle_ptr current, bool override = false);

  protected:

    ParticleCollection _particles;

    std::vector<Source*>* emissionNodes;
    std::vector<int>* refEmissionNodes;

    PrototypesArray* prototypes;
    std::vector<int>* refPrototypes;

    int maxParticles;
    float particlesBudget;
    float emissionRate;
    bool loop;
    bool active;

    int lastParticleNodeID;
    int lastParticlePrototypeID;

    int currentNodeID;
    int currentPrototypeID;

    Source* currentNode;
    tprototype_ptr currentPrototype;

    Source* GetCurrentNode( const int& particleID );
    tprototype_ptr GetCurrentPrototype( const int& particleID );

  private:

    float normalizationFactor;

  };


}



#endif /* EMITTER_H_ */
