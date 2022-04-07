/*
 * Copyright (c) 2014-2020 GMRV/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/gmrvvis/prefr>
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
      _distancesSquared = nullptr;
    }

    DistanceUnit( int* id_, float* distancesSquared_ )
      : _id( id_ )
      , _distancesSquared( distancesSquared_ )
    {
    }

    const int& id( void ) const { return *_id; }
    void id( int i ){ *_id = i; }

    const float& distanceSquared( void ) const { return *_distancesSquared; }
    void distanceSquared( float d ){ *_distancesSquared = d; }

  protected:

    int* _id;
    float* _distancesSquared;


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
        ( *element )._distancesSquared = &( *currentDist );
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

    virtual inline const float& getDistanceSquared( unsigned int i ) const
    {
      return elements[ i ].distanceSquared( );
    }

    inline void resetCounter( void )
    {
      current = 0;
      currentIt = elements.begin( );

      //TODO find something more efficient
      for( auto& dist : distances )
        dist = -1;
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
      return *lhs._distancesSquared > *rhs._distancesSquared;
    }

    inline static bool sortAscending ( const DistanceUnit& lhs,
                                       const DistanceUnit& rhs)
    {
      return *lhs._distancesSquared < *rhs._distancesSquared;
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
