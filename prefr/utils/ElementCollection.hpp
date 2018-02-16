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
#ifndef __PREFR__ELEMENT_COLLECTION__
#define __PREFR__ELEMENT_COLLECTION__

#include <vector>

namespace utils
{

  template< class ELEM, typename arrayIndexType = unsigned int >
  class ElementCollection
  {

  public:

    class iterator;
    class const_iterator;

    friend class ElementCollection::iterator;
    friend class ElementCollection::const_iterator;

    ElementCollection( void )
    : _begin( end( ))
    , _end( end( ))
    , _size( 0 )
    {

    }

    ElementCollection( ELEM& elementsArray,
                       const std::vector< arrayIndexType >& indices )
    {
      assert( elementsArray.size( ) <= indices.size( ));

      //TODO
      _elementIndices = indices;
      _size = _elementIndices.size( );

      _data = elementsArray._elements;

      _begin = createIterator( );
      _end= _begin + _size;

    }

    size_t size( )
    {
      return _elementIndices.size( );
    }

    iterator begin( void )
    {
      return this->_begin;
    }

    iterator end( void )
    {
      return this->_end;
    }

    const_iterator begin( void ) const
    {
      return this->_begin;
    }

    const_iterator end( void ) const
    {
      return this->_end;
    }

    iterator GetElement( unsigned int i )
    {
      iterator result( _begin );

      return result + i;
    }


    class iterator
    : public std::iterator< std::bidirectional_iterator_tag,
                            typename ELEM::iterator >
    {
      friend class ElementCollection;

    public:

      iterator( void );
      iterator( const iterator& other );
      virtual ~iterator( ){ }

      typename ELEM::iterator& operator*( void )
      {
        return _data->GetElement( _position ).value( );
      }

      virtual iterator& operator++( void );
      virtual iterator operator++( int );
      virtual iterator& operator--( void );
      virtual iterator operator--( int );

      virtual iterator operator+( int increase );
      virtual iterator operator-( int decrease );

      virtual int operator+( const iterator& other );
      virtual int operator-( const iterator& other );

      virtual bool operator== ( const iterator& other );
      virtual bool operator!= ( const iterator& other );

      virtual arrayIndexType index( void )
      {
        assert( _position < _data->_elementIndices.size( ));
        return _data->_elementIndices[ _position ];
      }

      virtual typename ELEM::iterator value( void )
      {
        assert( _position < _data->size( ) && _position < _elementIndices.size( ));
        return _data->GetElement( _elementIndices[ _position ]);
      }

    protected:

      ElementCollection* _data;
      unsigned long _position;
      unsigned long _size;

    };

    class const_iterator : public iterator
    {
      friend class ElementCollection;

    public:

      const_iterator( const iterator& other );

      const_iterator& operator++( void );
      iterator operator++( int );
      iterator& operator--( void );
      iterator operator--( int );

      iterator operator+( int increase );
      iterator operator-( int decrease );

    };

  protected:

    iterator createIterator( unsigned int index = 0 )
    {
      iterator result;
      result._data = _data;

      result._position = index;
      result._size = _size;

      return result;
    }

    std::vector< arrayIndexType > _elementIndices;

    ELEM* _data;
    iterator _begin;
    iterator _end;

    unsigned int _size;


  };

}

#endif
