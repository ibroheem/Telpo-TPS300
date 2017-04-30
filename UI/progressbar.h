#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "ui_elements.h"

namespace gpos
{
   namespace ui
   {
      namespace progress_bar
      {
         static uint8 progress_bar_sz = 100;
         char text_to_draw[32] = {NULL};

         struct Rect
         {
            uint8 left, top, right, bottom, &max;
         } rect = {0,0,0,0, progress_bar_sz};

         void do_nothing()
         {

         }

         void draw_char_xy(uint8 x, uint8 y, uint8 count, char ch = '.', const int16& delay = 100)
         {
            TP_ScrGotoxyEx(x, y);
            estr::set_null(text_to_draw, sizeof text_to_draw);

            for (uint8 i = 0; i < count; ++i)
            {
               estr::append<30>(text_to_draw, ch);
               timer::start_ms(delay);
               TP_LcdPrintf((uchar*)"%s", text_to_draw);
               TP_ScrGotoxyEx(x, y);
            }
            TP_ScrGotoxy(0, 0);
         }

         void draw_dots_xy(uint8 x, uint8 y, uint8 count)
         {
            TP_ScrGotoxyEx(x, y);
            static char text[16] = {NULL};
            if (strlen(text) == 7)
               estr::erase(text, 3, 8);
            for (int i = 0; i < count; ++i)
            {
               estr::append<7>(text, '-');
               timer::start_ms(500);
               TP_LcdPrintf((uchar*)"%s", text);
               TP_ScrGotoxyEx(x, y);
            }
            TP_ScrGotoxy(0, 0);
         }

         void draw_bar_term_init(uint8 from, const uint8& to)
         {
            TP_ScrGotoxy(1, 6);

            for (; from < to; ++from)
            {
               timer::start_ms(100);
               TP_ScrDrawRect(from, 55, to, 58);
            }
            TP_ScrGotoxy(0, 0);
         }

         void draw_bar_term_init_xy(uint8 left,
                                    const uint8& top,
                                    const uint8& right,
                                    const uint8& bottom, uint16 delay)
         {
            for (; left < right; ++left)
            {
               timer::start_ms(delay);
               TP_ScrDrawRect(left, top, right, bottom);
            }
            TP_ScrGotoxy(0, 0);
         }

         ///Used with struct Rect global instance
         void draw_bar_term_init_xy(Rect& rc = rect, bool cnt_ms = true, uint16 delay = 100)
         {
            for (; rc.left < rc.right; ++rc.left)
            {
               if(cnt_ms)
                  timer::start_ms(delay);
               TP_ScrDrawRect(rc.left, rc.top, rc.max, rc.bottom);
            }
            TP_ScrGotoxy(0, 0);
         }

         void draw_bar_range_xy(uint8 left,
                                uint8 top,
                                uint8 right,
                                uint8 bottom)
         {
            for (; left < right; ++left)
            {
               timer::start_ms(100);
               TP_ScrDrawRect(left, top, progress_bar_sz, bottom);
            }
            TP_ScrGotoxy(0, 0);
         }

         void draw_short_bar_left(uchar* msg)
         {
            TP_ScrClrLine(0, 2);
            TP_ScrDrawRect(21, 10, 100, 40);
            TP_ScrGotoxyEx(24, 11);
            TP_LcdPrintf((uint8*)msg);
         }

         void draw_short_bar_center(uchar* msg)
         {
            TP_ScrClrLine(0, 2);
            TP_ScrDrawRect(21, 10, 100, 40);
            TP_ScrGotoxyEx(33, 11);
            TP_LcdPrintf((uint8*)msg);
         }

         void draw_long_bar_center(uchar* msg)
         {
            TP_ScrClrLine(0, 2);
            TP_ScrClrRect(21, 10, 22, 63);
            TP_ScrClrRect(99, 10, 100, 63);
            //TP_ScrDrawRect(5, 10, 122, 40);
            TP_ScrDrawRect(11, 10, 117, 40);
            TP_ScrGotoxyEx(33, 11);
            TP_LcdPrintf((uint8*)msg);
            TP_ScrGotoxyEx(2, 11);
         }

         void draw_long_bar_left(uchar* msg)
         {
            TP_ScrClrLine(0, 2);
            TP_ScrClrRect(21, 10, 22, 63);
            TP_ScrClrRect(99, 10, 100, 63);
            TP_ScrDrawRect(11, 10, 117, 40);
            TP_ScrGotoxyEx(8, 11);
            TP_LcdPrintf(msg);
         }

         void draw_vlong_bar_left(uchar* msg)
         {
            TP_ScrClrLine(0, 2);
            TP_ScrClrRect(21, 10, 22, 63);
            TP_ScrClrRect(99, 10, 100, 63);
            TP_ScrDrawRect(4, 10, 124, 40);
            TP_ScrGotoxyEx(2, 11);
            TP_LcdPrintf(msg);
         }

         void draw_error_bar_left(uchar* msg)
         {
            TP_ScrClrLine(0, 2);
            TP_ScrClrRect(21, 10, 22, 63);
            TP_ScrClrRect(99, 10, 100, 63);
            TP_ScrDrawRect(1, 10, 126, 50);
            TP_ScrGotoxyEx(4, 11);
            TP_LcdPrintf((uint8*)msg);
         }

         void progress_bar_test2()
         {
            draw_short_bar_center((uchar*)"Sending");
            //draw_short_bar_left(" Connecting");
            draw_bar_range_xy(25, 30, 80, 34);
            timer::start_min(1);

            draw_long_bar_center((uchar*)"Receiving");
            draw_bar_range_xy(80, 30, 85, 34);
         }

