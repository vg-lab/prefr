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
#ifndef __PREFR__DISTANCE_ARRAY__
#define __PREFR__DISTANCE_ARRAY__

#include <vector>

#include "types.h"

#include <iostream>

#define SERIALIZE_BEFORE_SORT 1

namespace prefr
{

  class DistanceUnit
  {

  public:
    int* id;
    float* distance;

    DistanceUnit( void )
    {
      id = nullptr;
      distance = nullptr;
    }

    DistanceUnit( int* id_, float* distance_ )
      : id( id_ )
      , distance( distance_ )
    {
    }

    inline const int& Id( void ) const { return *id; }
    inline void Id( int i ){ *id = i; }

    inline const float& Distance( void ) const { return *distance; }
    inline void Distance( float d ){ *distance = d; }

  };

  typedef DistanceUnit TDistUnit;
  typedef std::vector<TDistUnit> TDistUnitContainer;

  class DistanceArray
  {
  public:

    DistanceArray(unsigned int size)
    {
      ids.resize( size );
      distances.resize( size );
      elements.resize( size );
      current = 0;
      currentIt = elements.begin( );

      std::vector< int >::iterator currentId = ids.begin( );
      std::vector< float >::iterator currentDist = distances.begin( );

      for( TDistUnitContainer::iterator element = elements.begin( );
           element != elements.end( );
           ++element, ++currentId, ++currentDist )
      {
        ( *element ).id = &( *currentId );
        ( *element ).distance = &( *currentDist );

        std::cout << ( *element ).id << " -> "
                  << ( *element ).distance << std::endl;
      }

    }

    virtual ~DistanceArray( void )
    {
    }

    std::vector< DistanceUnit >::iterator begin( void )
    {
      return elements.begin( );
    }

    std::vector< DistanceUnit >::iterator end( void )
    {
      return elements.end( );
    }

    inline DistanceUnit& at(unsigned int i)
    {
      return elements[ i ];
    }

    inline DistanceUnit& operator[](unsigned int i)
    {
      return elements[ i ];
    }

    virtual inline const int& getID( unsigned int i ) const
    {
      return elements[ i ].Id( );
    }

    virtual inline const float& getDistance( unsigned int i ) const
    {
      return elements[i].Distance( );
    }

    inline void ResetCounter( void )
    {
      current = 0;
      currentIt = elements.begin( );
    }

    inline DistanceUnit* next( void )
    {
      DistanceUnit* result = &( *currentIt );

      current++;
      currentIt++;

      return result;
    }

    inline static bool sortDescending ( const DistanceUnit& lhs,
                                        const DistanceUnit& rhs )
    {
      return *lhs.distance > *rhs.distance;
    }

    inline static bool sortAscending ( const DistanceUnit& lhs,
                                       const DistanceUnit& rhs)
    {
      return *lhs.distance < *rhs.distance;
    }

    std::vector< int > ids;
    std::vector< float > distances;

    TDistUnitContainer elements;
    TDistUnitContainer::iterator currentIt;

    int current;

  };
}
#endif /* __PREFR__DISTANCEARRAY__ */
