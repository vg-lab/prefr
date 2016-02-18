/*
 * ParticleEmissionNode.cpp
 *
 *  Created on: 11/12/2014
 *      Author: sgalindo
 */

#include "ParticleEmissionNode.h"
#include "types.h"

namespace prefr
{

    static float invRandMax = 1.0f / RAND_MAX;
    static float pi2 = 2.0f * float(M_PI);

    glm::vec3 GetRandomDirection(float thetaAngle = pi2)
    {
      float theta, phi, vxz;

      theta = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * thetaAngle;//asinf(clamp(rand()*invRandMax, 0.0f, 1.0f));
      phi = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * pi2;
      vxz = sinf(theta);

      return glm::vec3 (cosf(phi)*vxz, cosf(theta), sinf(phi)*vxz);
    }


    //***********************************************************
    // EMISSION NODE
    //***********************************************************

    EmissionNode::EmissionNode( const ParticleCollection& arrayParticles )
    : particles( new ParticleCollection( arrayParticles ) )
    , emissionAcc( 0 )
    , particlesBudget( 0 )
    , active( true )
    , continueEmission( true )
    , finished( false )
    , autoDeactivateWhenFinished( true )
    , killParticlesIfInactive( false )
    , lastFrameAliveParticles( 0 )
    , emittedParticles( 0 )
    , maxEmissionCycles( 0 )
    , currentCycle( 0 )
    {}

    bool EmissionNode::Active()
    {
      return active;
    }

    bool EmissionNode::Emits()
    {
      return active && particlesBudget > 0 && Continue();
    }

    bool EmissionNode::Continue()
    {
      return continueEmission;
    }

    bool EmissionNode::Finished()
    {
      return finished;
    }

    const int& EmissionNode::GetBudget()
    {
      return particlesBudget;
    }

    void EmissionNode::Restart()
    {
      currentCycle = 0;
      emittedParticles = 0;
      continueEmission = true;
    }

    void EmissionNode::StartFrame( const float& rawBudget,
                                           const float& /*deltaTime */)
    {
      emissionAcc += rawBudget;
      particlesBudget = int(floor(emissionAcc));
      emissionAcc -= particlesBudget;

      lastFrameAliveParticles = 0;
    }

    void EmissionNode::IncreaseAlive()
    {
      lastFrameAliveParticles++;
    }

    void EmissionNode::CheckEmissionEnd()
    {
      if (maxEmissionCycles > 0)
      {

        if (emittedParticles >= particles->size)
        {
         currentCycle++;
         emittedParticles -= particles->size;
        }

        this->continueEmission = !(currentCycle >= maxEmissionCycles);

      }
    }


    void EmissionNode::ReduceBudgetBy(const unsigned int& decrement)
    {
      particlesBudget -= decrement;
      emittedParticles += decrement;
    }


    void EmissionNode::CloseFrame()
    {
      particlesBudget = 0;

      CheckEmissionEnd();

      this->finished = !continueEmission && lastFrameAliveParticles == 0 ;

      if (finished && autoDeactivateWhenFinished)
        this->active = false;
    }


    //***********************************************************
    // TIMED EMISSION NODE
    //***********************************************************

    TimedEmissionNode::TimedEmissionNode( const ParticleCollection& arrayParticles )
    : EmissionNode( arrayParticles )
    , SingleFrameTimer( 0, 0, 0 )
    {}

    TimedEmissionNode::TimedEmissionNode( const ParticleCollection& arrayParticles,
                            float period,
                            float offset,
                            float duration)
    : EmissionNode( arrayParticles )
    , SingleFrameTimer( period, offset, duration )
    {}

    bool TimedEmissionNode::Emits()
    {
      return InTime() && EmissionNode::Emits();
    }

    void TimedEmissionNode::CheckEmissionEnd()
    {
      if (maxEmissionCycles > 0)
      {

        if (emittedParticles >= particles->size)
        {
          currentCycle++;
          emittedParticles -= particles->size;
        }
        else if (AfterTime())
        {
          currentCycle++;
          emittedParticles = 0;
        }

        this->continueEmission = !(currentCycle >= maxEmissionCycles);

      }
    }

    void TimedEmissionNode::StartFrame( const float& rawBudget,
                                   const float& deltaTime )
    {
      EmissionNode::StartFrame(rawBudget, deltaTime);

      UpdateTimer(deltaTime);
    }

    void TimedEmissionNode::CloseFrame()
    {
      EmissionNode::CloseFrame();

      RestoreTimer();

//      this->finished = !continueEmission && lastFrameAliveParticles == 0 ;

    }

    //***********************************************************
    // POINT EMISSION NODE
    //***********************************************************


    PointEmissionNode::PointEmissionNode( const ParticleCollection& arrayParticles, glm::vec3 _position)
    : TimedEmissionNode( arrayParticles )
    ,position( _position )
    {}

    PointEmissionNode::~PointEmissionNode()
    {}

    void PointEmissionNode::SetEmissionPosition(float x, float y, float z)
    {
      position = glm::vec3(x, y, z);
    }

    glm::vec3 PointEmissionNode::GetEmissionPosition()
    {
      return position;
    }

    glm::vec3 PointEmissionNode::GetEmissionVelocityDirection()
    {
      return GetRandomDirection();
    }



    SphereEmissionNode::SphereEmissionNode( const ParticleCollection& arrayParticles, glm::vec3 _position, float radius_, float angle_)
    : PointEmissionNode( arrayParticles, _position )
    , radius( radius_ )
    , angle( glm::radians(angle_) )
    {}

    SphereEmissionNode::~SphereEmissionNode()
    {}

    glm::vec3 SphereEmissionNode::GetEmissionPosition()
    {
      return position + (radius * velocity);
    }

    glm::vec3 SphereEmissionNode::GetEmissionVelocityDirection()
    {
      velocity = GetRandomDirection();
      return velocity;
    }

}


