/*
 * ParticleEmissionNode.h
 *
 *  Created on: 11/12/2014
 *      Author: sgalindo
 */

#ifndef __ParticleEmissionNode__
#define __ParticleEmissionNode__

#include <prefr/api.h>
#include "types.h"
#include "Particles.h"
#include "Cluster.h"
#include "Timer.hpp"

namespace prefr
{

  class Cluster;

  class Source
  {
    friend class ParticleSystem;
    friend class Cluster;

  public:

    PREFR_API
    Source( float emissionRate_, const glm::vec3& position_ );

    PREFR_API
    virtual ~Source( void );

    PREFR_API virtual bool Active();
    PREFR_API virtual bool Emits();
    PREFR_API virtual bool Continue();
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

    Cluster* cluster( void );
    void cluster( Cluster* cluster_ );

  protected:

      Cluster* _cluster;

      glm::vec3 _position;

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

    PREFR_API virtual bool Emits();

    PREFR_API virtual void CheckEmissionEnd();

    PREFR_API virtual void PrepareFrame( const float& deltaTime );
    PREFR_API virtual void CloseFrame();

  };

  //TODO class MultiTimedEmissionNode : public utils::MultiFrameTimer

  class PointSource : public TimedSource
  {
  public:

    PREFR_API PointSource( float emissionRate_, glm::vec3 position_ );

    PREFR_API virtual ~PointSource();

    PREFR_API virtual void SetEmissionPosition(float x, float y, float z);

    PREFR_API virtual glm::vec3 GetEmissionPosition();
    PREFR_API virtual glm::vec3 GetEmissionVelocityDirection();
  };

  class SphereSource : public PointSource
  {
  public:

    float radius;
    float angle;

    glm::vec3 velocity;

    PREFR_API SphereSource( float emissionRate_, glm::vec3 position_,
                                  float radius_ = 0, float angle_ = 360 );

    PREFR_API virtual ~SphereSource();

    PREFR_API virtual glm::vec3 GetEmissionPosition();
    PREFR_API virtual glm::vec3 GetEmissionVelocityDirection();

  };



}


#endif /* __ParticleEmissionNode__ */
