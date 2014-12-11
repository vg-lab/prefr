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

namespace prefr
{

  class EmissionNode
  {
  public:

    ParticleCollection* particles;
    float emissionAcc;
    int particlesBudget;
    bool active;

    EmissionNode( const ParticleCollection& arrayParticles )
    : particles( new ParticleCollection( arrayParticles ) )
    , emissionAcc( 0 )
    , particlesBudget( 0 )
    , active( true )
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

}


#endif /* __ParticleEmissionNode__ */
