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

#include "DistanceArray.hpp"
#include "RenderConfig.h"

#include "Particles.h"

#include "Model.h"
#include "Renderer.h"
#include "Sorter.h"
#include "Source.h"
#include "Updater.h"

#include <reto/reto.h>

#include "../utils/Log.h"

namespace prefr
{

  /*! \class ParticleSystem
   *
   * \brief This class provides the common inheritable framework for creating a
   * particle system.
   *
   * This class provides the common inheritable framework for creating a
   * particle system. This class establishes a common framework which
   * should handle the general behavior of the particle system. It is in
   * charge of providing several external calls in order to control the
   * particle system working. This class provides basic functionality
   * concerning particle sources, emission and updating. This framework
   * is particle-focused, i.e. the main entity used in the system
   * is the particle itself. Inherit from this class in order to create
   * customized behavior and features when communication between
   * different elements than the ones introduced in the core package.
   *
   * The pipeline consists of a series of methods that should be called in
   * the following order:
   *
   * - Constructor( maxParticles [,  cameraObject ])
   * - start( )/run( true )
   * - update( deltaTime )
   * - updateCameraDistances( [ cameraPosition ]) Note: Optional if ICamera
   * object provided.
   * - updateRender( )
   * - render( )
   *
   * Depending on the desired behaviors and the inherited implementation some
   * steps of the pipeline could be avoided such as sorting.
   *
   */
 class ParticleSystem
 {

 public:

   /*! \brief Default constructor.
    *
    * This constructor method creates the particle system with the
    * given number of particles. As optional parameter an instance
    * of a class inheriting from ICamera can be passed in order
    * to manage automatically the camera data retrieved each frame.
    *
    * @param maxParticles Number of particles to be potentially used.
    * @param camera (Optional) ICamera inherited class instance pointer.
    *
    * @see ICamera
    *
    *  */
    PREFR_API
    ParticleSystem( unsigned int maxParticles, ICamera* camera = nullptr );

    /*! \brief Default destructor.
     *
     * Default destructor.
     * */
    PREFR_API
    virtual ~ParticleSystem( );

    // Configuration methods

    /*! \brief Adds a Cluster to the system specifying the range of particles
     * handled by the cluster. Cluster object is appended to the Cluster objects
     * array.
     *
     * @param cluster Cluster object to be added.
     * @param start Start position for the used particles.
     * @param size_ Number of particles handled by the cluster.
     *
     * @see Cluster
     * @see ParticleSystem::update
     *
     */
    PREFR_API
    virtual void addCluster( Cluster* cluster,
                             unsigned int start,
                             unsigned int size );

    /*! \brief Adds a Source object to the particle system.
     *
     * Adds a Source object to the particle system. The object is appended to
     * the Source objects array.
     *
     * @param source Source object to be added.
     *
     * @see Source
     * @see ParticleSystem::update
     *
     */
    PREFR_API
    virtual void addSource( Source* source );

    /*! \brief Adds a Model object to the system.
     *
     * Adds a Model object to the system. The object is appended to the Model
     * objects array.
     *
     * @see Model
     * @see ParticleSystem::update
     *
     * @param model Model object to be added.
     */
    PREFR_API
    virtual void addModel( Model* model );

    /*! \brief Adds an Updater object to the system.
     *
     * Adds an Updater object to the system. The object is appended to
     * the Updater objects array.
     *
     * @param updater Updater object to be added.
     *
     * @see Updater
     * @see ParticleSystem::update
     *
     */
    PREFR_API
    virtual void addUpdater( Updater* updater );

    /*! \brief Sets the Sorter object.
     *
     * Sets the Sorter object. This sorter object performs the Sort operation
     * taking place within the ParticleSystem::UpdateRender method.
     *
     * @param sorter Sorter object.
     *
     * @see Sorter
     * @see updateRender
     *
     */
    PREFR_API
    virtual void sorter( Sorter* sorter );

    /*! \brief Returns the sorter.
     *
     * Returns the sorter object.
     *
     * @return Assigned Sorter object.
     *
     * @see Sorter
     * @see updateRender
     *
     */
    PREFR_API
    virtual Sorter* sorter( void ) const;

    /*! \brief Sets the Renderer object.
     *
     * Sets the Renderer object. This Renderer object will perform the
     * UpdateRender and Render operations within the
     * ParticleSystem::updateRender and ParticleSytem::render respectively.
     *
     * @param renderer Renderer object to be assigned.
     *
     * @see Renderer
     * @see updateRender
     * @see render
     *
     */
    PREFR_API
    virtual void renderer( Renderer* renderer );

    /*! \brief Returns the Renderer object.
     *
     * Returns the Renderer object.
     *
     * @return Renderer object.
     */
    PREFR_API
    Renderer* renderer( void ) const;


    // Pipeline methods

