/*
 * ParticleEmissionNode.h
 *
 *  Created on: 11/12/2014
 *      Author: sgalindo
 */

#ifndef __ParticleEmissionNode__
#define __ParticleEmissionNode__

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

    bool timed;

    EmissionNode( const ParticleCollection& arrayParticles );

    virtual ~EmissionNode(void) {delete particles;}

    virtual bool Active();
    virtual bool Emits();

    virtual const int& GetBudget();
    virtual void StartFrame(const float& rawBudget, const float& deltaTime);
    virtual void CloseFrame();

    virtual void ReduceBudgetBy(const unsigned int& decrement = 1);


    virtual glm::vec3 GetEmissionPosition() = 0;
    virtual glm::vec3 GetEmissionVelocityDirection() = 0;
  };

  typedef vector<EmissionNode*> EmissionNodesArray;

  class TimedEmissionNode : public EmissionNode, public utils::SingleFrameTimer
  {
  public:

    TimedEmissionNode( const ParticleCollection& arrayParticles );
    TimedEmissionNode( const ParticleCollection& arrayParticles,
                       float period,
                       float offset,
                       float duration);
    virtual bool Emits();

    virtual void StartFrame(const float& rawBudget, const float& deltaTime);
    virtual void CloseFrame();

  };

  //TODO class MultiTimedEmissionNode : public utils::MultiFrameTimer

  class PointEmissionNode : public TimedEmissionNode
  {
  public:

    glm::vec3 position;

    PointEmissionNode( const ParticleCollection& arrayParticles, glm::vec3 _position );
    virtual ~PointEmissionNode();

    virtual glm::vec3 GetEmissionPosition();
    virtual glm::vec3 GetEmissionVelocityDirection();
  };



}


#endif /* __ParticleEmissionNode__ */
