/*
 * Timer.h
 *
 *  Created on: 15/12/2014
 *      Author: sgalindo
 */

#ifndef __Timer__
#define __Timer__

#include "log.h"

namespace prefr
{

  namespace utils
  {

    class TimeFrame
    {
    public:
      float period, offset, duration;

      TimeFrame(const float& period_, const float& offset_, const float& duration_)
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

    protected:
      inline void RestoreTimer()
      {
        if (timer >= frame.offset + frame.duration)
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
