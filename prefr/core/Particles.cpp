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
#include "Particles.h"

namespace prefr
{

  Particles::Particles( )
  : _size( 0 )
  { }

  Particles::Particles( unsigned int size_ )
  {
    resize( size_ );
  }

  Particles::~Particles( )
  {
    clear( );
  }

  unsigned int Particles::numParticles( void ) const
  {
    return _size;
  }

  void Particles::resize( unsigned int newSize )
  {
    _idVector.resize( newSize );
    _lifeVector.resize( newSize, 0.0f );
    _sizeVector.resize( newSize, 0.0f );
    _positionVector.resize( newSize );
    _colorVector.resize( newSize );
    _velocityModuleVector.resize( newSize, 0.0f );
    _velocityVector.resize( newSize );
    _accelerationVector.resize( newSize );
    _aliveVector.resize( newSize, false );

    _size = newSize;
  }

  void Particles::clear( void )
  {
    _idVector.clear( );
    _lifeVector.clear( );
    _sizeVector.clear( );
    _positionVector.clear( );
    _colorVector.clear( );
    _velocityModuleVector.clear( );
    _velocityVector.clear( );
    _accelerationVector.clear( );
    _aliveVector.clear( );

    _size = 0;
  }

  Particles::iterator Particles::begin( void )
  {
    return _createIterator( 0 );
  }

  Particles::const_iterator Particles::begin( void ) const
  {
    return _createConstIterator( 0 );
  }

  Particles::iterator Particles::end( void )
  {
    return _createIterator( _size );
  }

  Particles::const_iterator Particles::end( void ) const
  {
    return _createConstIterator( _size );
  }

  ParticleRange Particles::range( void )
  {
    ParticleRange result ( this, begin( ), end( ));

    return result;
  }

  Particles::iterator Particles::at( unsigned int i )
  {
    return _createIterator( i );
  }

  Particles::const_iterator Particles::at( unsigned int i ) const
  {
    return _createConstIterator( i );
  }

  Particles::iterator
  Particles::_createIterator( unsigned int i )
  {
    assert( i <= _size );

    iterator it;
    it._data = this;

    it._idIterator = _idVector.begin( ) + i;
    it._lifeIterator = _lifeVector.begin( ) + i;
    it._sizeIterator = _sizeVector.begin( ) + i;
    it._positionIterator = _positionVector.begin( ) + i;
    it._colorIterator = _colorVector.begin( ) + i;
    it._velocityModuleIterator = _velocityModuleVector.begin( ) + i;
    it._velocityIterator = _velocityVector.begin( ) + i;
    it._accelerationIterator = _accelerationVector.begin( ) + i;
    it._aliveIterator = _aliveVector.begin( ) + i;

    it._position = i;
    it._size = _size;

    return it;
  }

  Particles::const_iterator
  Particles::_createConstIterator( unsigned int i ) const
  {
    assert( i <= _size );

    const_iterator it;
    it._data = this;

    it._idConstIterator = _idVector.begin( ) + i;
    it._lifeConstIterator = _lifeVector.begin( ) + i;
    it._sizeConstIterator = _sizeVector.begin( ) + i;
    it._positionConstIterator = _positionVector.begin( ) + i;
    it._colorConstIterator = _colorVector.begin( ) + i;
    it._velocityModuleConstIterator = _velocityModuleVector.begin( ) + i;
    it._velocityConstIterator = _velocityVector.begin( ) + i;
    it._accelerationConstIterator = _accelerationVector.begin( )+ i;
    it._aliveConstIterator = _aliveVector.begin( ) + i;

    it._position = i;
    it._size = _size;

    return it;
  }


  Particles::iterator::iterator( void )
  : _position( 0 )
  , _size( 0 )
  , _data( nullptr )
  { }


