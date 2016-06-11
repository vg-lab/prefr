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
    _newbornVector.resize( newSize, false );

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
    _newbornVector.clear( );

    _size = 0;
  }

  Particles::iterator Particles::begin( void )
  {
    return CreateIterator( 0 );
  }

  Particles::const_iterator Particles::begin( void ) const
  {
    return CreateConstIterator( 0 );
  }

  Particles::iterator Particles::end( void )
  {
    return CreateIterator( _size );
  }

  Particles::const_iterator Particles::end( void ) const
  {
    return CreateConstIterator( _size );
  }

  ParticleRange Particles::range( void )
  {
    ParticleRange result ( this, begin( ), end( ));

    return result;
  }

  Particles::iterator Particles::at( unsigned int i )
  {
    return CreateIterator( i );
  }

  Particles::const_iterator Particles::at( unsigned int i) const
  {
    return CreateConstIterator( i );
  }

  Particles::iterator
  Particles::CreateIterator( unsigned int i )
  {
    assert( i <= _size );

    iterator it;
    it._data = this;

    it.idIterator = _idVector.begin( ) + i;
    it.lifeIterator = _lifeVector.begin( ) + i;
    it.sizeIterator = _sizeVector.begin( ) + i;
    it.positionIterator = _positionVector.begin( ) + i;
    it.colorIterator = _colorVector.begin( ) + i;
    it.velocityModuleIterator = _velocityModuleVector.begin( ) + i;
    it.velocityIterator = _velocityVector.begin( ) + i;
    it.accelerationIterator = _accelerationVector.begin( ) + i;
    it.aliveIterator = _aliveVector.begin( ) + i;
    it.newbornIterator = _newbornVector.begin( ) + i;

    it._position = i;
    it._size = _size;

    return it;
  }

  Particles::const_iterator
  Particles::CreateConstIterator( unsigned int i ) const
  {
    assert( i <= _size );

    const_iterator it;
    it._data = this;

    it.idConstIterator = _idVector.begin( ) + i;
    it.lifeConstIterator = _lifeVector.begin( ) + i;
    it.sizeConstIterator = _sizeVector.begin( ) + i;
    it.positionConstIterator = _positionVector.begin( ) + i;
    it.colorConstIterator = _colorVector.begin( ) + i;
    it.velocityModuleConstIterator = _velocityModuleVector.begin( ) + i;
    it.velocityConstIterator = _velocityVector.begin( ) + i;
    it.accelerationConstIterator = _accelerationVector.begin( )+ i;
    it.aliveConstIterator = _aliveVector.begin( ) + i;
    it.newbornConstIterator = _newbornVector.begin( ) + i;

    it._position = i;
    it._size = _size;

    return it;
  }



//  Particles::base_iterator::base_iterator( void )
//  : _position( 0 )
//  , _size( 0 )
//  , _data( nullptr )
//  { }
//
//  Particles::base_iterator::base_iterator( const base_iterator& other )
//  : _position( other._position )
//  , _size( other._size )
//  , _data( other._data )
//  { }
//
//  Particles::base_iterator::~base_iterator( )
//  { }
//
//  bool Particles::base_iterator::operator== (
//      const Particles::base_iterator& other )
//  {
//    return ( other._data == this->_data &&
//             other._size == this->_size &&
//             other._position == this->_position );
//  }
//
//  bool Particles::base_iterator::operator!= (
//        const Particles::base_iterator& other )
//  {
//    return !(*this == other);
//  }

  Particles::iterator::iterator( void )
  : _position( 0 )
  , _size( 0 )
  , _data( nullptr )
  { }


  Particles::iterator::iterator( const Particles::iterator& other )
  : _position( other._position )
  , _size( other._size )
  , _data( other._data )
  , idIterator( other.idIterator )
  , lifeIterator( other.lifeIterator )
  , sizeIterator( other.sizeIterator )
  , positionIterator( other.positionIterator )
  , colorIterator( other.colorIterator )
  , velocityModuleIterator( other.velocityModuleIterator )
  , velocityIterator( other.velocityIterator )
  , accelerationIterator( other.accelerationIterator )
  , aliveIterator( other.aliveIterator )
  , newbornIterator( other.newbornIterator )

  { }

  Particles::iterator& Particles::iterator::operator++( void )
  {
    idIterator++;
    lifeIterator++;
    sizeIterator++;
    positionIterator++;
    colorIterator++;
    velocityModuleIterator++;
    velocityIterator++;
    accelerationIterator++;
    aliveIterator++;
    newbornIterator++;

    _position++;

    return *this;
  }

  Particles::iterator Particles::iterator::operator++( int )
  {
    Particles::iterator result( *this );

    idIterator++;
    lifeIterator++;
    sizeIterator++;
    positionIterator++;
    colorIterator++;
    velocityModuleIterator++;
    velocityIterator++;
    accelerationIterator++;
    aliveIterator++;
    newbornIterator++;

    _position++;

    return result;
  }

  Particles::iterator& Particles::iterator::operator--( void )
  {
    idIterator--;
    lifeIterator--;
    sizeIterator--;
    positionIterator--;
    colorIterator--;
    velocityModuleIterator--;
    velocityIterator--;
    accelerationIterator--;
    aliveIterator--;
    newbornIterator--;

    _position--;

    return *this;
  }

  Particles::iterator Particles::iterator::operator--( int )
  {
    Particles::iterator result( *this );

    idIterator--;
    lifeIterator--;
    sizeIterator--;
    positionIterator--;
    colorIterator--;
    velocityModuleIterator--;
    velocityIterator--;
    accelerationIterator--;
    aliveIterator--;
    newbornIterator--;

    _position--;

    return result;
  }

  Particles::iterator Particles::iterator::operator+( int increase )
  {
    Particles::iterator result( *this );

    result.idIterator += increase;
    result.lifeIterator += increase;
    result.sizeIterator += increase;
    result.positionIterator += increase;
    result.colorIterator += increase;
    result.velocityModuleIterator += increase;
    result.velocityIterator += increase;
    result.accelerationIterator += increase;
    result.aliveIterator += increase;
    result.newbornIterator += increase;

    result._position += increase;

    return result;
  }

  Particles::iterator Particles::iterator::operator-( int decrease )
  {
    Particles::iterator result( *this );

    result.idIterator -= decrease;
    result.lifeIterator -= decrease;
    result.sizeIterator -= decrease;
    result.positionIterator -= decrease;
    result.colorIterator -= decrease;
    result.velocityModuleIterator -= decrease;
    result.velocityIterator -= decrease;
    result.accelerationIterator -= decrease;
    result.aliveIterator -= decrease;
    result.newbornIterator -= decrease;

    result._position -= decrease;

    return result;
  }

