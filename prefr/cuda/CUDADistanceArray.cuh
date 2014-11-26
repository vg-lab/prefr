/*
 * CUDADistanceArray.h
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
 */

#ifndef CUDADISTANCEARRAY_H_
#define CUDADISTANCEARRAY_H_

#include "../DistanceArray.hpp"

#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

namespace particles
{

  class CUDADistanceArray : public DistanceArray
  {
  public:
    thrust::device_vector<int> deviceID;
    thrust::device_vector<float> deviceDistances;

    CUDADistanceArray ( unsigned int size)
    : DistanceArray( size )
    {}

    virtual int& getID(unsigned int i)
    {
      return ids->at(i);
    }

    virtual float& getDistance(unsigned int i)
    {
      return distances->at(i);
    }

  };

}

#endif /* CUDADISTANCEARRAY_H_ */
