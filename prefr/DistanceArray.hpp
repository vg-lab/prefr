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

#define SERIALIZE_BEFORE_SORT 1

namespace prefr
{

  class SortUnit
  {
  public:
    int idx;
    float distance;

    inline static bool sortDescending (const SortUnit& lhs,
                                const SortUnit& rhs)
    {
      return lhs.distance > rhs.distance;
    }

    inline static bool sortAscending (const SortUnit& lhs,
                               const SortUnit& rhs)
    {
      return lhs.distance < rhs.distance;
    }
  };

  class DistanceUnit
  {
  public:
    int* id;
    float* distance;

    DistanceUnit(void):id(nullptr), distance(nullptr){}

    DistanceUnit(int* id_, float* distance_)
      : id(id_)
      , distance(distance_)
    {
    }

    inline int& Id(void){return (int&)*id;}

    inline float& Distance(void){return (float&)*distance;}

    inline const int& getID(void){return (const int&)*id;}
    inline const float& getDistance(void){return (const float&)*distance;}
  };

  typedef DistanceUnit tdunit;
  typedef std::vector<tdunit> tdcontainter;

  class DistanceArray
  {
  public:

    DistanceArray(unsigned int size)
    {
      ids = new std::vector<int>(size);
      distances = new std::vector<float>(size);
      elements = new tdcontainter(size);
      current = 0;

      for (unsigned int i = 0; i < size; i++)
      {
        (*elements)[i] = tdunit(&(*ids)[i], &(*distances)[i]);
        (*ids)[i] = i;
      }

#ifdef SERIALIZE_BEFORE_SORT
      positions = new std::vector< glm::vec3 >( size );
      sizes = new std::vector< float >( size );
      colors = new std::vector< glm::vec4 >( size );
#endif


    }

    virtual ~DistanceArray()
    {
      delete( elements );
      delete( ids );
      delete( distances );
    }

    std::vector<DistanceUnit>::iterator begin()
    {
      return elements->begin();
    }

    std::vector<DistanceUnit>::iterator end()
    {
      return elements->end();
    }

    inline DistanceUnit& at(unsigned int i)
    {
      return (*elements)[i];
    }

    inline DistanceUnit& operator[](unsigned int i)
    {
      return (*elements)[i];
    }

    virtual inline int& getID(unsigned int i)
    {
      return (*elements)[i].Id();
    }

    virtual inline float& getDistance(unsigned int i)
    {
      return (*elements)[i].Distance();
    }

    inline void ResetCounter(){current=-1;}

    inline DistanceUnit& next()
    {
      current++;
//      if (current >= int(elements->size()))
//        current = elements->size() - 1;
      return (*elements)[current];
    }

    inline static bool sortDescending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance > *rhs.distance;}
    inline static bool sortAscending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance < *rhs.distance;}

    std::vector< int >* ids;
    std::vector< float >* distances;

    tdcontainter* elements;
    int current;

#ifdef SERIALIZE_BEFORE_SORT
    std::vector< glm::vec3 >* positions;
    std::vector< float >* sizes;
    std::vector< glm::vec4 >* colors;
  };
#endif

}
#endif /* DISTANCEARRAY_HPP_ */
