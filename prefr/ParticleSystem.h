/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef __PREFR__PARTICLE_SYSTEM__
#define __PREFR__PARTICLE_SYSTEM__


#include <prefr/api.h>
#include "ElementCollection.hpp"
#include "DistanceArray.hpp"
#include "RenderConfig.h"

#include "Particles.h"

#include <vector>

#include "Model.h"
#include "Renderer.h"
#include "Sorter.h"
#include "Source.h"
#include "Updater.h"


namespace prefr
{

  /*
   This class provides the common inheritable framework for creating a
   particle system. This class establishes a common framework which
   should handle the general behavior of the particle system. It is in
   charge of providing several external calls in order to control the
   particle system working. This class provides basic functionality
   concerning particle emission nodes, emitters and updaters, as well
   as managing the general correspondence between mentioned elements
   combinations.  This framework is particle-focused, i.e. the main
   entity used in the system is the particle itself, which eventually
   might be replaced as vector-based representation (for more
   efficiency).  Inherit from this class in order to created
   customized behavior and features when communication between
   different elements than the ones introduced here are necessary or
   to simply modify the actual functionality.
  */
 class ParticleSystem
 {

 public:

    PREFR_API
    ParticleSystem( unsigned int _maxParticles );

    PREFR_API
    virtual ~ParticleSystem();

    virtual void AddCluster( Cluster* cluster,
                             unsigned int start,
                             unsigned int size_ );

    PREFR_API
    virtual void AddEmissionNode(Source* node);
    PREFR_API
    virtual void AddPrototype(Model* prototype);
    PREFR_API
    virtual void AddUpdater(Updater* updater);
    PREFR_API
    virtual void sorter(Sorter* _sorter);
    PREFR_API
    virtual Sorter* sorter( void );

    PREFR_API
    virtual void renderer(Renderer* _renderConfig);

    PREFR_API Renderer* renderer( void );

    PREFR_API
    virtual void Start();

    // Particle updating method
    PREFR_API
    virtual void Update( const float& deltaTime);

    // Compute particles distance to camera
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

    virtual unsigned int aliveParticles( void );

  protected:

    //! Particles collection the system will manage.
//    ParticleCollection* particles;
    Particles _particles;

    ClustersArray _clusters;

    //! Emission nodes array of the particle set.
    SourcesArray _sources;

    //! Particle prototypes of the particle set.
    ModelsArray prototypes;

    //! Emitter objects collection of the system.
    std::vector<Emitter*> emitters;

    //! Updater objects collection of the system.
    std::vector<Updater*> updaters;

    //! Particle sorter for alpha rendering.
    Sorter* _sorter;

    //! Particles renderer (OpenGL, OSG, etc.)
    Renderer* _renderer;

    //    std::vector<int> particleEmissionNodes;
    //    std::vector<int> particlePrototype;
    //    std::vector<int> particleEmitter;
    //    std::vector<int> particleUpdater;
    std::vector< int > _clusterReference;

    unsigned int _aliveParticles;
    unsigned int maxParticles;

    bool renderDeadParticles;
    bool run;

  };

}

#endif /* _PREFR__PARTICLE_SYSTEM__ */
