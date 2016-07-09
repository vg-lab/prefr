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

#include <vector>

#include "../utils/ElementCollection.hpp"

#include "DistanceArray.hpp"
#include "RenderConfig.h"

#include "Particles.h"

#include "Model.h"
#include "Renderer.h"
#include "Sorter.h"
#include "Source.h"
#include "Updater.h"

#include <reto/reto.h>

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
    ParticleSystem( unsigned int _maxParticles,
                    ICamera* camera = nullptr );

    PREFR_API
    virtual ~ParticleSystem( );

    // Configuration methods

    PREFR_API
    virtual void AddCluster( Cluster* cluster,
                             unsigned int start,
                             unsigned int size_ );

    PREFR_API
    virtual void AddSource( Source* node );
    PREFR_API
    virtual void AddModel( Model* prototype );
    PREFR_API
    virtual void AddUpdater( Updater* updater );
    PREFR_API
    virtual void sorter( Sorter* _sorter );

    PREFR_API
    virtual Sorter* sorter( void ) const;

    PREFR_API
    virtual void renderer( Renderer* _renderConfig );

    PREFR_API
    Renderer* renderer( void ) const;


    // Pipeline methods


    PREFR_API
    virtual void Start( void );

    // Particle updating method
    PREFR_API
    virtual void Update( const float& deltaTime );

    // Compute particles distance to camera
    PREFR_API
    virtual void UpdateCameraDistances( const glm::vec3& cameraPosition );

    PREFR_API
    virtual void UpdateCameraDistances( void );

    // Particle render updating method
    PREFR_API
    virtual void UpdateRender( void );

    // Render method
    PREFR_API
    virtual void Render( void ) const;

    // Getters/Setters

    PREFR_API
    virtual void run( bool run_ );

    PREFR_API
    virtual bool run( void ) const ;

    PREFR_API
    virtual unsigned int aliveParticles( void ) const;

#ifdef PREFR_USE_OPENMP
    PREFR_API
    void parallel( bool parallelProcessing );
#endif

  protected:

    //! Particles collection the system will manage.
    Particles _particles;

    ClustersArray _clusters;

    //! Emission nodes array of the particle set.
    SourcesArray _sources;

    //! Particle prototypes of the particle set.
    ModelsArray _models;

    //! Emitter objects collection of the system.
    std::vector< Emitter* > emitters;

    //! Updater objects collection of the system.
    std::vector< Updater* > _updaters;

    //! Particle sorter for alpha rendering.
    Sorter* _sorter;

    //! Particles renderer (OpenGL, OSG, etc.)
    Renderer* _renderer;

    std::vector< int > _clusterReference;

    unsigned int _aliveParticles;
    unsigned int _maxParticles;

    bool _renderDeadParticles;
    bool _run;

    ICamera* _camera;

    bool _useExternalCamera;

#ifdef PREFR_USE_OPENMP
    bool _parallel;
#endif
  };

}

#endif /* _PREFR__PARTICLE_SYSTEM__ */
