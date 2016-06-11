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
#include "Source.h"
#include "types.h"

namespace prefr
{

    static float invRandMax = 1.0f / RAND_MAX;
    static float pi2 = 2.0f * float(M_PI);

    glm::vec3 GetRandomDirection(float thetaAngle = pi2)
    {
      float theta, phi, vxz;

      theta = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * thetaAngle;//asinf(clamp(rand()*invRandMax, 0.0f, 1.0f));
      phi = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * pi2;
      vxz = sinf(theta);

      return glm::vec3 (cosf(phi)*vxz, cosf(theta), sinf(phi)*vxz);
    }


    //***********************************************************
    // EMISSION NODE
    //***********************************************************

    Source::Source( float emissionRate_, const glm::vec3& position_ )
    : _cluster( nullptr )
    , _position( position_ )
    , _emissionRate( emissionRate_ )
    , _totalParticles( 0 )
    , _emissionAcc( 0 )
    , _particlesBudget( 0 )
    , _active( true )
    , _continueEmission( true )
    , _finished( false )
    , _autoDeactivateWhenFinished( true )
    , _killParticlesIfInactive( false )
    , _lastFrameAliveParticles( 0 )
    , _emittedParticles( 0 )
    , _maxEmissionCycles( 0 )
    , _currentCycle( 0 )
    {

    }

    Source::~Source( void )
    {

    }

    bool Source::Active()
    {
      return _active;
    }

    bool Source::Emits() const
    {
//      return true;
//      return _active && _particlesBudget > 0 && Continue();
      return _active && _particlesToEmit.size( ) > 0 && Continue( );
    }

    bool Source::Continue() const
    {
      return _continueEmission;
    }

    bool Source::Finished()
    {
      return _finished;
    }

    const int& Source::GetBudget()
    {
      return _particlesBudget;
    }

    void Source::Restart()
    {
      _currentCycle = 0;
      _emittedParticles = 0;
      _continueEmission = true;

      for( tparticle particle = _cluster->particles( ).begin( );
           particle != _cluster->particles( ).end( ); ++particle)
      {
        _deadParticles.push_back( particle.id( ));
        _particlesToEmit.clear( );
      }
    }

    void Source::PrepareFrame( const float& deltaTime )
    {
      assert( _cluster );

      // Compute raw budget, as it can be zero along several consecutive frames
      float rawBudget = deltaTime * _cluster->particles( ).size * _emissionRate;

      // Accumulate budget to emit as soon as it reaches a unit
      _emissionAcc += rawBudget;
      _particlesBudget = int(floor(_emissionAcc));
      _emissionAcc -= _particlesBudget;

      _lastFrameAliveParticles = 0;

      _emittedParticles = _particlesBudget - ( _particlesBudget - _deadParticles.size( ));

      // Fill dead pool for the emission for this frame
      while( _particlesBudget > 0 && _deadParticles.size( ) > 0 )
      {
        assert( _particlesBudget >= 0 );

        _particlesToEmit.push_back( _deadParticles.back( ));
        _deadParticles.pop_back( );

        --_particlesBudget;
      }
    }

    void Source::IncreaseAlive()
    {
      _lastFrameAliveParticles++;
    }

    void Source::CheckEmissionEnd()
    {
      if (_maxEmissionCycles > 0)
      {

        if (_emittedParticles >= _totalParticles )
        {
         _currentCycle++;
         _emittedParticles -= _totalParticles;
        }

        this->_continueEmission = !(_currentCycle >= _maxEmissionCycles);

      }
    }


    void Source::ReduceBudgetBy(const unsigned int& decrement)
    {
      _particlesBudget -= decrement;
      _emittedParticles += decrement;
    }


    void Source::CloseFrame()
    {
      _particlesBudget = 0;
      _particlesToEmit.clear( );

      CheckEmissionEnd();

      this->_finished = !_continueEmission && _lastFrameAliveParticles == 0 ;

      if (_finished && _autoDeactivateWhenFinished)
        this->_active = false;
    }


    Cluster* Source::cluster( void )
    {
      return _cluster;
    }

    void Source::cluster( Cluster* cluster_ )
    {
      _cluster = cluster_;

      if( _cluster->particles( ).size > 0 && _deadParticles.size( ) == 0)
      {
        InitializeParticles( );
      }
    }

    void Source::InitializeParticles( void )
    {
      if( !_cluster || _cluster->particles( ).size == 0)
      {
        std::cout << "Particles cannot be configured." << std::endl;
        return;
      }

      _totalParticles = _cluster->particles( ).size;
      _deadParticles.resize( _totalParticles );
      _particlesToEmit.resize( _totalParticles );

      for( tparticle particle = _cluster->particles( ).begin( );
                   particle != _cluster->particles( ).end( ); ++particle )
      {
        if( !particle.alive( ))
          _deadParticles.push_back( particle.id( ));
      }
    }

    //***********************************************************
    // TIMED EMISSION NODE
    //***********************************************************

    TimedSource::TimedSource( float emissionRate_, glm::vec3 position_ )
    : Source( emissionRate_, position_ )
    , SingleFrameTimer( 0, 0, 0 )
    {}

    TimedSource::TimedSource( float emissionRate_, glm::vec3 position_,
                              float period, float offset, float duration)
    : Source( emissionRate_, position_ )
    , SingleFrameTimer( period, offset, duration )
    {}

    bool TimedSource::Emits()
    {
      return InTime() && Source::Emits();
    }

    void TimedSource::CheckEmissionEnd()
    {
      if (_maxEmissionCycles > 0)
      {

        if (_emittedParticles >= _totalParticles)
        {
          _currentCycle++;
          _emittedParticles -= _totalParticles;
        }
        else if (AfterTime())
        {
          _currentCycle++;
          _emittedParticles = 0;
        }

        this->_continueEmission = !(_currentCycle >= _maxEmissionCycles);

      }
    }

    void TimedSource::PrepareFrame( const float& deltaTime )
    {
      Source::PrepareFrame( deltaTime);

      UpdateTimer(deltaTime);
    }

    void TimedSource::CloseFrame()
    {
      Source::CloseFrame();

      RestoreTimer();

//      this->finished = !continueEmission && lastFrameAliveParticles == 0 ;

    }

    //***********************************************************
    // POINT EMISSION NODE
    //***********************************************************


    PointSource::PointSource( float emissionRate_,
                                          glm::vec3 position_)
    : TimedSource( emissionRate_, position_ )
    {}

    PointSource::~PointSource()
    {}

    void PointSource::SetEmissionPosition(float x, float y, float z)
    {
      _position = glm::vec3(x, y, z);
    }

    glm::vec3 PointSource::GetEmissionPosition()
    {
      return _position;
    }

    glm::vec3 PointSource::GetEmissionVelocityDirection()
    {
      return GetRandomDirection();
    }



    SphereSource::SphereSource( float emissionRate_, glm::vec3 position_,
                                            float radius_, float angle_)
    : PointSource( emissionRate_, position_ )
    , radius( radius_ )
    , angle( glm::radians(angle_) )
    {}

    SphereSource::~SphereSource()
    {}

    glm::vec3 SphereSource::GetEmissionPosition()
    {
      return _position + (radius * velocity);
    }

    glm::vec3 SphereSource::GetEmissionVelocityDirection()
    {
      velocity = GetRandomDirection();
      return velocity;
    }

}


