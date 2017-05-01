#ifndef TIME_H
#define TIME_H


char dt_tm_buf[16] = {PNULL},
     dt_tmp[12] = {PNULL},
     tm_tmp[12] = {PNULL};

namespace gpos
{
   namespace date_time
   {
      uint8 hrs = DateTime.Time.Hours, tmp_min;
      char time_date_buf[5] = {NULL};
      TP_DateTime tmpDateTime = {0};

      void show_date_time2()
      {
         TP_GetDateTime(&DateTime);

         if (hrs > DateTime.Time.Hours)
         {
            hrs = DateTime.Time.Hours;
            //return ;
         }

         hrs = DateTime.Time.Hours;

         if (hrs > 0 && hrs < 12)
         {
            TP_ShowTextSingleLine(8, 36, 127, 48, CFONT, ALIGN_LEFT,
                                  (uchar*)"%02d/%02d/%4d  %02d:%02dAM"/*:%02d*/,
                                  DateTime.Date.Day,
                                  DateTime.Date.Month,
                                  DateTime.Date.Year,
                                  hrs,
                                  DateTime.Time.Minutes);
         }

         else if (hrs == 0)
         {
            TP_ShowTextSingleLine(8, 36, 127, 48, CFONT, ALIGN_LEFT,
                                  (uchar*)"%02d/%02d/%4d  %02d:%02dAM",
                                  DateTime.Date.Day,
                                  DateTime.Date.Month,
                                  DateTime.Date.Year,
                                  12,
                                  DateTime.Time.Minutes);
         }

         else if (hrs == 12)
         {
            TP_ShowTextSingleLine(8, 36, 127, 48, CFONT, ALIGN_LEFT,
                                  (uchar*)"%02d/%02d/%4d  %02d:%02dPM",
                                  DateTime.Date.Day,
                                  DateTime.Date.Month,
                                  DateTime.Date.Year,
                                  12,
                                  DateTime.Time.Minutes);
         }

         else if (hrs > 12 && hrs <= 24)
         {
            TP_ShowTextSingleLine(8, 36, 127, 48, CFONT, ALIGN_LEFT,
                                  (uchar*)"%02d/%02d/%4d  %02d:%02dPM",
                                  DateTime.Date.Day,
                                  DateTime.Date.Month,
                                  DateTime.Date.Year,
                                  hrs - 12,
                                  DateTime.Time.Minutes);
         }

         tmp_min = DateTime.Time.Minutes;
      }

      void set_date(const TP_Date& date)
      {
         DateTime.Date = date;
      }

      void set_date(const uint8& yy, const uint8& mm, const uint8& dd)
      {
         DateTime.Date.Day   = dd;
         DateTime.Date.Month = mm;
         DateTime.Date.Year  = yy;
      }

      void set_time(const uint8& hrs, const uint8& min)
      {
         DateTime.Time.Hours   = hrs;
         DateTime.Time.Minutes = min;
      }

      inline void use_system_time()
      {
         TP_GetDateTime(&DateTime);
      }

      ///tm is hh:mm or hh:mm:ss format
      inline void set_date(cchar* dt)
      {
         TP_DateTime tmpdt = {0};
         TP_GetDateTime(&tmpdt);
         estr::set_null<5>(time_date_buf);
         time_date_buf[0] = dt[0];
         time_date_buf[1] = dt[1];

         uint16 tmp = estr::to_int(time_date_buf);
         tmpdt.Date.Day = tmp;

         time_date_buf[0] = dt[3];
         time_date_buf[1] = dt[4];

         tmp = estr::to_int(time_date_buf);
         tmpdt.Date.Month = tmp;

         for (uint8 i = 0; i < 4; ++i)
         {
            time_date_buf[i] = dt[i + 6];
         }

         tmp = estr::to_int(time_date_buf);
         tmpdt.Date.Year = tmp;

         TP_SetDateTime(&tmpdt);
      }

      ///dt is dd-mm-yyyy format
      inline void set_time(cchar* tm)
      {
         TP_DateTime dateTime = {0};
         TP_GetDateTime(&dateTime);
         estr::set_null<5>(time_date_buf);
         time_date_buf[0] = tm[0];
         time_date_buf[1] = tm[1];

         dateTime.Time.Hours = estr::to_int(time_date_buf);

         time_date_buf[0] = tm[3];
         time_date_buf[1] = tm[4];

         dateTime.Time.Minutes = estr::to_int(time_date_buf);

         //int res = TP_SetDateTime(&dateTime);
         //TP_ShowTextFullScr(&display_info, (uchar*)"res: %d\n", res);
         //TP_LcdPrintf((uchar*)"res: %d\n", res);
         //timer::start_sec(5);
      }

      uint32 today_int()
      {
         TP_GetDateTime(&tmpDateTime);
         return tmpDateTime.Date.Year + tmpDateTime.Date.Month + tmpDateTime.Date.Day;
      }

      enum date_fromat { none, ddmmyyyy, yyyymmdd };

      void to_ddmmyy(char* date, char separator = '-')
      {
         char tmp[10] = {NULL};

         for(uint8 i = 6, j = 0; i < 10; ++i, ++j)
            tmp[i] = date[j];

         tmp[0] = date[8];
         tmp[1] = date[9];
         tmp[2] = separator;
         tmp[3] = date[5];
         tmp[4] = date[6];
         tmp[5] = separator;

         estr::assign<11>(date, tmp);
      }