         void progress_bar_test()
         {
            //progress_bar_term_init(10, 100);
            //progress_bar_range(10, 100);
            /*TP_ScrDrawRect(21, 10, 100, 40);
            TP_ScrGotoxyEx(30, 11);
            TP_LcdPrintf((uint8*)"Connecting");
            progress_bar_range_xy(34, 30, 50, 34);*/

            TP_ScrClrLine(0, 2);
            TP_ScrDrawRect(21, 10, 100, 40);
            TP_ScrGotoxyEx(38, 11);
            TP_LcdPrintf((uint8*)"Sending");
            draw_bar_range_xy(25, 30, 80, 34);

            TP_ScrClrLine(0, 2);
            TP_ScrClrRect(21, 10, 22, 63);
            TP_ScrClrRect(99, 10, 100, 63);
            TP_ScrDrawRect(5, 10, 122, 40);
            TP_ScrGotoxyEx(33, 11);
            TP_LcdPrintf((uint8*)"Receiving");
            draw_bar_range_xy(80, 30, 85, 34);
         }

      }

      using namespace ui::progress_bar;

      void recv(bool use_bar = true)
      {
         rect.right = 100;
         if(use_bar)
            TP_ScrSetIcon(ICON_DOWN, 1);
         timer::start_ms(500);
         (use_bar ? draw_bar_term_init_xy(rect, false) : do_nothing());
         timer::start_ms(500);
         if(use_bar)
            TP_ScrSetIcon(ICON_DOWN, 0);
      }

      void send(bool use_bar = true)
      {
         rect.right = 70;
         if(use_bar)
            TP_ScrSetIcon(ICON_UP, 1);
         (use_bar ? draw_bar_term_init_xy(rect) : do_nothing());
         if(use_bar)
            TP_ScrSetIcon(ICON_UP, 0);
      }

      void send_recv(bool use_bar = true)
      {
         /*rect.right = 70;
         TP_ScrClrLine(6, 7);
         TP_ScrGotoxyEx(39, 45);
         TP_ScrSetIcon(ICON_UP, 1);
         TP_LcdPrintf((uchar*)"Sending");

         draw_bar_term_init_xy(rect);

         rect.right = 100;
         TP_ScrClrLine(6, 7);
         TP_ScrGotoxyEx(39, 45);
         TP_ScrSetIcon(ICON_UP, 0);
         TP_ScrSetIcon(ICON_DOWN, 1);
         TP_LcdPrintf((uchar*)"Receiving");*/

         rect.right = 70;
         TP_ScrSetIcon(ICON_UP, 1);
         (use_bar ? draw_bar_term_init_xy(rect) : do_nothing());
         TP_ScrSetIcon(ICON_UP, 0);

         rect.right = 100;
         TP_ScrSetIcon(ICON_DOWN, 1);
         (use_bar ? draw_bar_term_init_xy(rect) : do_nothing() );
         TP_ScrSetIcon(ICON_DOWN, 0);
      }
   }
}

namespace tests
{
   using namespace gpos::ui::progress_bar;
   //using namespace gpos::misc;

   void progressbar_tests(uint8 opt)
   {
      switch(opt)
      {
         case 1:
            while(1)
            {
               draw_long_bar_left((uchar*)" Activating Device");
               TP_ScrGotoxyEx(53, 26);
               for(uint8 i = 0; i < 101; i += 2)
               {
                  TP_LcdPrintf((uchar*)"%d%%", i);
                  timer::start_ms(100);
                  TP_ScrClrRect(53, 26, 76, 39);
               }
               //draw_xy(38, 28, 5, '-');
               TP_ScrCls();
            }
            break;

         case 2:
            while(1)
            {
               draw_long_bar_left((uchar*)" Activating Device");
               TP_ScrGotoxyEx(53, 26);
               draw_char_xy(38, 28, 9, '.', 200);
               TP_ScrCls();
            }
            break;
         case 3:
            ui::message_box(" Device Activation",
                            "Please wait, activation in progress.", false);
            TP_ScrGotoxyEx(53, 41);
            for(uint8 i = 0; i < 101; i += 2)
            {
               TP_ScrClrRect(53, 41, 76, 50);
               TP_LcdPrintf((uchar*)"%d%%", i);
               timer::start_ms(100);
            }
            break;
         case 4:
            ui::message_box(" Device Activation",
                            "Please wait, activation in progress.", false);
            while(1)
            {
               TP_ScrGotoxyEx(53, 26);
               TP_ScrClrRect(38, 41, 89, 50);
               draw_char_xy(38, 41, 9, '.', 500);
            }
            break;
         case 5:
            ui::message_box(" Device Activation",
                            "Please wait, activation in progress.", false);
            while(1)
            {
               TP_ScrGotoxyEx(53, 26);
               TP_ScrClrRect(38, 57, 89, 60);
               draw_bar_term_init_xy(38, 57, 89, 60, 10);
            }
            break;
      }
   }

   void progressbar()
   {
      draw_long_bar_left((uchar*)" Activating Device");

      rect.left   = 20;
      rect.right  = 60;
      rect.top    = 28;
      rect.bottom = 32;
      rect.max    = 105;

      TP_ScrGotoxyEx(39, 45);
      TP_ScrSetIcon(ICON_UP, 1);
      TP_LcdPrintf((uchar*)"Connecting");
      draw_bar_term_init_xy(rect);

      //draw_long_bar_left((uchar*)" Logging In Device");
      rect.right  = 100;

      TP_ScrClrLine(6, 7);
      TP_ScrGotoxyEx(39, 45);
      TP_ScrSetIcon(ICON_UP, 0);
      TP_ScrSetIcon(ICON_DOWN, 1);
      TP_LcdPrintf((uchar*)"Receiving");
      draw_bar_term_init_xy(rect);
      TP_ScrSetIcon(ICON_DOWN, 0);
   }
}

#endif//PROGRESSBAR_H
