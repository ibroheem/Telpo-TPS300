#ifndef FORM_H
#define FORM_H

#include <TP_Singleime.h>
#include "../Display/TP_DisplayAPI.h"

#define TPS_MAX_LR_BUTTON_TITLE_LEN 32
#define KEY_APP_CONFIRM    TP_KEY_MENU
#define KEY_APP_CANCEL     TP_KEY_CANCEL
#define KEY_APP_FUNC       TP_KEY_FUNC
#define KEY_APP_CONFIRM_F  TP_KEY_OK
#define KEY_APP_CANCEL_F   TP_KEY_NONE
#define KEY_APP_FEED       TP_KEY_FEED

namespace gpos
{
   namespace form
   {
      static uint8 Result = 0, MinLen = 1;
      uchar progress_bar_title[30] = {NULL};

      enum input_mode { normal = 0x40 | 0x10 |0x04, password = 0x40 | 0x10 |0x04 | 0x8 };

      struct TPS_LR_ButtonInfo
      {
         uint8 strLButtonTitle[TPS_MAX_LR_BUTTON_TITLE_LEN];
         uint8 strRButtonTitle[TPS_MAX_LR_BUTTON_TITLE_LEN];
         uint8 LKeyValue;
         uint8 RKeyValue;
         uint8 LF_KeyValue;
         uint8 RF_KeyValue;
      };

      struct form_display_t
      {
         uchar title[22], label1[22], label2[22];
         uchar str_lhs[12], str_rhs[12];
         uint8 title_padding, label1_padding, input_box_sz;
         TP_InputMethodE input_method;
      };

      TPS_LR_ButtonInfo ButtonInfo = { "", "", TP_KEY_OK, TP_KEY_CANCEL, TP_KEY_OK, TP_KEY_CANCEL };

