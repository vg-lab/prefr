/*
 * ParticleSystem.cpp
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#include "ParticleSystem.h"
#include "log.h"

namespace prefr
{

  ParticleSystem::ParticleSystem(unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop)
  : sorter( nullptr )
  , renderer( nullptr )
  , maxParticles (_maxParticles)
  , loop(_loop)
  , renderDeadParticles( false )
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
      (*particles->elements)[counter] = new tparticle(counter, counter < initialParticlesNumber);
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

    int size = int(this->emissionNodes->size());
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

    int size = int(this->prototypes->size());
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

    int size = int(this->emitters->size());
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

    int size = int(this->updaters->size());
    int start = updater->particles->start - this->particles->start;
    int end = updater->particles->end - this->particles->start;

    for (int i = start; i < end; i++)
    {
      this->particleUpdater[i] = size-1;
    }

    updater->prototypes = this->prototypes;
    updater->refPrototypes = &this->particlePrototype;

    updater->emissionNodes = this->emissionNodes;
    updater->refEmissionNodes = &this->particleEmissionNodes;

  }
  void ParticleSystem::SetSorter(ParticleSorter* _sorter)
  {
    this->sorter = _sorter;
    this->sorter->InitDistanceArray();
    this->sorter->emissionNodes = this->emissionNodes;
  }
  void ParticleSystem::SetRenderer(ParticleRenderer* _renderer)
  {
    this->renderer = _renderer;

    PREFR_DEBUG_CHECK( this->sorter->distances, "distances is null" );
    this->renderer->distances = this->sorter->distances;
  }

  void ParticleSystem::Start()
  {
    for (unsigned int i = 0; i < aliveParticles; i++)
    {
      tparticle_ptr current = particles->GetElement(i);
      (*emitters)[particleEmitter[i]]->EmitFunction(current, true);
    }

  }

  void ParticleSystem::Update(float deltaTime)
  {
    for (unsigned int i = 0; i < emitters->size(); i++)
    {
      (*emitters)[i]->EmitAll(deltaTime);
    }

    int accumulator = 0;
    for (unsigned int i = 0; i < updaters->size(); i++)
    {
      accumulator += (*updaters)[i]->Update(deltaTime);
    }

    this->aliveParticles = accumulator;

  }

  void ParticleSystem::UpdateUnified(float deltaTime)
  {
    unsigned int i = 0;

    // Set emitter delta time to calculate the number of particles to emit this frame
    for (i = 0; i < emitters->size(); i++)
    {
      (*emitters)[i]->StartEmission(deltaTime);
    }

    int accumulator = 0;

    EmissionNodesArray::iterator nodit;
    for (nodit = emissionNodes->begin();
         nodit != emissionNodes->end();
         nodit++)
    {

      if (!(*nodit) || !(*nodit)->Active())
        continue;

      EmissionNode* currentNode = (*nodit);

      for (tparticleContainer::iterator it = currentNode->particles->start;
          it != currentNode->particles->end;
          it++)
      {
        i = ((tparticle_ptr) *it)->id;

        // Emit each particle with its own emitter
        (*emitters)[particleEmitter[i]]->EmitSingle(*it);

        // Update each particle with its own updater
        (*updaters)[particleUpdater[i]]->Update(*it, deltaTime);

        if ((*it)->Alive())
        {
        currentNode->IncreaseAlive();
        accumulator++;// += (*it)->Alive();
        }
      }

    }

    for (i = 0; i < emitters->size(); i++)
    {
      (*emitters)[i]->EndEmission();
    }

    this->aliveParticles = accumulator;
  }

  void ParticleSystem::UpdateCameraDistances(const glm::vec3& cameraPosition)
  {

    this->sorter->UpdateCameraDistance( cameraPosition, renderDeadParticles );
//    unsigned int i = 0;
//    for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
//    {
////     i = ((tparticle_ptr) *it)->id;
//     this->sorter->UpdateCameraDistance(*it, cameraPosition);
//    }
  }

  void ParticleSystem::UpdateRender()
  {
   this->sorter->Sort();

   this->renderer->SetupRender(this->aliveParticles);
  }

  void ParticleSystem::Render() const
  {
   this->renderer->Paint(aliveParticles);
  }

}


