/*
 * ParticleSystem.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_


#include <prefr/api.h>
#include "ElementCollection.hpp"
#include "DistanceArray.hpp"
#include "RenderConfig.h"

#include "ParticleEmitter.h"
#include "ParticleEmissionNode.h"
#include "ParticleUpdater.h"
#include "ParticleSorter.h"
#include "ParticleRenderer.h"

#include "ParticlePrototype.h"

#include <vector>


namespace prefr
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
    std::vector<ParticleEmitter*>* emitters;

    //! Updater objects collection of the system.
    std::vector<ParticleUpdater*>* updaters;

    //! Particle sorter for alpha rendering.
    ParticleSorter* sorter;

    //! Particles renderer (OpenGL, OSG, etc.)
    ParticleRenderer* renderer;

  protected:
    std::vector<int> particleEmissionNodes;
    std::vector<int> particlePrototype;
    std::vector<int> particleEmitter;
    std::vector<int> particleUpdater;

  public:
    unsigned int aliveParticles;
    unsigned int maxParticles;

    bool loop;
    bool renderDeadParticles;
    bool run;

    PREFR_API
    ParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop = true);

    PREFR_API
    virtual ~ParticleSystem();

    PREFR_API
    virtual void AddEmissionNode(EmissionNode* node);
    PREFR_API
    virtual void AddPrototype(ParticlePrototype* prototype);
    PREFR_API
    virtual void AddEmitter(ParticleEmitter* emitter);
    PREFR_API
    virtual void AddUpdater(ParticleUpdater* updater);
    PREFR_API
    virtual void SetSorter(ParticleSorter* _sorter);
    PREFR_API
    virtual void SetRenderer(ParticleRenderer* _renderConfig);

    PREFR_API
    virtual void Start();

    // Particle updating methods
    PREFR_API
    virtual void Update(float deltaTime);

    PREFR_API
    virtual void UpdateUnified(float deltaTime);

    PREFR_API
    virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);

    // Particle render updating method
    PREFR_API
    virtual void UpdateRender();

    // Render method
    PREFR_API
    virtual void Render() const;

    virtual void Run( bool run_ );
    virtual bool Run( void );


  };

}

#endif /* PARTICLESYSTEM_H_ */
