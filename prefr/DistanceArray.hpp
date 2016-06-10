/*
 * DistanceArray.hpp
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
 */

#ifndef DISTANCEARRAY_HPP_
#define DISTANCEARRAY_HPP_

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

    DistanceUnit(int* id_, float* distance_)
      : id(id_)
      , distance(distance_)
    {
    }

    inline const int& Id( void ) const { return *id; }
    inline void Id( int i ){ *id = i; }

    inline const float& Distance( void ) const { return *distance; }
    inline void Distance( float d ){ *distance = d; }

//    inline const int& getID(void){return (const int&)*id;}
//    inline const float& getDistance(void){return (const float&)*distance;}
  };

  typedef DistanceUnit tdunit;
  typedef std::vector<tdunit> tdcontainter;

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

//      unsigned int i = 0;
      std::vector< int >::iterator currentId = ids.begin( );
      std::vector< float >::iterator currentDist = distances.begin( );

      for( tdcontainter::iterator element = elements.begin( );
           element != elements.end( );
           ++element, ++currentId, ++currentDist )
      {
        ( *element ).id = &( *currentId );
        ( *element ).distance = &( *currentDist );
      }

    }

    virtual ~DistanceArray()
    {
    }

    std::vector< DistanceUnit >::iterator begin()
    {
      return elements.begin( );
    }

    std::vector< DistanceUnit >::iterator end()
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

    inline void ResetCounter()
    {
      current = 0;
      currentIt = elements.begin( );
    }

    inline DistanceUnit* next()
    {
      DistanceUnit* result = &( *currentIt );

      current++;
      currentIt++;

      return result;
    }

    inline static bool sortDescending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance > *rhs.distance;}
    inline static bool sortAscending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance < *rhs.distance;}

    std::vector< int > ids;
    std::vector< float > distances;

    tdcontainter elements;
    tdcontainter::iterator currentIt;

    int current;
  };
}
#endif /* DISTANCEARRAY_HPP_ */
