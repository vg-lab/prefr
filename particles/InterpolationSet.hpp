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
    vector<float> invIntervals;
    float step;


    unsigned int size;

    InterpolationSet(void): step(1), size(0){}

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


    T GetFirstValue()
    {
      return values[0];
    }

    // Implementation exportable to kernel due to avoiding loops
    T GetValue(float time)
    {
      if (size > 1 && time > 0.0f)
      {
        assert(time >= 0 && time <= 1.0f);

        int ref = quickReference[floor(time * (quickReference.size()-1))];

        float relTime = glm::clamp((time - times[ref]) * invIntervals[ref], 0.f, 1.f);

        T res = ((1.0f - relTime) * values[ref] + relTime * values[ref+1]);

        return res;
      }
      else
      {
        return values[0];
      }

    }

    // Faster implementation for CPU interpolation.
    T GetValueQuick(float time)
    {
      assert(time >= 0 && time <= 1.0f);

      if (size > 1 && time > 0.0f)
      {
       unsigned int i = 0;

       while (time > times[i+1])
       {
         i++;
       }

       float relTime = (time - times[i]) * invIntervals[i];

       T res = ((1.0f - relTime) * values[i] + relTime * values[i+1]);

       return res;

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
      string str = std::to_string( (long double) (newTime));

      unsigned int pos = str.length()-1;
      while (pos > 0)
      {
        if (str[pos] != '0')
          break;
        pos--;
      }
      str = str.substr(0, pos+1);

      precision = str.rfind('.');
      precision = precision > 0 ? str.length() - precision - 1 : 0;
      precision = pow(10.f, precision);

      if (precision > quickReference.size())
        quickReference.resize(precision);

      step = 1.0f / quickReference.size();

      invIntervals.resize(times.size(), 1.0f);

      if (times.size() > 1)
        for (unsigned int i = 0; i < times.size()-1; i++)
        {
          invIntervals[i] = 1.0f / (times[i+1] - times[i]);
        }

      vector<int> limits(times.size());

      for (unsigned int i = 0; i < limits.size()-1; i++)
      {
        limits[i] = int(floor(times[i+1] * quickReference.size()));
      }

      limits[limits.size()-1] = quickReference.size();

      pos = 0;
      for (unsigned int i = 0; i < quickReference.size(); i++)
      {
        if (i >= limits[pos])
          pos++;
        quickReference[i] = pos;
      }
    }
  };




}

#endif /* INTERPOLATIONSET_H_ */
