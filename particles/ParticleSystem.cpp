/*
 * ParticleSystem.cpp
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#include "ParticleSystem.h"

namespace particles
{

  ParticleSystem::ParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate
               , bool _loop)
  : emitters(nullptr)
  , updaters(nullptr)
  , sorter(nullptr)
  , renderConfigurer(nullptr)
  , render(nullptr)
  , maxParticles (_maxParticles)
  , emissionRate(_emissionRate)
  , loop(_loop)
  {
    tparticleContainer* p = new tparticleContainer(maxParticles);
    particles = new ParticleCollection(p, 0, maxParticles);

    particleEmitter.resize(maxParticles);
    particleUpdater.resize(maxParticles);

    int counter = 0;
    tparticleptr particle;
    for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
    {
      particle = new Particle(counter, counter < initialParticlesNumber);
      counter++;
    }

    aliveParticles = initialParticlesNumber;
  }


  ParticleSystem::~ParticleSystem()
  {
    delete(emitters);
    delete(updaters);
    delete(sorter);
    delete(renderConfigurer);
    delete(render);
  }

  void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
  {
    this->emitters->push_back(emitter);

    int size = this->emitters->size();
    int start = emitter->particles->start - this->particles->start;
    int end = emitter->particles->end - this->particles->start;

    for (int i = start; i < end; i++)
    {
      this->particleEmitter[i] = size;
    }

  }
  void ParticleSystem::AddUpdater(ParticleUpdater* updater)
  {
    this->updaters->push_back(updater);

    int size = this->emitters->size();
    int start = updater->particles->start - this->particles->start;
    int end = updater->particles->end - this->particles->start;

    for (int i = start; i < end; i++)
    {
      this->particleUpdater[i] = size;
    }

  }
  void ParticleSystem::SetSorter(ParticleSorter* sorter)
  {
    this->sorter = sorter;
  }
  void ParticleSystem::SetRenderConfig(ParticleRenderConfig* renderConfig)
  {
    this->renderConfigurer = renderConfig;
  }
  void ParticleSystem::SetParticleRender(ParticleRender* render)
  {
    this->render = render;
  }

  void ParticleSystem::UpdateUnified(float deltaTime)
  {
    int i = 0;
    for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
    {

      // Emit each particle with its own emitter
      emitters->at(particleEmitter[i])->Emit(i, deltaTime);

      // Update each particle with its own updater
      updaters->at(particleUpdater[i])->Update(i, deltaTime);

      i++;
    }
  }

  void ParticleSystem::UpdateSeparated(float deltaTime)
  {
    for (int i = 0; i < emitters->size(); i++)
    {
      emitters->at(i)->Update(deltaTime);
    }

    for (int i = 0; i < updaters->size(); i++)
    {
      updaters->at(i)->Update(deltaTime);
    }

  }

  void ParticleSystem::UpdateRender()
  {
    this->sorter->Sort();

    this->renderConfigurer->SetupRender();
  }

  void ParticleSystem::Render()
  {
    this->render->Paint();
  }




}


