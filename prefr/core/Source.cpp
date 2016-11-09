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

#include "../utils/types.h"

namespace prefr
{
    Source::Source( float emissionRate_,
                    const glm::vec3& position_,
                    Sampler* sampler_ )
    : _cluster( nullptr )
    , _sampler( sampler_ )
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

    bool Source::active( )
    {
      return _active;
    }

    bool Source::emits( ) const
    {
      return _active && _particlesToEmit.size( ) > 0 && continuing( );
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

      for( tparticle particle = _cluster->particles( ).begin( );
           particle != _cluster->particles( ).end( ); ++particle )
      {
        _deadParticles.push_back( particle.id( ));
        _particlesToEmit.clear( );
      }
    }

    void Source::prepareFrame( const float& deltaTime )
    {
      assert( _cluster );

      // Compute raw budget, as it can be zero along several consecutive frames
      float rawBudget =
          deltaTime * ( float ) _cluster->particles( ).size * _emissionRate;

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

        if( _emittedParticles >= _totalParticles )
        {
         _currentCycle++;
         _emittedParticles -= _totalParticles;
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
      _particlesToEmit.clear( );

      checkEmissionEnd( );

      this->_finished = !_continueEmission && _lastFrameAliveParticles == 0 ;

      if ( _finished && _autoDeactivateWhenFinished )
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
        _initializeParticles( );
      }
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
      if( !_cluster || _cluster->particles( ).size == 0 )
      {
        std::cout << "Particles cannot be configured." << std::endl;
        return;
      }

      _totalParticles = _cluster->particles( ).size;
      _deadParticles.resize( _totalParticles );

      std::vector< unsigned int >::iterator deadIt = _deadParticles.begin( );
      for( tparticle particle = _cluster->particles( ).begin( );
          particle != _cluster->particles( ).end( ); ++particle, ++deadIt )
      {
        if( !particle.alive( ))
          *deadIt = particle.id( );
      }
    }

    void Source::_prepareParticles( void )
    {
      if( _emissionRate <= 0.0f )
      {
        for( auto particle : _deadParticles )
        {
          _particlesToEmit.push_back( particle );
          ++_lastFrameAliveParticles;
        }

        _deadParticles.clear( );
      }
      else
      {
        // Fill dead pool for the emission for this frame
        while( _particlesBudget > 0 && _deadParticles.size( ) > 0 )
        {
          assert( _particlesBudget >= 0 );

          _particlesToEmit.push_back( _deadParticles.back( ));
          _deadParticles.pop_back( );

          --_particlesBudget;
          ++_lastFrameAliveParticles;
        }
      }

      _emittedParticles += _lastFrameAliveParticles;
    }

    std::vector< unsigned int >& Source::deadParticles( void )
    {
      return _deadParticles;
    }

    std::vector< unsigned int >& Source::particlesToEmit( void )
    {
      return _particlesToEmit;
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

        if( _emittedParticles >= _totalParticles )
        {
          _currentCycle++;
          _emittedParticles -= _totalParticles;
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
