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
#include "Model.h"
#include "Particles.h"
#include "Source.h"


namespace prefr
{

  class Emitter
  {

  public:

    PREFR_API Emitter( float _emissionRate, bool _loop);

    PREFR_API virtual ~Emitter();

//    PREFR_API virtual void StartEmission(float deltaTime);
//    PREFR_API virtual void EndEmission();
//    PREFR_API virtual int EmitSingle( const Cluster& cluster,
//                                      const tparticle_ptr particle );

//    PREFR_API virtual void EmitAll(float deltaTime);

//    PREFR_API virtual void EmitFunction( const Cluster& cluster,
//                                         const tparticle_ptr current,
//                                         bool override = false );

  protected:

    int maxParticles;
    float particlesBudget;
    float emissionRate;
    bool loop;
    bool active;

  private:

    float normalizationFactor;

  };


}



#endif /* EMITTER_H_ */
