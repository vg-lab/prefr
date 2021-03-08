/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/gmrvvis/prefr>
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
#include "UpdateConfig.h"

#include <reto/reto.h>

#include "../utils/Log.h"
#include "../utils/VectorizedSet.hpp"

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
 class PREFR_API ParticleSystem
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
    
    ParticleSystem( unsigned int maxParticles, ICamera* camera = nullptr );

    /*! \brief Default destructor.
     *
     * Default destructor.
     * */
    
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
    
    virtual void addCluster( Cluster* cluster,
                             const ParticleSet& indices = ParticleSet( ));

    
    virtual void detachCluster( Cluster* cluster );

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
    
    virtual void addSource( Source* source,
                            const ParticleSet& indices = ParticleSet( ) );

    
    virtual void detachSource( Source* source );

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
    
    virtual void addModel( Model* model );

    
    virtual void detachModel( Model* model );

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
    
    virtual void addUpdater( Updater* updater );

    
    virtual void detachUpdater( Updater* updater );

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
    
    virtual void renderer( Renderer* renderer );

    /*! \brief Returns the Renderer object.
     *
     * Returns the Renderer object.
     *
     * @return Renderer object.
     */
    
    Renderer* renderer( void ) const;


    // Pipeline methods

    /*! \brief Prepares the system for execution.
     *
     * Prepares the system for execution. Equivalent to run( true ).
     *
     */
    
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
    
    virtual void render( void ) const;

    // Getters/Setters

    /*! \brief Sets the running state of the system.
     *
     * Sets the running state of the system.
     *
     * @param run true to run, false to stop.
     */
    
    virtual void run( bool run );

    /*! \brief Returns true if the system is running.
     *
     * Returns true if the system is running, false if not.
     *
     * @return true if running, false if not.
     */
    
    virtual bool run( void ) const ;

    
    void resize( unsigned int newSize );

    /*! \brief Returns the current number of alive particles.
     *
     * Returns the current number of alive particles.
     *
     * @return Current number of alive particles.
     */
    
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
    
    void parallel( bool parallelProcessing );

    /*! \brief Returns the collection of cluster objects.
     *
     * Returns the collection of cluster objects.
     *
     * @return Current Cluster array.
     */
    const ClustersArray& clusters( void ) const;

    /*! \brief Returns a created particle collection.
     *
     * Returns a particles' collection created using the given indices.
     *
     * @param indices
     * @return Created collection
     */
    ParticleCollection createCollection( const ParticleSet& indices );


    //TODO
    void releaseParticles( const ParticleCollection& indices );

    void releaseParticles( const ParticleSet& indices );

    /*! \brief Returns a set of available particles.
     *
     * Returns a set with "size" particles (if) available.
     *
     * @param size Max number of particles to be included
     * @return Collection of available particles.
     */
    ParticleCollection retrieveUnused( unsigned int size = 0 );

    ParticleCollection retrieveActive( void );

    ParticleCollection particles( void );

  protected:

    virtual void prepareFrame( float deltaTime );
    virtual void updateFrame( float deltaTime );
    virtual void finishFrame( void );

    /*! Particles collection the system will manage. */
    Particles _particles;

    ParticleCollection _used;
    ParticleCollection _unused;

    ClustersArray _clusters;

    /*! Particle sources array of the particle set. */
    SourcesArray _sources;
    std::vector< Source* > _sourcesVec;

    /*! Particle models. */
    ModelsArray _models;

    /*! Updater objects collection of the system. */
    UpdatersArray _updaters;

    /*! Particle Sorter for composition rendering. */
    Sorter* _sorter;

    /*! Particle Renderer (OpenGL, OSG, etc.). */
    Renderer* _renderer;

    /*! Vectors storing a per-particle pointer to their modifiers. */
    std::vector< Source* > _referenceSources;
    std::vector< Model* > _referenceModels;
    std::vector< Updater* > _referenceUpdaters;

    /*! Vectors storing flags from different stages.*/
    FlagsArray _flagsEmitted;
    FlagsArray _flagsDead;

    UpdateConfig _updateConfig;

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

    unsigned int _lastAlive;

    unsigned int _noVariationFrames;
  };

}

#endif /* _PREFR__PARTICLE_SYSTEM__ */
