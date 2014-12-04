/*
 * ParticleEmitter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include "InterpolationSet.hpp"
#include "ElementCollection.hpp"
#include "ParticlePrototype.h"

using namespace utils;

namespace prefr
{

  class EmissionNode
  {
  public:
    ParticleCollection* particles;

    EmissionNode( const ParticleCollection& arrayParticles )
    : particles( new ParticleCollection( arrayParticles ) )
    {
    }

    virtual ~EmissionNode(void) {delete particles;}

    virtual glm::vec3 GetEmissionPosition() = 0;
    virtual glm::vec3 GetEmissionVelocityDirection() = 0;
  };

  typedef vector<EmissionNode*> EmissionNodesArray;

  class PointEmissionNode : public EmissionNode
  {
  public:

    glm::vec3 position;

    PointEmissionNode(  const ParticleCollection& arrayParticles, glm::vec3 _position);
    virtual ~PointEmissionNode();

    virtual glm::vec3 GetEmissionPosition();
    virtual glm::vec3 GetEmissionVelocityDirection();
  };



  class ParticleEmitter
  {

  public:

    ParticleCollection* particles;

    vector<EmissionNode*>* emissionNodes;
    vector<int>* refEmissionNodes;

    PrototypesArray* prototypes;
    vector<int>* refPrototypes;

    vector<int> emissionNodeParticlesPerCycle;

    int maxParticles;
    int particlesPerCycle;
    float emissionRate;
    bool loop;

  private:

    float normalizationFactor;

  public:

    ParticleEmitter( const ParticleCollection& particlesArray, float _emissionRate, bool _loop);

    virtual ~ParticleEmitter();

    virtual void StartEmission(float deltaTime);
    virtual int EmitSingle(unsigned int i);

    virtual void EmitAll(float deltaTime);

    virtual void EmitFunction(unsigned int i, bool override = false);

    virtual void UpdateConfiguration();

  };


}



#endif /* PARTICLEEMITTER_H_ */