//  Particles::iterator& Particles::iterator::operator*( void ) const
//  {
//    return *this;
//  }
//
//  Particles::iterator* Particles::iterator::operator->( void ) const
//  {
//    return this;
//  }

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
  , idConstIterator( other.idConstIterator )
  , lifeConstIterator( other.lifeConstIterator )
  , sizeConstIterator( other.sizeConstIterator )
  , positionConstIterator( other.positionConstIterator )
  , colorConstIterator( other.colorConstIterator )
  , velocityModuleConstIterator( other.velocityModuleConstIterator )
  , velocityConstIterator( other.velocityConstIterator )
  , accelerationConstIterator( other.accelerationConstIterator )
  , aliveConstIterator( other.aliveConstIterator )
  , newbornConstIterator( other.newbornConstIterator )
  { }

  Particles::const_iterator::const_iterator(
      const Particles::iterator& other )
  : _position( other._position )
  , _size( other._size )
  , _data( other._data )
  , idConstIterator( other.idIterator )
  , lifeConstIterator( other.lifeIterator )
  , sizeConstIterator( other.sizeIterator )
  , positionConstIterator( other.positionIterator )
  , colorConstIterator( other.colorIterator )
  , velocityModuleConstIterator( other.velocityModuleIterator )
  , velocityConstIterator( other.velocityIterator )
  , accelerationConstIterator( other.accelerationIterator )
  , aliveConstIterator( other.aliveIterator )
  , newbornConstIterator( other.newbornIterator )
  { }

  Particles::const_iterator& Particles::const_iterator::operator++( void )
  {

    idConstIterator++;
    lifeConstIterator++;
    sizeConstIterator++;
    positionConstIterator++;
    colorConstIterator++;
    velocityModuleConstIterator++;
    velocityConstIterator++;
    accelerationConstIterator++;
    aliveConstIterator++;
    newbornConstIterator++;

    _position++;

    return *this;
  }

  Particles::const_iterator Particles::const_iterator::operator++( int )
  {
    Particles::const_iterator result( *this );

    idConstIterator++;
    lifeConstIterator++;
    sizeConstIterator++;
    positionConstIterator++;
    colorConstIterator++;
    velocityModuleConstIterator++;
    velocityConstIterator++;
    accelerationConstIterator++;
    aliveConstIterator++;
    newbornConstIterator++;

    _position++;

    return result;
  }

  Particles::const_iterator& Particles::const_iterator::operator--( void )
  {

    idConstIterator--;
    lifeConstIterator--;
    sizeConstIterator--;
    positionConstIterator--;
    colorConstIterator--;
    velocityModuleConstIterator--;
    velocityConstIterator--;
    accelerationConstIterator--;
    aliveConstIterator--;
    newbornConstIterator--;

    _position--;

    return *this;
  }

  Particles::const_iterator Particles::const_iterator::operator--( int )
  {
    Particles::const_iterator result( *this );

    idConstIterator--;
    lifeConstIterator--;
    sizeConstIterator--;
    positionConstIterator--;
    colorConstIterator--;
    velocityModuleConstIterator--;
    velocityConstIterator--;
    accelerationConstIterator--;
    aliveConstIterator--;
    newbornConstIterator--;

    _position--;

    return result;
  }

  Particles::const_iterator
  Particles::const_iterator::operator+( int increase )
  {
    Particles::const_iterator result( *this );

    result.idConstIterator += increase;
    result.lifeConstIterator += increase;
    result.sizeConstIterator += increase;
    result.positionConstIterator += increase;
    result.colorConstIterator += increase;
    result.velocityModuleConstIterator += increase;
    result.velocityConstIterator += increase;
    result.accelerationConstIterator += increase;
    result.aliveConstIterator += increase;
    result.newbornConstIterator += increase;

    result._position += increase;

    return result;
  }

  Particles::const_iterator
  Particles::const_iterator::operator-( int decrease )
  {
    Particles::const_iterator result( *this );

    result.idConstIterator -= decrease;
    result.lifeConstIterator -= decrease;
    result.sizeConstIterator -= decrease;
    result.positionConstIterator -= decrease;
    result.colorConstIterator -= decrease;
    result.velocityModuleConstIterator -= decrease;
    result.velocityConstIterator -= decrease;
    result.accelerationConstIterator -= decrease;
    result.aliveConstIterator -= decrease;
    result.newbornConstIterator -= decrease;

    result._position -= decrease;

    return result;
  }

//  Particles::const_iterator& Particles::const_iterator::operator*( void )
//  {
//    return *this;
//  }
//
//  Particles::const_iterator* Particles::const_iterator::operator->( void )
//  {
//    return this;
//  }

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
    return !(*this == other);
  }

}


