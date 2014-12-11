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


    PointEmissionNode::PointEmissionNode( const ParticleCollection& arrayParticles, glm::vec3 _position)
    : EmissionNode( arrayParticles )
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


