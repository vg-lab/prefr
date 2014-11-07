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
#include <string>

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

    vector<int> quickReference;
    float step;

    unsigned int size;

    InterpolationSet(void): size(0){}

  public:

    void Insert(float time, T value)
    {
      assert(time >= 0 && time <= 1.0f);

      int i = 0;

      while (i < size-1 && size > 0)
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

      UpdateQuickReference(time);
    }



    T GetValue(float time)
    {
      if (size > 1 && time > 0.0f)
      {
        assert(time >= 0 && time <= 1.0f);

        unsigned int i = 0;

        while (time > times[i+1])
        {
//          std::cout << time << " > " << i+1 << " " << times[i+1] << std::endl;
          i++;
        }

        float relTime = (time - times[i]) / (times[i+1] - times[i]);

//        std::cout << time << " -> times[" << i << "] = " << times[i]
//                          << " -> times[" << i+1 << "] = " << times[i+1]
//                          << " = " << relTime << std::endl;
        T res = ((1.0f - relTime) * values[i] + relTime * values[i+1]);

        return res;

//        while (i < size-1)
//        {
//          if (time >= times[i] && time < times[i+1])
//          {
//            return ((1.0f - time) * values[i] + time * values[i+1]);
//          }
//          i++;
//        }
//
//        return ((1.0f - time) * values[size-2] + time * values[size-1]);

      }
      else
      {
        return values[0];
      }

    }

  private:

    void UpdateQuickReference(float newTime)
    {
      int precision = 0;
      std::string cad;// = std::to_string(newTime);
//      precision = str.find_last_of('.');
//      precision = precision > 0 ? str.length() - precision : 0;
//      precision = pow(10.f, precision);

      if (precision > quickReference.size())
        quickReference.resize(precision);

      step = 1.0f / quickReference.size();


      vector<int> limits(times.size());

      for (unsigned int i = 0; i < limits.size()-1; i++)
      {
        limits[i] = int(floor(times[i+1] * step * precision));
      }

      limits[limits.size()-1] = quickReference.size();

      unsigned int pos = 0;
      for (unsigned int i = 0; i < quickReference.size(); i++)
      {
        if (i >= limits[pos])
          pos++;
        quickReference[i] = pos;

        std::cout << quickReference[i] << " ";
      }
      std::cout << std::endl;
    }
  };




}

#endif /* INTERPOLATIONSET_H_ */