      void build_date(char* dt, char* dest, date_fromat format = ddmmyyyy)
      {
         estr::set_null(dest, strlen(dest));
         for(uint8 i = 0, j = 0; i < 8; ++i, ++j)
         {
            if (j == 4 || j == 7)
               dest[j++]= '-';

            dest[j] = dt[i];
         }
         if (format == ddmmyyyy)
            to_ddmmyy(dest);
      }

      void build_time(char* dt, char* dest)
      {
         estr::set_null(dest, strlen(dest));
         for(uint8 i = 8, j = 0; i < 12; ++i, ++j)
         {
            if (j == 2 || j == 5)
               dest[j++]= ':';

            dest[j] = dt[i];
         }
      }

      void build_time_w_sec(char* dt, char* dest)
      {
         estr::set_null(dest, 12);
         for(uint8 i = 8, j = 0; i < 14; ++i, ++j)
         {
            if (j == 2 || j == 5)
               dest[j++]= ':';

            dest[j] = dt[i];
         }
      }

      void build_time_am_pm(char* dt, char* dest)
      {
         char  ctmp[8] = {PNULL};

         build_time(dt, ctmp);
         //set_time(ctmp);
         TP_GetDateTime(&DateTime);
         uint8& hrs = DateTime.Time.Hours;
         estr::set_null(dest, strlen(dest));

         if (hrs > 0 && hrs < 12)
            sprintf(dest, "%02d:%02dAM", hrs, DateTime.Time.Minutes);

         else if (hrs == 0)
            sprintf(dest, "%02d:%02dAM", 12, DateTime.Time.Minutes);

         else if (hrs == 12)
            sprintf(dest,"%02d:%02dPM", 12, DateTime.Time.Minutes);

         else if (hrs > 12 && hrs <= 24)
            sprintf(dest, "%02d:%02dPM", hrs - 12, DateTime.Time.Minutes);
      }

      bool isLeap(const unsigned short& year)
      {
          return (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));
      }

      unsigned short daysPerMonth[] = {31, (isLeap(2014) ? 29 : 28), 31, 30, 31, 30, 31,
                                       31, 30, 31, 30, 31};

      unsigned short& days_in_month(const unsigned short& mnth)
      {
         return daysPerMonth[mnth];
      }

      unsigned short& days_in_month()
      {
         TP_GetDateTime(&tmpDateTime);
         return daysPerMonth[tmpDateTime.Date.Month];
      }

      unsigned short days_since_jan(const unsigned short& mnth)
      {
         unsigned short ret = 0;
         for (size_t i = 0; i < mnth; ++i)
            ret += daysPerMonth[i];

         return ret;
      }

      unsigned short days_since_jan()
      {
         TP_GetDateTime(&tmpDateTime);
         return days_since_jan(tmpDateTime.Date.Month);
      }

      unsigned short days_in_year()
      {
         return days_since_jan(12);
      }

      void get_date_time()
      {
         TP_GetDateTime(&DateTime);
      }

      unsigned int get_time()
      {
         TP_GetDateTime(&tmpDateTime);
         short tm_yday = days_since_jan(tmpDateTime.Date.Month),
               tm_year = tmpDateTime.Date.Year - 1900;

         unsigned int res = (tm_yday * 86400)   +
                            (tm_year - 70) * 31536000 + ((tm_year - 69) / 4) * 86400 -
                            ((tm_year - 1) / 100) * 86400 + ((tm_year + 299) / 400)*86400;


         res += tmpDateTime.Time.Seconds +
                (tmpDateTime.Time.Minutes * 60) + (tmpDateTime.Time.Hours * 3600);

         return res - 1558800;
      }

      uint8& day_of_month()
      {
         return DateTime.Date.Day;
      }

      uint8& month()
      {
         return DateTime.Date.Month;
      }

      uint16& year()
      {
         return DateTime.Date.Year;
      }

      void show_date_time()
      {
         TP_GetDateTimeEx(dt_tm_buf, 16);

         date_time::build_time_am_pm(dt_tm_buf, tm_tmp);
         date_time::build_date(dt_tm_buf, dt_tmp);
         TP_ScrGotoxyEx(7, 36);
         TP_LcdPrintf((uchar*)"%s\t\t%s", dt_tmp, tm_tmp);
      }
   }
}

namespace tests
{
   void date_time_tests()
   {
      using namespace gpos;
      while (1)
      {
         TP_GetDateTimeEx(dt_tm_buf, 16);
         date_time::build_time_w_sec(dt_tm_buf, tm_tmp);
         date_time::build_date(dt_tm_buf, dt_tmp);
         //date_time::to_ddmmyy(dt_tmp, '/');
         TP_LcdPrintf((uchar*)"%s\n%s\n%s", dt_tm_buf, dt_tmp, tm_tmp);
         //timer::start_ms(200);
         TP_ScrCls();
         //TP_ShowTextFullScr(&display_info, (uchar*)"%s\n%s\n%s", dt_tm_buf, dt_tmp, tm_tmp);
      }
   }
}

#endif//TIME_H