  Particles::iterator::iterator( const Particles::iterator& other )
  : _position( other._position )
  , _size( other._size )
  , _data( other._data )
  , _idIterator( other._idIterator )
  , _lifeIterator( other._lifeIterator )
  , _sizeIterator( other._sizeIterator )
  , _positionIterator( other._positionIterator )
  , _colorIterator( other._colorIterator )
  , _velocityModuleIterator( other._velocityModuleIterator )
  , _velocityIterator( other._velocityIterator )
  , _accelerationIterator( other._accelerationIterator )
  , _aliveIterator( other._aliveIterator )
  { }

  Particles::iterator& Particles::iterator::operator++( void )
  {
    _idIterator++;
    _lifeIterator++;
    _sizeIterator++;
    _positionIterator++;
    _colorIterator++;
    _velocityModuleIterator++;
    _velocityIterator++;
    _accelerationIterator++;
    _aliveIterator++;

    _position++;

    return *this;
  }

  Particles::iterator Particles::iterator::operator++( int )
  {
    Particles::iterator result( *this );

    _idIterator++;
    _lifeIterator++;
    _sizeIterator++;
    _positionIterator++;
    _colorIterator++;
    _velocityModuleIterator++;
    _velocityIterator++;
    _accelerationIterator++;
    _aliveIterator++;

    _position++;

    return result;
  }

  Particles::iterator& Particles::iterator::operator--( void )
  {
    _idIterator--;
    _lifeIterator--;
    _sizeIterator--;
    _positionIterator--;
    _colorIterator--;
    _velocityModuleIterator--;
    _velocityIterator--;
    _accelerationIterator--;
    _aliveIterator--;

    _position--;

    return *this;
  }

  Particles::iterator Particles::iterator::operator--( int )
  {
    Particles::iterator result( *this );

    _idIterator--;
    _lifeIterator--;
    _sizeIterator--;
    _positionIterator--;
    _colorIterator--;
    _velocityModuleIterator--;
    _velocityIterator--;
    _accelerationIterator--;
    _aliveIterator--;

    _position--;

    return result;
  }

  Particles::iterator Particles::iterator::operator+( int increase )
  {
    Particles::iterator result( *this );

    result._idIterator += increase;
    result._lifeIterator += increase;
    result._sizeIterator += increase;
    result._positionIterator += increase;
    result._colorIterator += increase;
    result._velocityModuleIterator += increase;
    result._velocityIterator += increase;
    result._accelerationIterator += increase;
    result._aliveIterator += increase;

    result._position += increase;

    return result;
  }

  Particles::iterator Particles::iterator::operator-( int decrease )
  {
    Particles::iterator result( *this );

    result._idIterator -= decrease;
    result._lifeIterator -= decrease;
    result._sizeIterator -= decrease;
    result._positionIterator -= decrease;
    result._colorIterator -= decrease;
    result._velocityModuleIterator -= decrease;
    result._velocityIterator -= decrease;
    result._accelerationIterator -= decrease;
    result._aliveIterator -= decrease;

    result._position -= decrease;

    return result;
  }

  bool Particles::iterator::operator==( const Particles::iterator& other )
  {
    return ( other._data == this->_data &&
             other._size == this->_size &&
             other._position == this->_position );
  }

  bool Particles::iterator::operator!=( const Particles::iterator& other )
  {
    return !(*this == other);
  }

  int Particles::iterator::operator+( const Particles::iterator& other )
  {
    return this->_position + other._position;
  }

  int Particles::iterator::operator-( const Particles::iterator& other )
  {
    return this->_position - other._position;
  }

  Particles::const_iterator::const_iterator( void )
  : _position( 0 )
  , _size( 0 )
  , _data( nullptr )
  { }

  Particles::const_iterator::const_iterator(
      const Particles::const_iterator& other )
  : _position( other._position )
  , _size( other._size )
  , _data( other._data )
  , _idConstIterator( other._idConstIterator )
  , _lifeConstIterator( other._lifeConstIterator )
  , _sizeConstIterator( other._sizeConstIterator )
  , _positionConstIterator( other._positionConstIterator )
  , _colorConstIterator( other._colorConstIterator )
  , _velocityModuleConstIterator( other._velocityModuleConstIterator )
  , _velocityConstIterator( other._velocityConstIterator )
  , _accelerationConstIterator( other._accelerationConstIterator )
  , _aliveConstIterator( other._aliveConstIterator )
  { }

