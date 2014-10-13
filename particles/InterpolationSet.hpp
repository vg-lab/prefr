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
  private:

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
      }

      times.push_back(time);
      values.push_back(value);
      size = times.size();
    }

    T& GetValue(float time)
    {
      if (size > 1)
      {
        assert(time >= 0 && time <= 1.0f);

        unsigned int i = 0;

        while (i < size-1)
        {
          if (time >= times[i] && time < times[i+1])
          {
            return (time * values[i] + (1.0f - time) * values[i+1].value);
          }
        }

        return (time * values[size-2] + (1.0f - time) * values[size-1].value);

      }
      else
      {
        return values[0];
      }

    }

  };


}

#endif /* INTERPOLATIONSET_H_ */
