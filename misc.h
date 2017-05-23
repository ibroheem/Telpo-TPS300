#ifndef MISC_H
#define MISC_H

#include "Ext/typedefs.h"
#include "macros.h"
#include "file_system.h"
#include "ttime.h"
#include <ArduinoJson.h>
#include <ctime>

#define to_string_c(val) #val


namespace gpos
{
   namespace misc
   {
      ArduinoJson::StaticJsonBuffer<KB> json_buffer_KB;
      ArduinoJson::StaticJsonBuffer<_100KB> json_buffer_50KB;
      ArduinoJson::StaticJsonBuffer<_100KB> json_out_buffer_50KB;

      void set_bufs_to_null(uchar buf1[], uchar buf2[])
      {
         estr::set_null(buf1, sizeof(buf1));
         estr::set_null(buf2, sizeof(buf2));
      }

      typedef struct _JSON_BODY
      {
         uchar body_arr[_50KB];
         uint32   sz;
      } json_body_t;

      json_body_t json_body = {{NULL}, 0};

      enum JsonMode { Array, Object};

      int extract_json(char *http_response, uchar* dest, JsonMode mode = Object)
      {
         char *pos = NULL, *pos_end = NULL;

         if(mode == Object)
            pos = strstr(http_response, "{\"");
         else if(mode == Array)
            pos = strstr(http_response, "[");

         if (pos)
         {
            if(mode == Object)
               pos_end = strrchr(http_response, '}') + 1;
            else if(mode == Array)
               pos_end = strrchr(http_response, ']') + 1;
            if (pos_end)
            {
               estr::set_null(dest, strlen((char*)dest));

               for (uint32 i= 0; i < (pos_end - pos); ++i)
                  dest[i] = pos[i];
            }
         }
         return 0;
      }

      int extract_json(char *http_response, json_body_t& jbody, JsonMode mode = Object)
      {
         char *pos = NULL, *pos_end = NULL;

         if(mode == Object)
            pos = strstr(http_response, "{\"");
         else if(mode == Array)
            pos = strstr(http_response, "[");

         if (pos)
         {
            if(mode == Object)
               pos_end = strrchr(http_response, '}') + 1;
            else if(mode == Array)
               pos_end = strrchr(http_response, ']') + 1;
            if (pos_end)
            {
               //jbody.sz = strlen(pos) - strlen(pos_end);
               jbody.sz = 0;
               estr::set_null(jbody.body_arr, strlen((char*)jbody.body_arr));

               for (uint32 &i= json_body.sz; i < (pos_end - pos); ++i)
                  jbody.body_arr[i] = pos[i];
            }
         }
         return 0;
      }

      int extract_json(uchar *http_response, json_body_t& jbody, JsonMode mode = Object)
      {
         return extract_json((char*)http_response, jbody, mode);
      }

      bool response_body(char *http_response, json_body_t& jbody)
      {
         int16 index = estr::str_index<int16>(http_response, "\r\n\r\n");
         if (index != -1)
         {
            estr::set_null(jbody.body_arr, sizeof jbody.body_arr);
            index    += 4;
            jbody.sz = strlen(http_response) - index;
            estr::copy_from_n(http_response, index, jbody.body_arr, jbody.sz);
            return true;
         }
         return false;
      }

      bool response_body(char *http_response, char* dest)
      {
         int16 index = estr::str_index<int16>(http_response, "\r\n\r\n");
         if (index != -1)
         {
            index += 4;
            estr::copy_from_n(http_response, index, dest, strlen(http_response) - index);
            return true;
         }
         return false;
      }

      bool response_body(uchar *http_response, uchar* dest)
      {
         return response_body((char*)http_response, (char*)dest);
      }

      void create_config_file()
      {
         file_system_t fl;
         fl.set_file_name("config.cfg");
         bool b = fl.exists();
         if(!b)
         {
            fl.set_max_size(_5KB);
            fl.create_overwrite();
         }
      }

      template <class T>
      void clear_json_buffer(T& instance)
      {
         instance = T();
      }

      void reset_screen_pos_display()
      {
         TP_SetDisplayArea( 0, 0, 127, 63);
         TP_ScrGotoxyEx(0, 0);
      }

      int32& last_error() { return error_code; }

      void wait_till_key_press()
      {
         while (1)
         {
            if (TP_Kbhit() == 0xFF)
            {
               TP_GetKey();
               TP_Kbflush();
               break;
            }
         }
      }
   }

   namespace random
   {
      char arr[16], tmp[16];
      uint16 len;

      double unifRand()
      {
         return rand() / double(RAND_MAX);
      }

      double unifRand(double a, double b)
      {
         return (b - a) * unifRand() + a;
      }

      int unifRand(int n)
      {
         if (n < 0) n = -n;
         if (n==0) return 0;

         int guard = (int) (unifRand() * n) +1;
         return (guard > n)? n : guard;
      }

      // Reset the random number generator with the system clock.
      void seed()
      {
         srand(date_time::get_time());
      }

      void randomise()
      {
         seed();
         int32 j = 0, k = 0;

         for (int i = 0; i < 5; ++i)
         {
            j += unifRand() * 795849;
            k += unifRand() * 981872;
         }

         sprintf(tmp, "%d", j);
         estr::assign(arr, 15, tmp);

         sprintf(tmp, "%d", k);
         estr::append(arr, 15, tmp);

         len = strlen(arr) - 1;

         char c = arr[0];
         arr[0] = arr[len];
         arr[len] = c;
      }
   }
}


#endif//MISC_H