  Particles::const_iterator::const_iterator(
      const Particles::iterator& other )
  : _position( other._position )
  , _size( other._size )
  , _data( other._data )
  , _idConstIterator( other._idIterator )
  , _lifeConstIterator( other._lifeIterator )
  , _sizeConstIterator( other._sizeIterator )
  , _positionConstIterator( other._positionIterator )
  , _colorConstIterator( other._colorIterator )
  , _velocityModuleConstIterator( other._velocityModuleIterator )
  , _velocityConstIterator( other._velocityIterator )
  , _accelerationConstIterator( other._accelerationIterator )
  , _aliveConstIterator( other._aliveIterator )
  { }

  Particles::const_iterator& Particles::const_iterator::operator++( void )
  {

    ++_idConstIterator;
    ++_lifeConstIterator;
    ++_sizeConstIterator;
    ++_positionConstIterator;
    ++_colorConstIterator;
    ++_velocityModuleConstIterator;
    ++_velocityConstIterator;
    ++_accelerationConstIterator;
    ++_aliveConstIterator;

    ++_position;

    return *this;
  }

  Particles::const_iterator Particles::const_iterator::operator++( int )
  {
    Particles::const_iterator result( *this );

    ++_idConstIterator;
    ++_lifeConstIterator;
    ++_sizeConstIterator;
    ++_positionConstIterator;
    ++_colorConstIterator;
    ++_velocityModuleConstIterator;
    ++_velocityConstIterator;
    ++_accelerationConstIterator;
    ++_aliveConstIterator;

    ++_position;

    return result;
  }

  Particles::const_iterator& Particles::const_iterator::operator--( void )
  {

    --_idConstIterator;
    --_lifeConstIterator;
    --_sizeConstIterator;
    --_positionConstIterator;
    --_colorConstIterator;
    --_velocityModuleConstIterator;
    --_velocityConstIterator;
    --_accelerationConstIterator;
    --_aliveConstIterator;

    --_position;

    return *this;
  }

  Particles::const_iterator Particles::const_iterator::operator--( int )
  {
    Particles::const_iterator result( *this );

    --_idConstIterator;
    --_lifeConstIterator;
    --_sizeConstIterator;
    --_positionConstIterator;
    --_colorConstIterator;
    --_velocityModuleConstIterator;
    --_velocityConstIterator;
    --_accelerationConstIterator;
    --_aliveConstIterator;

    --_position;

    return result;
  }

  Particles::const_iterator
  Particles::const_iterator::operator+( int increase )
  {
    Particles::const_iterator result( *this );

    result._idConstIterator += increase;
    result._lifeConstIterator += increase;
    result._sizeConstIterator += increase;
    result._positionConstIterator += increase;
    result._colorConstIterator += increase;
    result._velocityModuleConstIterator += increase;
    result._velocityConstIterator += increase;
    result._accelerationConstIterator += increase;
    result._aliveConstIterator += increase;

    result._position += increase;

    return result;
  }

  Particles::const_iterator
  Particles::const_iterator::operator-( int decrease )
  {
    Particles::const_iterator result( *this );

    result._idConstIterator -= decrease;
    result._lifeConstIterator -= decrease;
    result._sizeConstIterator -= decrease;
    result._positionConstIterator -= decrease;
    result._colorConstIterator -= decrease;
    result._velocityModuleConstIterator -= decrease;
    result._velocityConstIterator -= decrease;
    result._accelerationConstIterator -= decrease;
    result._aliveConstIterator -= decrease;

    result._position -= decrease;

    return result;
  }

  bool
  Particles::const_iterator::operator==( const Particles::const_iterator& other )
  {
    return ( other._data == this->_data &&
             other._size == this->_size &&
             other._position == this->_position );
  }

  bool
  Particles::const_iterator::operator!=( const Particles::const_iterator& other )
  {
    return !( *this == other );
  }

}


