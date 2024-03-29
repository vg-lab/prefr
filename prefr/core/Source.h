/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/vg-lab/prefr>
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

#ifndef __PREFR__SOURCE__
#define __PREFR__SOURCE__

#include <prefr/api.h>

#include "../utils/types.h"
#include "../utils/Timer.hpp"

#include "Particles.h"
#include "Cluster.h"
#include "Sampler.h"
#include "UpdateConfig.h"

namespace prefr
{
  class Cluster;
  class Sampler;

  class SampledValues
  {
  public:

    unsigned int index;
    glm::vec3 position;
    glm::vec3 direction;
    glm::mat4 rotation;
  };

  /*! \class Source
   *
   * \brief This class provides the base for the emission of particles for
   * a Cluster object.
   *
   * This class provides the base for the emission of particles for
   * a Cluster object. The Cluster object uses a Source object for handling
   * the emission according to the emission rate with the position, direction
   * and rotation generated by the given Sampler. Emission can be set to
   * be repeated indefinitely or to a certain number of cycles.
   *
   * Note: Reimplement this class to create a different behavior.
   *
   */
  class PREFR_API Source
  {
    friend class ParticleSystem;
    friend class Cluster;
    friend class Updater;

  public:

    
    Source( float emissionRate,
            const glm::vec3& position,
            Sampler* sampler = nullptr );

    
    virtual ~Source( void );

    ParticleCollection& particles( void );
    void particles( const ParticleSet& indices );

     virtual bool active( ) const;
     virtual void active( bool state );

     virtual bool emits( ) const;
     virtual bool continuing( ) const;
     virtual bool finished( ) const;
     virtual void restart( );

     virtual const int& budget( ) const;
     virtual void prepareFrame( const float& deltaTime );
     virtual void closeFrame( );

     virtual void maxEmissionCycles( unsigned int cycles );

     void sampler( Sampler* sampler );
     const Sampler* sampler( void ) const;

     glm::vec3 position( void ) const;
     virtual void sample( SampledValues* );

     unsigned int aliveParticles( void ) const;

     void autoDeactivateWhenFinished( bool state );
     void killParticlesWhenInactive( bool state );

  protected:

    virtual void _finishFrame( void );
    virtual void _checkEmissionEnd( void );
    virtual void _checkFinished( void );

    virtual void _initializeParticles( void );
    virtual void _prepareParticles( void );

    ParticleCollection _particles;
    UpdateConfig* _updateConfig;

    Sampler* _sampler;

    glm::vec3 _position;

    std::unordered_map< unsigned int, unsigned int > _emittedIndices;

    unsigned int _particlesToEmit;

    unsigned int _aliveParticles;
    unsigned int _lastFrameAliveParticles;

    unsigned int _currentFrameEmittedParticles;
    unsigned int _emittedParticles;

    float _emissionRate;

    float _emissionAcc;
    int _particlesBudget;

    bool _active;
    bool _continueEmission;
    bool _finished;

    unsigned int _maxEmissionCycles;
    unsigned int _currentCycle;

    bool _autoDeactivateWhenFinished;
    bool _killParticlesIfInactive;

  };

  typedef VectorizedSet< Source* > SourcesArray;

  /*! \class TimedSource
   *
   * \brief This class provides the feature of emitting within a time frame.
   *
   * This class adds the base Source the feature of establishing a
   * time frame in which particles will be emitted. This time window will be
   * restarted after finishing, allowing to emit repeatedly according to the
   * set period.
   *
   * @see utils::SingleFrameTimer
   * @see Source
   *
   */
  class PREFR_API TimedSource : public Source, public utils::SingleFrameTimer
  {
  public:

     TimedSource( float emissionRate_, glm::vec3 position_ );
     TimedSource( float emissionRate_, glm::vec3 position_,
                           float period, float offset, float duration );

     virtual bool emits( ) const;

     virtual void _checkEmissionEnd( );

     virtual void prepareFrame( const float& deltaTime );
     virtual void closeFrame( );

  };
}

#endif /* __PREFR__SOURCE__ */
