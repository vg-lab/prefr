/*
 * ParticleSystem.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <particles/config.h>

#include "ElementCollection.hpp"

#include "ParticleEmitter.h"
#include "ParticleUpdater.h"
#include "ParticleSorter.h"
#include "ParticleRenderer.h"

#include "ParticlePrototype.h"

#include <vector>

using namespace std;
using namespace utils;

namespace particles
{

  //! This class provides the common inheritable framework for creating a particle system.
  /*!
   *  This class establishes a common framework which should handle the general behavior of the particle system.
   * It is in charge of providing several external calls in order to control the particle system working.
   *
   *  This class provides basic functionality concerning particle emission nodes, emitters and updaters, as well
   * as managing the general correspondence between mentioned elements' combinations.
   *
   *  This framework is particle-focused, i.e. the main entity used in the system is the particle itself, which
   * eventually might be replaced as vector-based representation (for more efficiency).
   *
   *  Inherit from this class in order to created customized behavior and features when communication between
   * different elements than the ones introduced here are necessary or to simply modify the actual functionality.
   *
   */
  class ParticleSystem
  {
  public:

    //! Particles collection the system will manage.
    ParticleCollection* particles;

    //! Emission nodes array of the particle set.
    EmissionNodesArray* emissionNodes;

    //! Particle prototypes of the particle set.
    PrototypesArray* prototypes;

    //! Emitter objects collection of the system.
    vector<ParticleEmitter*>* emitters;

    //! Updater objects collection of the system.
    vector<ParticleUpdater*>* updaters;

    //! Particle sorter for alpha rendering.
    ParticleSorter* sorter;

    //! Particles renderer (OpenGL, OSG, etc.)
    ParticleRenderer* renderer;

  protected:
    vector<int> particleEmissionNodes;
    vector<int> particlePrototype;
    vector<int> particleEmitter;
    vector<int> particleUpdater;

  public:
    unsigned int aliveParticles;
    unsigned int maxParticles;

    bool loop;

    ParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop = true);

    virtual ~ParticleSystem();

    virtual void AddEmissionNode(EmissionNode* node);
    virtual void AddPrototype(ParticlePrototype* prototype);
    virtual void AddEmitter(ParticleEmitter* emitter);
    virtual void AddUpdater(ParticleUpdater* updater);
    virtual void SetSorter(ParticleSorter* sorter);
    virtual void SetRenderer(ParticleRenderer* renderConfig);


    virtual void Start();

    // Particle updating methods
    virtual void Update(float deltaTime);

    virtual void UpdateUnified(float deltaTime);


    virtual void UpdateCameraDistances(const glm::vec3& cameraPosition) = 0;

    // Particle render updating method
    virtual void UpdateRender() = 0;

    // Render method
    virtual void Render() const = 0;




  };

}

#endif /* PARTICLESYSTEM_H_ */
