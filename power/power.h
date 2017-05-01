#ifndef POWER_H
#define POWER_H

#include "battery_res.h"

namespace gpos
{
   namespace power
   {

      enum _battery_level
      {
         BATTERY_VERY_LOW = 0, /** No Bars */
         BATTERY_LOW,      /** 1 Bar */
         BATTERY_MEDIUM,   /** 2 Bars */
         BATTERY_HIGH,     /** 3 Bars */
         BATTERY_FULL,      /** 4 Bars */
         Max = 6
      };

      enum _power_source
      {
         Battery, Charging
      };

      struct _power
      {
         uint8  level;
         uint32 volts;/// Voltage
      } battery_t;

      uint8& battery_level()
      {
         battery_t.level = TP_GetBatterylevel();
         return battery_t.level;
      }

      void print_battery_status(uchar status)
      {
         uchar bt[40] = {NULL};
         estr::set_null<40>(bt);

         TP_ScrSetIcon(ICON_BATTERY, battery_level());

         switch (status)
         {
            case BATTERY_VERY_LOW:
               estr::assign<30>(bt, "Battery LOW!");
               //TP_ShowPicture(45, 18, 124, 52, battery_bad);
               TP_ShowPicture(88, 12, 124, 42, battery_empty_32);
               TP_ScrGotoxyEx(14, 11);
               break;

            case BATTERY_LOW:
               estr::assign<30>(bt, "Battery LOW!");
               TP_ShowPicture(88, 12, 124, 42, battery_1_32);
               TP_ScrGotoxyEx(13, 11);
               break;

            case BATTERY_MEDIUM:
               estr::assign<30>(bt, "Battery GOOD");
               TP_ShowPicture(88, 12, 124, 42, battery_2_32);
               TP_ScrGotoxyEx(12, 11);
               break;
            case BATTERY_HIGH:
               estr::assign<30>(bt, "Battery GOOD");
               TP_ShowPicture(88, 12, 124, 42, battery_4_32);
               TP_ScrGotoxyEx(12, 11);
               break;

            case BATTERY_FULL:
               estr::assign<40>(bt, "Battery FULL,\n Unplug Charger");
               TP_ScrGotoxyEx(8, 11);
               TP_LcdPrintf(bt);
               TP_ShowPicture(88, 12, 124, 42, battery_plugged_32);
               break;
            default:
               estr::assign<30>(bt, "NO Battery,\nInsert Battery");
               TP_ScrGotoxyEx(8, 11);
               TP_LcdPrintf(bt);
               TP_ShowPicture(88, 12, 124, 42, battery_not_found_32);
               break;
         }
         //TP_ScrDrawRect(9, 8, 116, 47)
      }

      uint32& battery_voltage()
      {
         battery_t.volts = TP_GetBatteryValue();
         return battery_t.volts;
      }

      bool battery_low()
      {
         return battery_level() == BATTERY_LOW;
      }

      bool battery_very_low()
      {
         return battery_level() == BATTERY_VERY_LOW;
      }

      bool battery_charging()
      {
         return static_cast<bool>(TP_IsCharging());
      }

      bool source()
      {
         return static_cast<bool>(TP_InCharge());
      }

   }
}

namespace tests
{
   void power_tests()
   {
      using namespace gpos;

      TP_LcdPrintf((uchar*)"Hello World");
      TP_ScrRestore(0);
      TP_ScrCls();
      power::print_battery_status(33);
      timer::start_sec(9);
      TP_ScrRestore(1);
   }
}

#endif//POWER_H
