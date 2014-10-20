/*
 * InterpolationSet.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef INTERPOLATIONSET_H_
#define INTERPOLATIONSET_H_

#include <assert.h>
#include <vector>

#include <algorithm>

using namespace std;

namespace utils
{
  template <class T>
  class InterpolationSet
  {
  public:

    vector<float> times;
    vector<T> values;

    int size;

  public:

    void Insert(float time, T value)
    {
      assert(time >= 0 && time <= 1.0f);

      int i = 0;

      while (i < size)
      {
        if (time < times[i])
        {
          times.emplace(times.begin() + std::max(i-1,0), time);
          values.emplace(values.begin() + std::max(i-1,0), value);
          size = times.size();
          return;
        }
        i++;
      }

      times.push_back(time);
      values.push_back(value);
      size = times.size();

    }

    T GetValue(float time)
    {
      if (size > 1 && time > 0.0f)
      {
        assert(time >= 0 && time <= 1.0f);

        unsigned int i = 0;

        while (time > times[i+1] && i < times.size()-1)
        {
          i++;
        }

//        time = (time - times[i]) / (times[i+1] - times[i]);

        return (time * values[i] + (1.0f - time) * values[i+1]);
//
//        while (i < size-1)
//        {
//          if (time >= times[i] && time < times[i+1])
//          {
//            return (time * values[i] + (1.0f - time) * values[i+1]);
//          }
//          i++;
//        }
//
//        return (time * values[size-2] + (1.0f - time) * values[size-1]);

      }
      else
      {
        return values[0];
      }

    }

  };


}

#endif /* INTERPOLATIONSET_H_ */
