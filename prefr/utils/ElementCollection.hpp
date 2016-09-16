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

  template< class ELEM >
  class ElementCollection
  {
  public:

    ELEM* elements;
    typename ELEM::iterator _begin;
    typename ELEM::iterator _end;

    unsigned int size;

    ElementCollection( )
    {
      this->elements = nullptr;
      size = 0;
    }

    ElementCollection( ELEM& elementsArray )
    {
      this->elements = &elementsArray;
      this->_begin = elementsArray.begin( );
      this->_end = elementsArray.end( );
      this->size = end( ) - begin( );
    }


    ElementCollection( ELEM& elementsArray,
                       unsigned int start_,
                       unsigned int end_ )
    {
      this->elements = &elementsArray;
      this->_begin = elementsArray.begin( ) + start_;
      this->_end = elementsArray.begin( ) + end_;
      this->size = end_ - start_;
    }

    ElementCollection( ELEM* elementsArray,
                       unsigned int start_,
                       unsigned int end_ )
    {
      this->elements = elementsArray;
      this->_begin = elementsArray->begin( ) + start_;
      this->_end = elementsArray->begin( ) + end_;
      this->size = end_ - start_;
    }

    ElementCollection( ELEM* elementsArray,
                       typename ELEM::iterator start_,
                       typename ELEM::iterator end_ )
    {
      this->elements = elementsArray;
      this->_begin = start_;
      this->_end = end_ ;
      this->size = end_  - start_;
    }

    ElementCollection( const ElementCollection& other )
    {
      this->elements = other.elements;
      this->_begin = other._begin;
      this->_end = other._end;
      this->size = this->_end - this->_begin;
    }

    ElementCollection( const ElementCollection* other )
    {
      this->elements = other->elements;
      this->_begin = other->_begin;
      this->_end = other->_end;
      this->size = this->_end - this->_begin;
    }

    ElementCollection( ElementCollection* other,
                       unsigned int start_,
                       unsigned int end_ )
    {
      this->elements = other->elements;
      this->_begin = other->elements->begin( ) + start_;
      this->_end = other->elements->begin( ) + end_;
      this->size = end_ - start_;
    }

    ElementCollection( ElementCollection* other,
                       typename ELEM::iterator start_,
                       typename ELEM::iterator end_ )
    {
      this->elements = other->elements;
      this->_begin = start_;
      this->_end = end_;
      this->size = end_ - start_;
    }

    typename ELEM::iterator begin( void )
    {
      return this->_begin;
    }

    typename ELEM::iterator end( void )
    {
      return this->_end;
    }

    typename ELEM::const_iterator begin( void ) const
    {
      return this->_begin;
    }

    typename ELEM::const_iterator end( void ) const
    {
      return this->_end;
    }

    typename ELEM::iterator GetElement( unsigned int i )
    {
      typename ELEM::iterator result( _begin );

      return result + i;
    }
  };

}

#endif
