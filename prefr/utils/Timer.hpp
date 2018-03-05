/*
 * Copyright (c) 2014-2018 GMRV/URJC.
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
#ifndef __PREFR__TIMER__
#define __PREFR__TIMER__

#include "error.h"

namespace prefr
{

  namespace utils
  {

    class TimeFrame
    {
    public:

      TimeFrame( const float& period_,
                 const float& offset_,
                 const float& duration_ )
        : period(period_), offset(offset_), duration(duration_){}

      TimeFrame( const TimeFrame& other )
        : period(other.period)
        , offset(other.offset)
        , duration(other.duration)
          //@sgalingo: this is not supported in VS 10 
          // : TimeFrame( other.period, other.offset, other.duration )
      {}

      inline bool check(const float& time) const
      {
        return (time >= offset && time <= offset + duration);
      }

      float period;
      float offset;
      float duration;
    };

    class Timer
    {
    public:

      float timer;

    public:

      Timer()
        : timer( 0 )
      {}

      virtual ~Timer(){}

    protected:

      virtual inline void UpdateTimer(float deltaTime){timer += deltaTime;}

      virtual inline void RestoreTimer() = 0;

    public:

      virtual inline bool InTime() const = 0;
      virtual inline void ResetTimer(){timer = 0;}


    };

    class SingleFrameTimer : public Timer
    {
    private:

      TimeFrame frame;

    public:

      SingleFrameTimer( )
        : Timer( )
        , frame( TimeFrame(0, 0, 0 ) )
      {}

      SingleFrameTimer( float period, float offset, float duration)
        : Timer( )
        , frame( TimeFrame( period, offset, duration ) )
      {}

      inline virtual void SetFrame(const float& period, const float& offset, const float& duration)
      {
        PREFR_DEBUG_CHECK(period > duration, "Duration cannot be greater than period.");

        frame.period = period;
        frame.offset = offset;
        frame.duration = duration;
      }

      inline virtual void SetFrame(const TimeFrame& timeFrame)
      {
        frame.period = timeFrame.period;
        frame.offset = timeFrame.offset;
        frame.duration = timeFrame.duration;
      }

      inline bool InTime() const
      {
        if (frame.duration == 0.0f && timer >= frame.offset + frame.duration)
          return true;
        else
          return frame.check(timer);
      }

      inline bool AfterTime() const
      {
        return (frame.period > 0) &&
          (timer >= (frame.offset + frame.duration));
      }

    protected:
      inline void RestoreTimer()
      {
        if (AfterTime())
          timer -= (frame.duration + frame.period);
      }

      inline void UpdateTimer(float deltaTime)
      {
        if (frame.period > 0)
          timer += deltaTime;
      }

    };

//    class MultiFrameTimer : public Timer
//    {
//    private:
//
//      float last;
//
//      typedef std::vector<TimeFrame> TTimeFrames;
//      TTimeFrames frames;
//
//    public:
//
//      MultiFrameTimer()
//      : Timer()
//      , last(std::numeric_limits<float>::min())
//      {
//
//      }
//
//      inline virtual void AddFrame(const float& start, const float& end)
//      {
//        frames.push_back(TimeFrame(start, end));
//        if (last < frames.back().duration)
//          last = frames.back().duration;
//      }
//
//      inline bool InTime() const
//      {
//        for (TTimeFrames::const_iterator it = frames.begin(); it != frames.end(); it++)
//        {
//          if ((*it).check(timer))
//            return true;
//        }
//
//        return false;
//      }
//
//    protected:
//      inline void RestoreTimer(){if (timer > last) timer -= last;}

//    };

  }
}


#endif /* __Timer__ */