      form_display_t display_elements = { {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, 13, 29, 0 };

      void reset_display_elements_values()
      {
         misc::set_bufs_to_null(display_elements.label1, display_elements.label2);
         misc::set_bufs_to_null(display_elements.str_lhs, display_elements.str_rhs);
      }

      void TPS_PrintText(const uint8& left,
                         const uint8& top,
                         const uint8& right,
                         const uint8& bottom,
                         Boolean ifMid, uint8 FONT, const char* pText)
      {
         int16 Len = 0;
         ST_FONT SFont, MutlFont;
         uint32 charspace,linespace;
         if ( NULL == pText )
            return ;

         TP_ScrFontSet( FONT );
         TP_ScrGetFont(&SFont,&MutlFont);
         TP_ScrSpaceGet (&charspace, &linespace );

         TP_SetDisplayArea( left, top, right, bottom );
         //TP_ScrClrRect( left, top, right, bottom );
         TP_ScrClsDisplayArea( );
         TP_ScrAttrSet(0);
         Len = TP_GetMaxCharInRow() - strlen(pText);
         if ( ifMid && Len > 0 )
         {
            if ( CFONT == FONT)
            {
               TP_ScrGotoxyEx(left + Len *(MutlFont.Width / 2 + charspace) / 2,
                              (top+bottom-MutlFont.Height)/2 );
            }
            else if ( ASCII == FONT )
               TP_ScrGotoxyEx( left+Len*(SFont.Width+charspace)/2,(top+bottom-SFont.Height)/2 );

            else
               TP_ScrGotoxyEx( left, top );
         }
         else
            TP_ScrGotoxyEx( left, top );

         TP_LcdPrintf((uchar*)"%s", pText );
         TP_ScrFontSet( CFONT );
      }

      void TPS_DrawInputNumRect( const uint8 left, const uint8 top, const uint16 MaxLen )
      {
         uint8 Len = NumberPrintLen( MaxLen );
         TP_SetDisplayArea( left,top,left+Len*8+10,top+8);
         TP_ScrClrRect(left,top,left+Len*8+10,top+8);
         TP_ScrDrawRect( left,top,left+Len*8+10,top+8);
         TP_ShowInputNum( 1, left+2, top+2 );
      }

      bool single_edit(uchar buf[], size_t MaxLen, input_mode im = normal, bool NumOnly = true)
      {
         Result = 0, MinLen = 1;

         TP_Kbflush();
         TP_ScrCls();
         TP_ScrGotoxy(display_elements.title_padding, 0);
         TP_LcdPrintf(display_elements.title);
         TP_ScrGotoxyEx(2, 33);

         TP_DrawFootMenu(2, 50, 127, 63, 11,
                         display_elements.str_lhs,
                         display_elements.str_rhs);

         TP_ScrDrawRect( 1, 15, MAX_SCREEN_WIDTH - 2, 50 );
         TPS_PrintText(display_elements.label1_padding,
                       17, MAX_SCREEN_WIDTH - 8,
                       31,
                       FALSE,
                       CFONT,
                       (char*)display_elements.label1);

         TP_ScrDrawRect( 24, 42, display_elements.input_box_sz - 18, 45 );

         TP_SetInputModePosition(58, 129);
         TP_SetInputMode(display_elements.input_method);
         TP_SetInputModeControlEx( ButtonInfo.LKeyValue, ButtonInfo.LF_KeyValue,
                                   ButtonInfo.RKeyValue, TP_KEY_CLEAR, TP_KEY_FUNC);

         //TP_SetInputModeControl(TP_KEY_OK, TP_KEY_CANCEL, TP_KEY_POUND);
         TP_SetInputModePosition(50, 54);
         TPS_DrawInputNumRect( 99, 12, MaxLen );
         //estr::set_null(buf, MaxLen + 1);
         TP_ScrFontSet(CFONT);

         do
         {
            TP_SetDisplayArea( 36, 32, display_elements.input_box_sz - 19, 44);
            TP_ScrClrRect(25, 32, 35, 44);
            TP_ScrGotoxy(7, 38);
            if (NumOnly)
               Result = TP_GetString((char *)buf, im, MinLen, MaxLen);
            else
               Result = TP_GetHzString((char *)buf, 0, MaxLen);
            if (Result == 0xFF) /** Cancel Pressed **/
            {
               estr::set_null(buf, MaxLen);
               break;
            }
         } while ( Result != 0xFB );

         misc::reset_screen_pos_display();

         if (Result == 0xFB)
            return true;

         return false;
      }

      bool single_edit2(uchar buf[])
      {
         TP_Kbflush();
         TP_ScrCls();
         TP_ScrGotoxy(display_elements.title_padding, 0);
         TP_LcdPrintf(display_elements.title);
         TP_ScrGotoxyEx(2, 33);

         TP_DrawFootMenu(2, 50, 127, 63, 11,
                         display_elements.str_lhs,
                         display_elements.str_rhs);

         TP_ScrDrawRect( 1, 15, MAX_SCREEN_WIDTH - 2, 50 );
         TPS_PrintText(display_elements.label1_padding,
                       17, MAX_SCREEN_WIDTH - 8,
                       31,
                       FALSE,
                       CFONT,
                       (char*)display_elements.label1);


         TP_SetInputModePosition(58, 129);
         TP_SetInputMode(display_elements.input_method);
         TP_SetInputModeControlEx( ButtonInfo.LKeyValue, ButtonInfo.LF_KeyValue,
                                   ButtonInfo.RKeyValue, TP_KEY_CLEAR, TP_KEY_FUNC);

         TP_ScrFontSet(CFONT);
         TP_SetDisplayArea( 36, 32, display_elements.input_box_sz - 19, 44);
         TP_ScrGotoxyEx(26, 33);
         TP_LcdPrintf((uchar*)"%s", buf);

         while(1)
         {
            if (TP_Kbhit() == 0xFF)
            {
               switch(TP_GetKey())
               {
                  case TP_KEY_OK:
                  misc::reset_screen_pos_display();
                  return true;

                  case TP_KEY_CANCEL:
                  misc::reset_screen_pos_display();
                  return false;
               }
            }
         }
         return false;
      }

      bool single_display(uchar buf[])
      {
         Result = 0, MinLen = 1;

         TP_Kbflush();
         TP_ScrGotoxy(display_elements.title_padding, 0);
         TP_LcdPrintf(display_elements.title);
         TP_ScrGotoxyEx(2, 33);

         TP_DrawFootMenu(2, 50, 127, 63, 11,
                         display_elements.str_lhs,
                         display_elements.str_rhs);

         TP_ScrDrawRect( 1, 15, MAX_SCREEN_WIDTH - 2, 50 );
         TPS_PrintText(display_elements.label1_padding, 17, MAX_SCREEN_WIDTH - 8, 31, FALSE, CFONT,
                       (char*)display_elements.label1);

         //TP_ScrDrawRect( 13, 32, MAX_SCREEN_WIDTH - 18, 44 );
         TP_ScrGotoxyEx( 13, 32);
         TP_LcdPrintf((uchar*)"%s", buf);

         //reset_screen_pos_display();
         while (1)
         {
            TP_NotifyCheck();
            if (TP_Kbhit() == 0xFF)
            {
               Result = TP_GetKey();
               if (Result == TP_KEY_OK)
                  return TRUE;
               else if (Result == TP_KEY_CANCEL)
                  return FALSE;
            }
         }
      }

      bool single_display_int(int val)
      {
         Result = 0, MinLen = 1;

         TP_Kbflush();
         TP_ScrGotoxy(display_elements.title_padding, 0);
         TP_LcdPrintf(display_elements.title);
         TP_ScrGotoxyEx(2, 33);

         TP_DrawFootMenu(2, 50, 127, 63, 11,
                         display_elements.str_lhs,
                         display_elements.str_rhs);

         TP_ScrDrawRect( 1, 15, MAX_SCREEN_WIDTH - 2, 50 );
         TPS_PrintText(display_elements.label1_padding, 17, MAX_SCREEN_WIDTH - 8, 31, FALSE, CFONT,
                       (char*)display_elements.label1);

         //TP_ScrDrawRect( 13, 32, MAX_SCREEN_WIDTH - 18, 44 );
         TP_ScrGotoxyEx( 23, 32);
         TP_LcdPrintf((uchar*)"%d", val);

         //reset_screen_pos_display();
         while (1)
         {
            TP_NotifyCheck();
            if (TP_Kbhit() == 0xFF)
            {
               Result = TP_GetKey();
               if (Result == TP_KEY_OK)
                  return TRUE;
               else if (Result == TP_KEY_CANCEL)
                  return FALSE;
            }
         }
      }


      bool dual_edit(uchar *buf1, uchar *buf2,
                     size_t MaxLen1, size_t MaxLen2, input_mode im = normal)
      {
         Result = 0, MinLen = 1;

         TP_ScrSetIcon(ICON_LOCK, 0);
         TP_Kbflush();
         TP_ScrClrLine(4, 5);
         TP_ScrGotoxy(display_elements.title_padding, 0);
         TP_LcdPrintf(display_elements.title);
         TP_ScrGotoxyEx(10, 16);
         TP_LcdPrintf(display_elements.label1);
         TP_ScrGotoxyEx(10, 33);
         TP_LcdPrintf(display_elements.label2);
         TP_DrawFootMenu(2, 50, 127, 63, 11,
                         display_elements.str_lhs,
                         display_elements.str_rhs);

         TP_ScrDrawRect( 44, 16, MAX_SCREEN_WIDTH - 14, 28 );
         TP_ScrDrawRect( 44, 32, MAX_SCREEN_WIDTH - 14, 44 );

         TP_SetInputModePosition(56, 153); //don't show
         TP_SetInputMode(display_elements.input_method);
         TP_SetInputModeControlEx( ButtonInfo.LKeyValue, ButtonInfo.LF_KeyValue,
                                   ButtonInfo.RKeyValue, TP_KEY_CLEAR, TP_KEY_FUNC);

         TPS_DrawInputNumRect( 50, 52, MaxLen1 );
         //set_bufs_to_null(buf1, buf2);

         do
         {
            TP_SetDisplayArea( 47, 16, MAX_SCREEN_WIDTH - 18, 28);
            TP_ScrGotoxy(4, 38);
            Result = TP_GetString((char *)buf1, im, MinLen, MaxLen1);
            if (Result == 0xFF) /** Cancel Pressed **/
            {
               estr::set_null(buf1, MaxLen1);
               estr::set_null(buf2, MaxLen2);
               return FALSE;
            }

         } while ( Result != 0xFB);

         TP_ScrDrawRect( 44, 16, MAX_SCREEN_WIDTH - 14, 28 );
         TP_Kbflush();

         TPS_DrawInputNumRect( 50, 52, MaxLen2 );
         do
         {
            TP_SetDisplayArea( 47, 32, MAX_SCREEN_WIDTH - 18, 44);
            TP_ScrGotoxy(4, 38);
            Result = TP_GetString((char *)buf2, im, MinLen, MaxLen2);
            if (Result == 0xFF) /** Cancel Pressed **/
            {
               estr::set_null(buf1, MaxLen1);
               estr::set_null(buf2, MaxLen2);
               return dual_edit(buf1, buf2, MaxLen1, MaxLen2);
            }

         } while ( Result != 0xFB);

         misc::reset_screen_pos_display();

         if (Result == 0xFB)
            return true;

         return false;
      }

      bool dual_edit_ex(uchar buf1[], uchar buf2[],
                        size_t MaxLen1, size_t MaxLen2)
      {
         Result = 0, MinLen = 1;

         TP_ScrSetIcon(ICON_LOCK, 0);
         TP_Kbflush();
         TP_ScrClrLine(4, 5);
         TP_ScrGotoxy(30, 0);
         TP_LcdPrintf(display_elements.title);
         TP_ScrGotoxyEx(2, 16);
         TP_LcdPrintf(display_elements.label1);
         TP_ScrGotoxyEx(2, 33);
         TP_LcdPrintf(display_elements.label2);
         TP_DrawFootMenu(2, 50, 127, 63, 11,
                         display_elements.str_lhs,
                         display_elements.str_rhs);

         TP_ScrDrawRect( 48, 16, MAX_SCREEN_WIDTH - 5, 28 );
         TP_ScrDrawRect( 48, 32, MAX_SCREEN_WIDTH - 5, 44 );

         TP_SetInputModePosition(56, 153); //don't show
         TP_SetInputMode(display_elements.input_method);
         TP_SetInputModeControlEx( ButtonInfo.LKeyValue, ButtonInfo.LF_KeyValue,
                                   ButtonInfo.RKeyValue, TP_KEY_CLEAR, TP_KEY_FUNC);

         TPS_DrawInputNumRect( 50, 52, MaxLen1 );
         //set_bufs_to_null(buf1, buf2);

         do
         {
            TP_SetDisplayArea( 51, 16, MAX_SCREEN_WIDTH - 8, 28);
            TP_ScrGotoxy(4, 38);
            Result = TP_GetString((char *)buf1, 0x40 | 0x10 |0x04, MinLen, MaxLen1);
            if (Result == 0xFF) /** Cancel Pressed **/
            {
               misc::set_bufs_to_null(buf1, buf2);
               return FALSE;
            }

         } while ( Result != 0xFB);

         TP_ScrDrawRect( 48, 16, MAX_SCREEN_WIDTH - 5, 28 );
         TP_Kbflush();

         TPS_DrawInputNumRect( 50, 52, MaxLen2 );
         do
         {
            TP_SetDisplayArea( 51, 32, MAX_SCREEN_WIDTH - 8, 44);
            TP_ScrGotoxy(4, 38);
            Result = TP_GetString((char *)buf2, 0x40 | 0x10 |0x04, MinLen, MaxLen2);
            if (Result == 0xFF) /** Cancel Pressed **/
            {
               misc::set_bufs_to_null(buf1, buf2);
               return dual_edit_ex(buf1, buf2, MaxLen1, MaxLen2);
            }

         } while ( Result != 0xFB);

         misc::reset_screen_pos_display();

         if (Result == 0xFB)
            return true;

         return false;
      }
   }
}

#endif//FORM_H
