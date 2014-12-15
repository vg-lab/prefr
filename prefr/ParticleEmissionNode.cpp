/*
 * ParticleEmissionNode.cpp
 *
 *  Created on: 11/12/2014
 *      Author: sgalindo
 */

#include "ParticleEmissionNode.h"

namespace prefr
{

    static float invRandMax = 1.0f / RAND_MAX;

    glm::vec3 GetRandomDirection()
    {
      float theta, phi, vxz;

      theta = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * M_PI;//asinf(clamp(rand()*invRandMax, 0.0f, 1.0f));
      phi = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * M_PI;
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
    , timed( false )
    {}

    bool EmissionNode::Active()
    {
      return active;
    }

    bool EmissionNode::Emits()
    {
      return active && particlesBudget;
    }

    const int& EmissionNode::GetBudget()
    {
      return particlesBudget;
    }

    void EmissionNode::StartFrame( const float& rawBudget,
                                           const float& /*deltaTime */)
    {
      emissionAcc += rawBudget;
      particlesBudget = int(floor(emissionAcc));
      emissionAcc -= particlesBudget;
    }

    void EmissionNode::ReduceBudgetBy(const unsigned int& decrement)
    {
      particlesBudget -= decrement;
    }


    void EmissionNode::CloseFrame()
    {
      particlesBudget = 0;
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
    }

    //***********************************************************
    // POINT EMISSION NODE
    //***********************************************************


    PointEmissionNode::PointEmissionNode( const ParticleCollection& arrayParticles, glm::vec3 _position)
    : TimedEmissionNode( arrayParticles )
    ,position( _position )
    {}

    PointEmissionNode::~PointEmissionNode()
    {
      delete ( particles );
    }

    glm::vec3 PointEmissionNode::GetEmissionPosition()
    {
      return position;
    }

    glm::vec3 PointEmissionNode::GetEmissionVelocityDirection()
    {
      return GetRandomDirection();
    }

}


