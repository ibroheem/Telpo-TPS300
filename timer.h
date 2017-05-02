#ifndef TIMER_H
#define TIMER_H

namespace gpos
{
   namespace timer
   {
      void start_ms(const uint32 delay) ///delay in secs
      {
         int16 timer = TP_TimerAllocate();
         if (timer < 0)
            timer = 0;

         TP_TimerStart(timer, delay);
         while (TP_TimerCheck(timer) > 0) ;
         TP_TimerFree(timer);
      }

      void start_sec(const uint16 delay) //delay in secs
      {
         start_ms(delay * 1000);
      }

      void start_min(const uint16 delay) //delay in secs
      {
         start_sec(delay * 60);
      }

   }
}

#endif//TIMER_H
