/*
 * ParticleEmissionNode.h
 *
 *  Created on: 11/12/2014
 *      Author: sgalindo
 */

#ifndef __ParticleEmissionNode__
#define __ParticleEmissionNode__

#include <prefr/api.h>
#include "types.h"
#include "ElementCollection.hpp"
#include "Timer.hpp"

namespace prefr
{

  class EmissionNode
  {
  public:

    ParticleCollection* particles;
    float emissionAcc;
    int particlesBudget;
    bool active;
    bool continueEmission;
    bool finished;

    bool killParticlesIfInactive;

    int lastFrameAliveParticles;

    int emittedParticles;
    unsigned int maxEmissionCycles;
    unsigned int currentCycle;

    PREFR_API
    EmissionNode( const ParticleCollection& arrayParticles );

    PREFR_API
    virtual ~EmissionNode(void) {delete particles;}

    PREFR_API virtual bool Active();
    PREFR_API virtual bool Emits();
    PREFR_API inline virtual bool Continue();
    PREFR_API inline virtual bool Finished();

    PREFR_API virtual const int& GetBudget();
    PREFR_API virtual void StartFrame(const float& rawBudget, const float& deltaTime);
    PREFR_API virtual void CloseFrame();

    PREFR_API virtual void IncreaseAlive();
    PREFR_API inline virtual void CheckEmissionEnd();

    PREFR_API virtual void ReduceBudgetBy(const unsigned int& decrement = 1);

    PREFR_API virtual glm::vec3 GetEmissionPosition() = 0;
    PREFR_API virtual glm::vec3 GetEmissionVelocityDirection() = 0;
  };

  typedef vector<EmissionNode*> EmissionNodesArray;

  class TimedEmissionNode : public EmissionNode, public utils::SingleFrameTimer
  {
  public:

    PREFR_API TimedEmissionNode( const ParticleCollection& arrayParticles );
    PREFR_API TimedEmissionNode( const ParticleCollection& arrayParticles,
				 float period,
				 float offset,
				 float duration);
    PREFR_API inline virtual bool Emits();

    PREFR_API inline virtual void CheckEmissionEnd();

    PREFR_API inline virtual void StartFrame(const float& rawBudget, const float& deltaTime);
    PREFR_API inline virtual void CloseFrame();

  };

  //TODO class MultiTimedEmissionNode : public utils::MultiFrameTimer

  class PointEmissionNode : public TimedEmissionNode
  {
  public:

    glm::vec3 position;

    PREFR_API PointEmissionNode( const ParticleCollection& arrayParticles,
				 glm::vec3 _position );

    PREFR_API virtual ~PointEmissionNode();

    PREFR_API inline virtual glm::vec3 GetEmissionPosition();
    PREFR_API inline virtual glm::vec3 GetEmissionVelocityDirection();
  };

  class SphereEmissionNode : public PointEmissionNode
  {
  public:

    float radius;
    float angle;

    glm::vec3 velocity;

    PREFR_API SphereEmissionNode( const ParticleCollection& arrayParticles,
				  glm::vec3 _position, float radius_ = 0, float angle_ = 360 );

    PREFR_API virtual ~SphereEmissionNode();

    PREFR_API inline virtual glm::vec3 GetEmissionPosition();
    PREFR_API inline virtual glm::vec3 GetEmissionVelocityDirection();

  };



}


#endif /* __ParticleEmissionNode__ */
