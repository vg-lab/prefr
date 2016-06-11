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
#ifndef __PREFR__INTERPOLATION_SET__
#define __PREFR__INTERPOLATION_SET__


#include "types.h"

#include <assert.h>
#include <vector>
#include <string>

#include <algorithm>

#include <iostream>

namespace utils
{
  template <class T>
  class InterpolationSet
  {
  public:

    std::vector<float> times;
    std::vector<T> values;

    std::vector<unsigned int> precisionValues;

    std::vector<int> quickReference;
    std::vector<float> invIntervals;
    float step;


    unsigned int size;

    InterpolationSet(void): step(1), size(0){}

  public:

    inline void Insert(float time, T value)
    {
      assert(time >= 0 && time <= 1.0f);

      unsigned int i = 0;
      unsigned int precision;

      // Iterate over time values till the last minus one
      while (i < size && size > 0)
      {
        // Overwrite value
        if (time == times[i])
        {
          values[i] = value;
          return;
        }
        // New intermediate value
        else if (time < times[i])
        {
          times.emplace(times.begin() + i, time);
          values.emplace(values.begin() + i, value);

          precision = GetPrecision(time);
          precisionValues.emplace(precisionValues.begin() + i,
                                  precision);

          size = (unsigned int) times.size();
          UpdateQuickReference(precision);
          return;
        }
        i++;
      }

      // New highest value
      times.push_back(time);
      values.push_back(value);

      precision = GetPrecision(time);
      precisionValues.push_back(precision);

      size = (unsigned int) times.size();

      UpdateQuickReference(precision);
    }

    inline void Clear()
    {
      times.clear();
      values.clear();
      precisionValues.clear();
      quickReference.clear();
      invIntervals.clear();
      step = 0;
      size = 0;

    }

    inline void Remove(unsigned int i)
    {
      if (i >= size || size == 1)
        return;

      times.erase(times.begin()+i);
      values.erase(values.begin()+i);
      precisionValues.erase(precisionValues.begin()+i);
      size = times.size();

      UpdateQuickReference(GetMaxPrecision());

    }

    inline const T& GetFirstValue()
    {
        return values[0];
    }

    // Implementation exportable to kernel due to avoiding loops
    inline T GetValueKernel(float time)
    {
      if (size > 1 && time > 0.0f)
      {
        assert(time >= 0 && time <= 1.0f);

        int ref = quickReference[floor(time * (quickReference.size()-1))];

        float relTime = glm::clamp((time - times[ref]) * invIntervals[ref],
                                   0.f, 1.f);

        T res = ((1.0f - relTime) * values[ref] + relTime * values[ref+1]);

        return res;
      }
      else
      {
        return values[0];
      }

    }

    // Faster implementation for CPU interpolation.
    inline T GetValue(float time)
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

    unsigned int GetMaxPrecision()
    {
      unsigned int maxPrecision = 0;
      std::vector<unsigned int>::const_iterator it;
      for (it = precisionValues.begin(); it != precisionValues.end(); it++)
      {
        if (*it > maxPrecision)
          maxPrecision = *it;
      }
      return maxPrecision;
    }

    int GetPrecision(float time)
    {
      int precision = 0;

      std::string str = std::to_string( (long double) (time));

      //@sgalindo: this may yield to an underflow value
      unsigned int pos = (unsigned int) str.length()-1;
      while (pos > 0)
      {
        if (str[pos] != '0')
          break;
        pos--;
      }
      str = str.substr(0, pos+1);

      precision = int(str.rfind('.'));
      precision = precision > 0 ? int(str.length()) - precision - 1 : 0;
      precision = pow(10.f, precision);

      return precision;
    }

    void UpdateQuickReference(unsigned int newSize)
    {
      if (newSize == 0)
      {
        std::cerr << "Error: Given quick reference size cannot be zero."
                  << std::endl;
        return;
      }
      else if (newSize != quickReference.size())
      {
        quickReference.resize(newSize);
      }

      quickReference.clear();
      invIntervals.clear();

      step = 1.0f / quickReference.size();

      invIntervals.resize(times.size(), 1.0f);

      if (times.size() > 1)
        for (unsigned int i = 0; i < times.size()-1; i++)
        {
          invIntervals[i] = 1.0f / (times[i+1] - times[i]);
        }

      std::vector<int> limits(times.size());

      for (unsigned int i = 0; i < limits.size()-1; i++)
      {
        limits[i] = int(floor(times[i+1] * quickReference.size()));
      }

      limits[limits.size()-1] = int(quickReference.size());

      unsigned int pos = 0;
      for (unsigned int i = 0; i < quickReference.size(); i++)
      {
        if (i >= (unsigned int )limits[pos])
          pos++;
        quickReference[i] = pos;
      }
    }

    T defaultValue;

  };




}

#endif /* INTERPOLATIONSET_H_ */
