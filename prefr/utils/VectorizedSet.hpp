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

#ifndef __UTILS_VECTORIZEDSET__
#define __UTILS_VECTORIZEDSET__

#include <list>
#include <unordered_map>

#include <vector>
#include <set>

namespace prefr
{

  template< typename T >
  class VectorizedSet
  {

    typedef typename std::list< T >::const_iterator const_iterator;

  public:

    VectorizedSet( void )
    { }

    VectorizedSet( const VectorizedSet& other )
    : _elementsList( other._elementsList )
    {
      for( const_iterator it = _elementsList.begin( ); it != _elementsList.end( ); ++it )
      {
        _elementsRef.insert( std::make_pair( *it, it ));
      }
    }

    VectorizedSet( std::vector< T > elements )
    {
      _elementsList.insert( _elementsList.end( ),
                            elements.begin( ), elements.end( ));

      for( auto it = _elementsList.begin( ); it != _elementsList.end( ); ++it )
      {
        _elementsRef.insert( std::make_pair( *it, it ));
      }
    }

    VectorizedSet( std::set< T > elements )
    : _elementsList( elements )
    {
      for( auto element : _elementsList )
      {
        _elementsRef.insert( std::make_pair( *element, element ));
      }
    }

    VectorizedSet( const_iterator begin_, const_iterator end_ )
    : _elementsList( begin_, end_ )
    {
      for( auto element : _elementsList )
      {
        _elementsRef.insert( std::make_pair( *element, element ));
      }
    }

    VectorizedSet( std::initializer_list< T > initializer )
    : _elementsList( initializer )
    {
      for( auto it = _elementsList.begin( ); it != _elementsList.end( ); ++it )
      {
        _elementsRef.insert( std::make_pair( *it, it ));
      }
    }

    size_t size( void ) const
    {
      return _elementsList.size( );
    }

    void clear( void )
    {
      _elementsList.clear( );
      _elementsRef.clear( );
    }

    const std::set< T >& set( void ) const
    {
      std::set< T > result( _elementsList.begin( ), _elementsList.end( ));

      return result;
    }

    std::vector< T > vector( void ) const
    {
      std::vector< T > result( _elementsList.begin( ), _elementsList.end( ));

      return result;
    }

    const_iterator find( const T& value ) const
    {
      auto it = _elementsRef.find( value );
      if( it != _elementsRef )
        return it->second;

      return end( );
    }

    bool hasElement( const T& value ) const
    {
      return _elementsRef.find( value ) != _elementsRef.end( );
    }

    const_iterator begin( void ) const
    {
      return _elementsList.begin( );
    }

    const_iterator end( void ) const
    {
      return _elementsList.end( );
    }

    bool push_front( const T& element )
    {
      auto it = _elementsRef.find( element );
      if( it == _elementsRef.end( ))
      {
        _elementsList.push_front( element );
        _elementsRef.insert( std::make_pair( element, _elementsList.begin( )));
        return true;
      }

      return false;
    }

    bool push_back( const T& element )
    {
      auto it = _elementsRef.find( element );
      if( it == _elementsRef.end( ))
      {
        _elementsList.push_back( element );
        _elementsRef.insert( std::make_pair( element, --_elementsList.end( )));
        return true;
      }

      return false;
    }

    bool append( const T& element){ return push_back( element ); }
    bool prepend( const T& element){ return push_front( element ); }

    void insert( const VectorizedSet& other )
    {
      for( auto element : other )
        append( element );
    }

    void insert( const std::vector< T >& others )
    {
      for( auto element : others )
        append( element );
    }

    void insert( const std::set< T >& others )
    {
      for( auto element : others )
        append( element );
    }

    void remove( const VectorizedSet& other )
    {
      for( auto element : other )
        remove( element );
    }

    bool remove( const T& element )
    {
      auto it = _elementsRef.find( element );
      if( it != _elementsRef.end( ))
      {
        _elementsList.erase( it->second );
        _elementsRef.erase( it );

        return true;
      }

      return false;
    }

    void remove( const std::vector< T >& others )
    {
      for( auto element : others )
        remove( element );
    }

    void remove( const std::set< T >& others )
    {
      for( auto element : others )
        remove( element );
    }


    // TODO Add union, intersection

  protected:

    std::list< T > _elementsList;
    std::unordered_map< T, const_iterator > _elementsRef;

  };

}

#endif /* __UTILS_VECTORIZEDSET__ */
