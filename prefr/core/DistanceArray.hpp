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
#include <iostream>

#include "../utils/types.h"
#include "ICamera.h"

namespace prefr
{

  class DistanceUnit
  {
    friend class DistanceArray;

  public:

    DistanceUnit( void )
    {
      _id = nullptr;
      _distance = nullptr;
    }

    DistanceUnit( int* id_, float* distance_ )
      : _id( id_ )
      , _distance( distance_ )
    {
    }

    const int& id( void ) const { return *_id; }
    void id( int i ){ *_id = i; }

    const float& distance( void ) const { return *_distance; }
    void distance( float d ){ *_distance = d; }

  protected:

    int* _id;
    float* _distance;


  };

  typedef DistanceUnit TDistUnit;
  typedef std::vector< TDistUnit > TDistUnitContainer;

  class DistanceArray
  {
  public:

    DistanceArray( unsigned int size,
                   ICamera* camera = nullptr )
    : _camera( camera )
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
        ( *element )._id = &( *currentId );
        ( *element )._distance = &( *currentDist );
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

    inline DistanceUnit& at( unsigned int i )
    {
      return elements[ i ];
    }

    inline DistanceUnit& operator[]( unsigned int i )
    {
      return elements[ i ];
    }

    virtual inline const int& getID( unsigned int i ) const
    {
      return elements[ i ].id( );
    }

    virtual inline const float& getDistance( unsigned int i ) const
    {
      return elements[ i ].distance( );
    }

    inline void resetCounter( void )
    {
      current = 0;
      currentIt = elements.begin( );
    }

    inline DistanceUnit* next( void )
    {
      DistanceUnit* result = &( *currentIt );

      ++current;
      ++currentIt;

      return result;
    }

    inline static bool sortDescending ( const DistanceUnit& lhs,
                                        const DistanceUnit& rhs )
    {
      return *lhs._distance > *rhs._distance;
    }

    inline static bool sortAscending ( const DistanceUnit& lhs,
                                       const DistanceUnit& rhs)
    {
      return *lhs._distance < *rhs._distance;
    }

    std::vector< int > ids;
    std::vector< float > distances;

    TDistUnitContainer elements;
    TDistUnitContainer::iterator currentIt;

    int current;

    ICamera* _camera;
  };
}
#endif /* __PREFR__DISTANCEARRAY__ */
