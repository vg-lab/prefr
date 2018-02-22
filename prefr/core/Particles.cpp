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

#include <iostream>

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
    _accelerationModuleVector.resize( newSize );
    _accelerationVector.resize( newSize );
    _aliveVector.resize( newSize, false );

    initVectorReferences( );

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
    _accelerationModuleVector.clear( );
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
    return _createIterator( 0 );
  }

  Particles::iterator Particles::end( void )
  {
    return _createIterator( _size );
  }

  Particles::const_iterator Particles::end( void ) const
  {
    return _createIterator( _size );
  }

  ParticleRange Particles::range( void )
  {
    ParticleRange result ( *this, begin( ), end( ));

    return result;
  }

  Particles::iterator Particles::at( unsigned int i )
  {
    return _createIterator( i );
  }

  Particles::const_iterator Particles::at( unsigned int i ) const
  {
    return _createIterator( i );
  }


  Particles::iterator
  Particles::_createIterator( unsigned int i ) const
  {
    assert( i <= _size );

    iterator it;
    it._data = this;
    it._vectorRef = _vectorReferences;
    it._size = 0;

    it._indexPosition = 0;
    it._particleIndices = nullptr;

    it.set( i );

    return it;
  }

  const TParticle& Particles::vectorReferences( void ) const
  {
    return _vectorReferences;
  }

  void Particles::initVectorReferences( void )
  {
    _vectorReferences = std::make_tuple( _idVector.data( ),
                                         _lifeVector.data( ),
                                         _sizeVector.data( ),
                                         _positionVector.data( ),
                                         _colorVector.data( ),
                                         _velocityModuleVector.data( ),
                                         _velocityVector.data( ),
                                         _accelerationModuleVector.data( ),
                                         _accelerationVector.data( ),
                                         _aliveVector.data( ));

  }

  // BASE_ITERATOR

  Particles::base_const_iterator::base_const_iterator( void )
  : _position( 0  )
  , _size( 0 )
  , _data( nullptr )
  , _vectorRef( )
  , _indexPosition( 0 )
  , _particleIndices( nullptr )
  , _id_ptr( nullptr )
  , _life_ptr( nullptr )
  , _size_ptr( nullptr )
  , _position_ptr( nullptr )
  , _color_ptr( nullptr )
  , _velocityModule_ptr( nullptr )
  , _velocity_ptr( nullptr )
  , _accelerationModule_ptr( nullptr )
  , _acceleration_ptr( nullptr )
  , _alive_ptr( nullptr )
  { }

  Particles::base_const_iterator::base_const_iterator( const Particles::base_const_iterator& other )
  : _position( other._position )
  , _size( other._size )
  , _data( other._data )
  , _vectorRef( other._vectorRef )
  , _indexPosition( other._indexPosition )
  , _particleIndices( other._particleIndices )
  , _id_ptr( other._id_ptr )
  , _life_ptr( other._life_ptr )
  , _size_ptr( other._size_ptr )
  , _position_ptr( other._position_ptr )
  , _color_ptr( other._color_ptr )
  , _velocityModule_ptr( other._velocityModule_ptr )
  , _velocity_ptr( other._velocity_ptr )
  , _accelerationModule_ptr( other._accelerationModule_ptr )
  , _acceleration_ptr( other._acceleration_ptr )
  , _alive_ptr( other._alive_ptr )
  { }

  Particles::base_const_iterator::~base_const_iterator( void )
  { }

  void Particles::base_const_iterator::set( unsigned int index_ )
  {
    _id_ptr =
        std::get< ( unsigned int ) prefr::ID >( _vectorRef ) + index_;
    _life_ptr =
        std::get< ( unsigned int ) prefr::LIFE >( _vectorRef ) + index_;
    _size_ptr =
        std::get< ( unsigned int ) prefr::SIZE >( _vectorRef ) + index_;
    _position_ptr =
        std::get< ( unsigned int ) prefr::POSITION >( _vectorRef ) + index_;
    _color_ptr =
        std::get< ( unsigned int ) prefr::COLOR >( _vectorRef ) + index_;
    _velocityModule_ptr =
        std::get< ( unsigned int ) prefr::VELOCITY_MODULE >( _vectorRef ) + index_;
    _velocity_ptr =
        std::get< ( unsigned int ) prefr::VELOCITY >( _vectorRef ) + index_;
    _accelerationModule_ptr =
        std::get< ( unsigned int ) prefr::ACCELERATION_MODULE >( _vectorRef ) + index_;
    _acceleration_ptr =
        std::get< ( unsigned int ) prefr::ACCELERATION >( _vectorRef ) + index_;
    _alive_ptr =
        std::get< ( unsigned int ) prefr::ALIVE >( _vectorRef ) + index_;

    _position = index_;
  }

  void Particles::base_const_iterator::increase( int inc )
  {
    _indexPosition += inc;

    if( _particleIndices && _indexPosition < _size )
    {

      unsigned int index = (*_particleIndices)[ _indexPosition ];

      set( index );
    }
    else
    {
      _id_ptr += inc;
      _life_ptr += inc;
      _size_ptr += inc;
      _position_ptr += inc;
      _color_ptr += inc;
      _velocityModule_ptr += inc;
      _velocity_ptr += inc;
      _accelerationModule_ptr += inc;
      _acceleration_ptr += inc;
      _alive_ptr += inc;

      _position += inc;
    }

//    std::cout << "it " << _position << " " << _indexPosition << " " << _size << std::endl;
  }

  void Particles::base_const_iterator::decrease( int dec )
  {
    _indexPosition -= dec;

    if( _particleIndices && _indexPosition < _particleIndices->size( ))
    {

      unsigned int index = (*_particleIndices)[ _indexPosition ];

      set( index );
    }
    else
    {
      _id_ptr -= dec;
      _life_ptr -= dec;
      _size_ptr -= dec;
      _position_ptr -= dec;
      _color_ptr -= dec;
      _velocityModule_ptr -= dec;
      _velocity_ptr -= dec;
      _accelerationModule_ptr -= dec;
      _acceleration_ptr -= dec;
      _alive_ptr -= dec;

      _position -= dec;
    }


  }

  int Particles::base_const_iterator::sum( const base_const_iterator& other ) const
  {
    if( _particleIndices )
          return _indexPosition + other._indexPosition;
        else
          return _position + other._position;
  }

  int Particles::base_const_iterator::difference( const base_const_iterator& other ) const
  {
    if( _particleIndices )
      return ( int ) _indexPosition - ( int ) other._indexPosition;
    else
      return ( int ) _position - ( int ) other._position;
  }

  bool Particles::base_const_iterator::compare( const base_const_iterator& other ) const
  {
    return ( other._data == this->_data &&
             other._position == this->_position );
  }

  TParticle Particles::base_const_iterator::currentValues( void )
  {
    return std::make_tuple( _id_ptr,
                            _life_ptr,
                            _size_ptr,
                            _position_ptr,
                            _color_ptr,
                            _velocityModule_ptr,
                            _velocity_ptr,
                            _accelerationModule_ptr,
                            _acceleration_ptr,
                            _alive_ptr);
  }


  Particles::base_iterator::base_iterator( void )
  : base_const_iterator( )
  { }


  Particles::base_iterator::base_iterator( const base_const_iterator& other )
  : base_const_iterator( other )
  { }


  Particles::base_iterator::~base_iterator( void )
  { }


  // ITERATOR


  Particles::iterator Particles::iterator::operator*( void )
  {
    return (*this);
  }

  Particles::iterator Particles::iterator::operator->( void )
  {
    return (*this);
  }


  ParticleCollection::ParticleCollection( void )
  : _vectorReferences( )
  , _size( 0 )
  , _data( nullptr )
  { }

  ParticleCollection::ParticleCollection( const ParticleCollection& other )
  : _particleIndices( other._particleIndices )
  , _vectorReferences( other._vectorReferences )
  , _size( other._particleIndices.size( ))
  , _data( other._data )
  { }

  ParticleCollection::ParticleCollection( const Particles& data_ )
  : _vectorReferences( data_.vectorReferences( ))
  , _data( & data_ )
  {
    _particleIndices.clear( );
    _size = _data->numParticles( );
  }

  ParticleCollection::ParticleCollection( const Particles& data_,
                                          unsigned int begin_,
                                          unsigned int end_ )
  : _vectorReferences( data_.vectorReferences( ))
  , _data( & data_ )
  {
    if( end_ < begin_ )
      std::swap( begin_, end_ );

    _particleIndices.reserve( end_ - begin_ );
    for( unsigned int i = begin_; i < end_; i++ )
    {
      _particleIndices.push_back( i );
    }

    _size = _particleIndices.size( );
  }

  ParticleCollection::ParticleCollection( const Particles& data_,
                                          Particles::iterator begin_,
                                          Particles::iterator end_ )
  : _vectorReferences( data_.vectorReferences( ))
  , _data( &data_ )
  {
    auto beginIt = _data->begin( );
    unsigned int begin = begin_ - beginIt;
    unsigned int end = end_ - beginIt;

    _particleIndices.reserve( begin - end );

    for( unsigned int i = begin; i < end; i++ )
    {
      _particleIndices.push_back( i );
    }

    _size = _particleIndices.size( );
  }

  ParticleCollection::ParticleCollection( const Particles& data_,
                                          const ParticleIndices& indices_ )
  : _particleIndices( indices_ )
  , _vectorReferences( data_.vectorReferences( ))
  , _size( _particleIndices.size( ))
  , _data( & data_ )
  { }

  const ParticleIndices& ParticleCollection::indices( void ) const
  {
    return _particleIndices;
  }

  void ParticleCollection::indices( const ParticleIndices& newIndices )
  {
    _particleIndices = newIndices;
    _size = _particleIndices.size( );
  }

  unsigned int ParticleCollection::size( void )
  {
    return _size;
  }

  Particles::iterator ParticleCollection::begin( void )
  {
    return _createIterator( );
  }

  Particles::const_iterator ParticleCollection::begin( void ) const
  {
    return _createIterator( );
  }

  Particles::iterator ParticleCollection::end( void )
  {
    return _createIterator( _size );
  }

  Particles::const_iterator ParticleCollection::end( void ) const
  {
    return _createIterator( _size );
  }

  Particles::iterator ParticleCollection::at( unsigned int index_ )
  {
    return _createIterator( index_ );
  }

  Particles::iterator
  ParticleCollection::_createIterator( unsigned int index_ ) const
  {
    assert( _data );
    assert( index_ < _data->numParticles( ));

    Particles::iterator result;

    result._data = _data;
    result._vectorRef = _vectorReferences;

    if( index_ < _particleIndices.size( ))
    {
      result._particleIndices = &_particleIndices;
      result._size = _particleIndices.size( );

      result._indexPosition = index_;

      unsigned int index = _particleIndices[ index_ ];

      result.set( index );

    }
    else if( _particleIndices.size( ) && index_ == _particleIndices.size( ) )
    {
      unsigned int index = _particleIndices.back( ) + 1;
      result._particleIndices = nullptr;
      result._size = 0;
      result._position = index;
    }
    else
    {
      result._particleIndices = nullptr;
      result._size = 0;
      result.set( index_ );
    }

    return result;
  }

  void ParticleCollection::addIndex( unsigned int idx )
  {
    std::set< unsigned int > indexSet( _particleIndices.begin( ),
                                       _particleIndices.end( ));

    indexSet.insert( idx );

    _particleIndices.clear( );
    _particleIndices.insert( _particleIndices.end( ), indexSet.begin( ), indexSet.end( ));

    _size = _particleIndices.size( );

    std::cout << "Indices: " << _particleIndices.size( ) << std::endl;
  }

  void ParticleCollection::addIndices( ParticleIndices idxVector )
  {
    std::set< unsigned int > indexSet( _particleIndices.begin( ),
                                       _particleIndices.end( ));

    for( auto idx : idxVector )
      indexSet.insert( idx );

    _particleIndices.clear( );
    _particleIndices.insert( _particleIndices.end( ), indexSet.begin( ), indexSet.end( ));

    _size = _particleIndices.size( );
  }

  void ParticleCollection::removeIndex( unsigned int idx )
  {
    std::set< unsigned int > indexSet( _particleIndices.begin( ), _particleIndices.end( ));

    indexSet.erase( idx );

    _particleIndices.clear( );
    _particleIndices.insert( _particleIndices.end( ), indexSet.begin( ), indexSet.end( ));

    _size = _particleIndices.size( );
  }

  void ParticleCollection::removeIndices( ParticleIndices idxVector )
  {
    std::set< unsigned int > indexSet( _particleIndices.begin( ),
                                       _particleIndices.end( ));

    for( auto idx : idxVector )
      indexSet.erase( idx );

    _particleIndices.clear( );
    _particleIndices.insert( _particleIndices.end( ), indexSet.begin( ), indexSet.end( ));

    _size = _particleIndices.size( );
  }

  void ParticleCollection::transferIndexTo( ParticleCollection& other,
                                            unsigned int idx )
  {
    removeIndex( idx );
    other.addIndex( idx );
  }

  void ParticleCollection::transferIndicesTo( ParticleCollection& other,
                                              ParticleIndices idxVector  )
  {
    removeIndices( idxVector );
    other.addIndices( idxVector );
  }



  // PARTICLECOLLECTION::ITERATOR

  Particles::iterator::iterator( void )
  : base_iterator( )
  { }

  Particles::iterator::iterator( const Particles::iterator& other )
  : base_iterator( other )
  { }

  Particles::iterator::iterator( const Particles::const_iterator& other )
  : base_iterator( other )
  { }


  Particles::iterator& Particles::iterator::operator++( void )
  {

    increase( 1 );

    return *this;
  }

  Particles::iterator Particles::iterator::operator++( int )
  {

    Particles::iterator result( *this );

    increase( 1 );

    return result;

  }

  Particles::iterator& Particles::iterator::operator--( void )
  {
    decrease( 1 );

    return *this;
  }

  Particles::iterator Particles::iterator::operator--( int )
  {
    Particles::iterator result( *this );

    decrease( 1 );

    return result;
  }

  int Particles::iterator::operator+( const Particles::iterator& other ) const
  {
    return sum( other );
  }

  int Particles::iterator::operator-( const Particles::iterator& other ) const
  {
    return difference( other );
  }

  Particles::iterator Particles::iterator::operator+( int increment ) const
  {

    Particles::iterator result( *this );

    result.increase( increment );

    return result;

  }

  Particles::iterator Particles::iterator::operator-( int decrement ) const
  {
    Particles::iterator result( *this );

    result.decrease( decrement );

    return result;

  }

  bool Particles::iterator::operator==( const Particles::iterator& other ) const
  {
    return compare( other );
  }

  bool Particles::iterator::operator!=( const Particles::iterator& other ) const
  {
    return !(*this == other);
  }


  // PARTICLECOLLECTION::CONST_ITERATOR

  Particles::const_iterator::const_iterator( void )
  : base_const_iterator( )
  { }

  Particles::const_iterator::const_iterator( const Particles::iterator& other )
  : base_const_iterator( other )
  { }

  Particles::const_iterator::const_iterator( const Particles::const_iterator& other )
  : base_const_iterator( other )
  { }


  Particles::const_iterator& Particles::const_iterator::operator++( void )
  {
    increase( 1 );

    return *this;
  }

  Particles::const_iterator Particles::const_iterator::operator++( int )
  {

    Particles::const_iterator result( *this );

    increase( 1 );

    return result;

  }

  Particles::const_iterator& Particles::const_iterator::operator--( void )
  {
    decrease( 1 );

    return *this;
  }

  Particles::const_iterator Particles::const_iterator::operator--( int )
  {
    Particles::const_iterator result( *this );

    decrease( 1 );

    return result;
  }

  int Particles::const_iterator::operator+( const Particles::const_iterator& other ) const
  {
    return sum( other );
  }

  int Particles::const_iterator::operator-( const Particles::const_iterator& other ) const
  {
    return difference( other );
  }


  Particles::const_iterator
  Particles::const_iterator::operator+( int increase ) const
  {
    iterator result( *this );

    result.increase( increase );

    return result;
  }

  Particles::const_iterator
  Particles::const_iterator::operator-( int decrease ) const
  {
    iterator result( *this );

    result.decrease( decrease );

    return result;
  }

  bool Particles::const_iterator::operator==( const Particles::const_iterator& other ) const
  {
    return compare( other );
  }

  bool Particles::const_iterator::operator!=( const Particles::const_iterator& other ) const
  {
    return !(*this == other);
  }

  Particles::const_iterator Particles::const_iterator::operator*( void )
  {
    return (*this);
  }

  Particles::const_iterator  Particles::const_iterator::operator->( void )
  {
    return (*this);
  }



}


