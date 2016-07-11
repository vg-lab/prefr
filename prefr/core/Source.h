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
#ifndef __PREFR__SOURCE__
#define __PREFR__SOURCE__

#include <prefr/api.h>

#include "../utils/types.h"

#include "Particles.h"
#include "Cluster.h"

#include "../utils/Timer.hpp"

namespace prefr
{

  class Cluster;

  class Source
  {
    friend class ParticleSystem;
    friend class Cluster;
    friend class Updater;

  public:

    PREFR_API
    Source( float emissionRate_, const glm::vec3& position_ );

    PREFR_API
    virtual ~Source( void );

    PREFR_API virtual bool Active();
    PREFR_API virtual bool Emits() const;
    PREFR_API virtual bool Continue() const;
    PREFR_API virtual bool Finished();
    PREFR_API virtual void Restart();

    PREFR_API virtual const int& GetBudget();
    PREFR_API virtual void PrepareFrame( const float& deltaTime );
    PREFR_API virtual void CloseFrame();

    PREFR_API virtual void IncreaseAlive();
    PREFR_API virtual void CheckEmissionEnd();

    PREFR_API virtual void ReduceBudgetBy(const unsigned int& decrement = 1);

    PREFR_API virtual glm::vec3 GetEmissionPosition() = 0;
    PREFR_API virtual glm::vec3 GetEmissionVelocityDirection() = 0;

    PREFR_API virtual void maxEmissionCycles( unsigned int cycles );

    PREFR_API Cluster* cluster( void );
    PREFR_API void cluster( Cluster* cluster_ );

  protected:

    virtual void InitializeParticles( void );
    virtual void PrepareParticles( void );

    Cluster* _cluster;

    glm::vec3 _position;

    std::vector< unsigned int > _deadParticles;
    std::vector< unsigned int > _particlesToEmit;

    float _emissionRate;
    unsigned int _totalParticles;

    float _emissionAcc;
    int _particlesBudget;
    bool _active;
    bool _continueEmission;
    bool _finished;

    bool _autoDeactivateWhenFinished;

    bool _killParticlesIfInactive;

    int _lastFrameAliveParticles;

    unsigned int _emittedParticles;
    unsigned int _maxEmissionCycles;
    unsigned int _currentCycle;

  };

  typedef std::vector<Source*> SourcesArray;

  class TimedSource : public Source, public utils::SingleFrameTimer
  {
  public:

    PREFR_API TimedSource( float emissionRate_, glm::vec3 position_ );
    PREFR_API TimedSource( float emissionRate_, glm::vec3 position_,
                           float period, float offset, float duration );

    PREFR_API virtual bool Emits( ) const;

    PREFR_API virtual void CheckEmissionEnd( );

    PREFR_API virtual void PrepareFrame( const float& deltaTime );
    PREFR_API virtual void CloseFrame( );

  };

  //TODO class MultiTimedEmissionNode : public utils::MultiFrameTimer

  class PointSource : public TimedSource
  {
  public:

    PREFR_API PointSource( float emissionRate_, const glm::vec3& position_ );

    PREFR_API virtual ~PointSource();

    PREFR_API virtual void SetEmissionPosition( float x, float y, float z );

    PREFR_API virtual glm::vec3 GetEmissionPosition( );
    PREFR_API virtual glm::vec3 GetEmissionVelocityDirection( );
  };

  class SphereSource : public PointSource
  {
  public:

    float radius;
    float angle;

    glm::vec3 velocity;

    PREFR_API SphereSource( float emissionRate_, const glm::vec3& position_,
                            float radius_ = 0, float angle_ = 360 );

    PREFR_API virtual ~SphereSource( );

    PREFR_API virtual glm::vec3 GetEmissionPosition( );
    PREFR_API virtual glm::vec3 GetEmissionVelocityDirection( );

  };



}


#endif /* __PREFR__SOURCE__ */