    /*! \brief Prepares the system for execution.
     *
     * Prepares the system for execution. Equivalent to run( true ).
     *
     */
    PREFR_API
    virtual void start( void );

    /*! \brief Particle updating method.
     *
     * This method updates the properties of the particles each frame.
     *
     * @param deltaTime Current delta time. In default Updater class, all
     * variances in the properties will be computed multiplying by delta time.
     *
     * @see Updater
     * @see Model
     * @see Source
     *
     *  */
    PREFR_API
    virtual void update( const float& deltaTime );

    /*! \brief Computes particles distance to camera.
     *
     * Calculates the distance of each particle to the given camera position
     * for the latter sorting operation (if using composite rendering).
     *
     * @param cameraPosition Current camera position.
     *
     * @see Sorter
     *
     *  */
    PREFR_API
    virtual void updateCameraDistances( const glm::vec3& cameraPosition );

    /*! \brief Computes particles distance to camera (only with integrated
     * ICamera object).
     *
     * Method that calculates particles' distances to camera from the integrated
     * ICamera object.
     *
     * @see UpdateCameraDistances
     * @see Sorter
     * @see ICamera
     *  */
    PREFR_API
    virtual void updateCameraDistances( void );

    /*! \brief Particle render updating method
     *
     * This method prepares the latter render process as an intermediate
     * step between calculating camera distance and rendering by performing
     * several operations such as sorting, serializing, etc.).
     *
     * @see Renderer
     * @see UpdateCameraDistances
     * @see Render
     *
     * */
    PREFR_API
    virtual void updateRender( void );

    /*! \brief Rendering method.
     *
     * This method launches the rendering process through the Renderer object.
     * Consider calling this method alone if no changes have been made since
     * the last frame (camera position/orientation, colors, positions, etc.).
     *
     * Note: This method should be called always after UpdateRender.
     *
     * @see Renderer
     * @see UpdateRender
     *
     */
    PREFR_API
    virtual void render( void ) const;

    // Getters/Setters

    /*! \brief Sets the running state of the system.
     *
     * Sets the running state of the system.
     *
     * @param run true to run, false to stop.
     */
    PREFR_API
    virtual void run( bool run );

    /*! \brief Returns true if the system is running.
     *
     * Returns true if the system is running, false if not.
     *
     * @return true if running, false if not.
     */
    PREFR_API
    virtual bool run( void ) const ;

    /*! \brief Returns the current number of alive particles.
     *
     * Returns the current number of alive particles.
     *
     * @return Current number of alive particles.
     */
    PREFR_API
    virtual unsigned int aliveParticles( void ) const;

    /*! \brief Activates the rendering of dead particles.
     *
     * Activates the rendering of dead particles. By default, the rendering
     * of dead particles is deactivated.
     *
     * Note: This feature is not meant to be used for the expected general
     * cases. If activated, a customized Updater object should be used in order
     * to keep particles properly valued.
     *
     * @param renderDead true to render dead particles.
     */
    virtual void renderDeadParticles( bool renderDead );

    /*! \brief Activates the (multicore) parallel execution of the engine.
     *
     * Activates the (multicore) parallel execution of the engine. True for
     * activating, false for deactivating. Parallel execution will use
     * all the resources provided by OpenMP, as it is not configured for
     * a particular number of cores.
     *
     * @param parallelProcessing True to activate, false to deactivate.
     */
    PREFR_API
    void parallel( bool parallelProcessing );

    /*! \brief Returns the collection of cluster objects.
     *
     * Returns the collection of cluster objects.
     *
     * @return Current Cluster array.
     */
    const ClustersArray& clusters( void ) const;

  protected:

    /*! Particles collection the system will manage. */
    Particles _particles;

    ClustersArray _clusters;

    /*! Particle sources array of the particle set. */
    SourcesArray _sources;

    /*! Particle models. */
    ModelsArray _models;

    /*! Updater objects collection of the system. */
    std::vector< Updater* > _updaters;

    /*! Particle Sorter for composition rendering. */
    Sorter* _sorter;

    /*! Particle Renderer (OpenGL, OSG, etc.). */
    Renderer* _renderer;

    /*! Vector storing a per-particle index to its cluster. */
    std::vector< int > _clusterReference;

    /*! Stores the number of currently alive particles (current frame). */
    unsigned int _aliveParticles;

    /*! Stores the number of maximum particles available.*/
    unsigned int _maxParticles;

    /*! Flag for not discarding dead particles on rendering tasks. */
    bool _renderDeadParticles;

    /*! Flag signaling whether the system will update and render. */
    bool _run;

    /*! External camera interface object. */
    ICamera* _camera;

    /*! Flag indicating if using an integrated external camera. */
    bool _useExternalCamera;

    /*! Flag indicating if the system will run concurrently. */
    bool _parallel;
  };

}

#endif /* _PREFR__PARTICLE_SYSTEM__ */
