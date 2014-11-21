/*
 * ParticleSystem.cpp
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#include "ParticleSystem.h"

namespace particles
{

  ParticleSystem::ParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop)
  : sorter( nullptr )
  , renderer( nullptr )
  , maxParticles (_maxParticles)
  , loop(_loop)
  {
    tparticleContainer* p = new tparticleContainer(maxParticles);
    particles = new ParticleCollection(p, p->begin(), p->end());

    particleEmissionNodes.resize(maxParticles, -1);
    particlePrototype.resize(maxParticles, -1);
    particleEmitter.resize(maxParticles, -1);
    particleUpdater.resize(maxParticles, -1);

    emissionNodes = new EmissionNodesArray;
    prototypes = new PrototypesArray;
    emitters = new vector<ParticleEmitter*>;
    updaters = new vector<ParticleUpdater*>;


    if (initialParticlesNumber > maxParticles)
      initialParticlesNumber = maxParticles;

    unsigned int counter = 0;

    for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
    {
      particles->elements->at(counter) = new tparticle(counter, counter < initialParticlesNumber);
      counter++;
    }

    aliveParticles = initialParticlesNumber;
  }


  ParticleSystem::~ParticleSystem()
  {
    delete( emissionNodes );
    delete( prototypes);
    delete( emitters );
    delete( updaters );
    delete( sorter );
    delete( renderer );
  }

  void ParticleSystem::AddEmissionNode(EmissionNode* node)
  {
    this->emissionNodes->push_back(node);

    int size = this->emissionNodes->size();
    int start = node->particles->start - this->particles->start;
    int end = node->particles->end - this->particles->start;

    for (int i = start; i < end; i++)
    {
      this->particleEmissionNodes[i] = size-1;
    }
  }

  void ParticleSystem::AddPrototype(ParticlePrototype* prototype)
  {
    this->prototypes->push_back(prototype);

    int size = this->prototypes->size();
    int start = prototype->particles->start - this->particles->start;
    int end = prototype->particles->end - this->particles->start;

    for (int i = start; i < end; i++)
    {
      this->particlePrototype[i] = size-1;
    }
  }

  void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
  {
    this->emitters->push_back(emitter);

    int size = this->emitters->size();
    int start = emitter->particles->start - this->particles->start;
    int end = emitter->particles->end - this->particles->start;

    for (int i = start; i < end; i++)
    {
      this->particleEmitter[i] = size-1;
    }

    emitter->emissionNodes = this->emissionNodes;
    emitter->refEmissionNodes = &this->particleEmissionNodes;

    emitter->prototypes = this->prototypes;
    emitter->refPrototypes = &this->particlePrototype;

  }
  void ParticleSystem::AddUpdater(ParticleUpdater* updater)
  {
    this->updaters->push_back(updater);

    int size = this->updaters->size();
    int start = updater->particles->start - this->particles->start;
    int end = updater->particles->end - this->particles->start;

    for (int i = start; i < end; i++)
    {
      this->particleUpdater[i] = size-1;
    }

    updater->prototypes = this->prototypes;
    updater->refPrototypes = &this->particlePrototype;

  }
  void ParticleSystem::SetSorter(ParticleSorter* sorter)
  {
    this->sorter = sorter;
  }
  void ParticleSystem::SetRenderer(ParticleRenderer* renderConfig)
  {
    this->renderer = renderConfig;
  }





}


