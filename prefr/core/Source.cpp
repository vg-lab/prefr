/*
 * Copyright (c) 2014-2018 GMRV/URJC.
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

#include "../utils/types.h"

namespace prefr
{
    Source::Source( float emissionRate_,
                    const glm::vec3& position_,
                    Sampler* sampler_ )
    : _updateConfig( nullptr )
    , _sampler( sampler_ )
    , _position( position_ )
    , _particlesToEmit( 0 )
    , _aliveParticles( 0 )
    , _emissionRate( emissionRate_ )
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
    { }

    Source::~Source( void )
    { }

    ParticleCollection& Source::particles( void )
    {
      return _particles;
    }

    bool Source::active( )
    {
      return _active;
    }

    bool Source::emits( ) const
    {
      return _active && _particlesToEmit > 0 && continuing( );
    }

    bool Source::continuing( ) const
    {
      return _continueEmission;
    }

    bool Source::finished( )
    {
      return _finished;
    }

    const int& Source::budget( )
    {
      return _particlesBudget;
    }

    void Source::restart( )
    {
      _currentCycle = 0;
      _emittedParticles = 0;
      _continueEmission = true;

      for( auto const & particle : _particles)
      {
        _updateConfig->setEmitted( particle.id( ), false );
        _updateConfig->setDead( particle.id( ), true );
      }

      _particlesToEmit = _particles.size( );
    }

    void Source::prepareFrame( const float& deltaTime )
    {
      assert( _particles.size( ) > 0 );

      // Compute raw budget, as it can be zero along several consecutive frames
      float rawBudget =
          deltaTime * ( float ) _particles.size( ) * _emissionRate;

      // Accumulate budget to emit as soon as it reaches a unit
      _emissionAcc += rawBudget;
      _particlesBudget = int( floor( _emissionAcc ));
      _emissionAcc -= _particlesBudget;

      _lastFrameAliveParticles = 0;

      _prepareParticles( );
    }

    void Source::increaseAlive( )
    {
      _lastFrameAliveParticles++;
    }

    void Source::checkEmissionEnd( )
    {
      if( _maxEmissionCycles > 0 )
      {

        if( _emittedParticles >= _particles.size( ) )
        {
         _currentCycle++;
         _emittedParticles -= _particles.size( );
        }

        this->_continueEmission = !( _currentCycle >= _maxEmissionCycles );

      }
    }

    void Source::maxEmissionCycles( unsigned int cycles )
    {
      _maxEmissionCycles = cycles;
    }


    void Source::closeFrame( )
    {
      _particlesBudget = 0;

      _aliveParticles = 0;

      for( auto const & particle : _particles )
      {
        _updateConfig->setEmitted( particle.id( ), false );

        if( particle.alive( ))
        {
          _aliveParticles++;
        }
      }

      checkEmissionEnd( );

      this->_finished = !_continueEmission && _lastFrameAliveParticles == 0 ;

      if ( _finished && _autoDeactivateWhenFinished )
        this->_active = false;
    }


    void Source::sampler( Sampler* sampler_ )
    {
      _sampler = sampler_;
    }

    const Sampler* Source::sampler( void ) const
    {
      return _sampler;
    }

    glm::vec3 Source::position( void ) const
    {
      return _position;
    }

    void Source::sample( SampledValues* sampledValues ) const
    {
      assert( _sampler );

      _sampler->sample( *this, sampledValues );

    }

    void Source::_initializeParticles( void )
    {
      if( _particles.empty( ))
      {
        std::cout << "Particles cannot be configured. Collection size is zero." << std::endl;
        return;
      }

      for( auto const & particle : _particles )
      {
        _updateConfig->setDead( particle.id( ), true );
        _updateConfig->setEmitted( particle.id( ), false );
      }

    }

    void Source::_prepareParticles( void )
    {
      if( _emissionRate <= 0.0f )
      {
        for( auto const & particle : _particles )
        {
          if( _updateConfig->dead( particle.id( )))
          {
            _updateConfig->setEmitted( particle.id( ), true );
            _updateConfig->setDead( particle.id( ), false );

            ++_lastFrameAliveParticles;
          }
        }
      }
      else
      {
        // Fill dead pool for the emission for this frame
        for( auto const& particle : _particles )
        {
          if( _particlesBudget == 0 )
            break;

          if( !_updateConfig->dead( particle.id( )))
            continue;

          _updateConfig->setEmitted( particle.id( ), true );
          _updateConfig->setDead( particle.id( ), false );

          --_particlesBudget;
          ++_lastFrameAliveParticles;

        }
      }

      _emittedParticles += _lastFrameAliveParticles;
    }

    unsigned int Source::aliveParticles( void ) const
    {
      return _aliveParticles;
    }

    TimedSource::TimedSource( float emissionRate_, glm::vec3 position_ )
    : Source( emissionRate_, position_ )
    , SingleFrameTimer( 0, 0, 0 )
    { }

    TimedSource::TimedSource( float emissionRate_, glm::vec3 position_,
                              float period, float offset, float duration )
    : Source( emissionRate_, position_ )
    , SingleFrameTimer( period, offset, duration )
    { }

    bool TimedSource::emits( ) const
    {
      return InTime( ) && Source::emits( );
    }

    void TimedSource::checkEmissionEnd( )
    {
      if( _maxEmissionCycles > 0 )
      {

        if( _emittedParticles >= _particles.size( ) )
        {
          _currentCycle++;
          _emittedParticles -= _particles.size( );
        }
        else if( AfterTime( ))
        {
          _currentCycle++;
          _emittedParticles = 0;
        }

        this->_continueEmission = !( _currentCycle >= _maxEmissionCycles );

      }
    }

    void TimedSource::prepareFrame( const float& deltaTime )
    {
      Source::prepareFrame( deltaTime );

      UpdateTimer( deltaTime );
    }

    void TimedSource::closeFrame( )
    {
      Source::closeFrame( );

      RestoreTimer( );
    }
}
