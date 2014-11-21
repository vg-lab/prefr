/*
 * GLParticleSystem.h
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#ifndef GLPARTICLESYSTEM_H_
#define GLPARTICLESYSTEM_H_

#include "../ParticleSystem.h"
#include "DefaultTypes.h"


using namespace std;
using namespace particles;

namespace particles
{
  namespace defaultParticleSystem
  {

    class DefaultParticleSystem : public ParticleSystem
    {
    public:

      bool updateLoopUnified;

      DefaultParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop = true);
      virtual ~DefaultParticleSystem(){}


      virtual void Start();
      virtual void Update(float deltaTime);
      virtual void UpdateUnified(float deltaTime);
      virtual void UpdateSeparated(float deltaTime);
      virtual void UpdateCameraDistances(const glm::vec3& cameraPosition) = 0;
      virtual void UpdateRender() = 0;
      virtual void Render() const = 0;
    };

  }
}
#endif /* GLPARTICLESYSTEM_H_ */
