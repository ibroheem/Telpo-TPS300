#ifndef PRINTER_H
#define PRINTER_H

#include <TPPrint.h>
#include "misc.h"
#include "timer.h"
#include "ui_elements.h"

namespace gpos
{
   namespace printer
   {
      struct printer_t
      {
            enum StatusCode
            {
               PrintSucceed = 0x00,
               Waiting = 0x00,
               PrintSessStarted = 0x01,
               NotResponding = 0xEE,
               OutOfPaper = 0xFA,
               BufferFull = 0xFB,
               OverHeat = 0xFC,
               AccessDenied = 0xFD,
               NotReady = 0xFF
            };

            printer_t();

            void add_newlines(const uint8& = 1);
            bool add_string(cuchar*);
            bool add_string(cuchar*, cuchar*);

            void add_to_buffer(const uint16& val) { buffer += val; }

            uint16& buffer_size() { return buffer; }

            bool clear_str() { return TP_PrnClearString() == 0x00; }

            void handle_errors(const int16&);

            bool print();
            bool print(cuchar*);

            bool safe_to_add() { return buffer < 1024; }

         private:
            uint16 buffer;
      };

      using namespace gpos::printer;

      printer_t::printer_t() : buffer(0)
      {
         TP_PrnInit();
      }

      void printer_t::add_newlines(const uint8& cnt)
      {
         for (uint8 i = 0; i < cnt; ++i)
            add_string((uchar*)"\r\n");
      }

      bool printer_t::add_string(cuchar* val)
      {
         return add_string((uchar*)"%s", val);
      }

      bool printer_t::add_string(cuchar* fmt, cuchar* val)
      {
         size_t sz = strlen((cchar*)val);
         if (buffer + sz)
         {
            int16 err = TP_PrnAddString(fmt, val);
            if (err == 0x00)
            {
               buffer += sz;
               return true;
            }
            handle_errors(err);
            return false;
         }
         handle_errors(BufferFull);
         return false;
      }

      void printer_t::handle_errors(const int16& error_code)
      {
         switch (error_code)
         {
            case Waiting:
               ui::message_box(ui::information, "Printer Waiting");
               break;

            case PrintSessStarted:
               ui::message_box(ui::information,"Print Session Started");
               break;

            case NotResponding:
               ui::message_box(ui::error, "Printer Not Responding");
               break;

            case OutOfPaper:
               ui::message_box(ui::warning, "Printer Out Of Paper");
               break;

            case BufferFull:
               //ui::message_box("Printer Buffer Full");
               break;

            case OverHeat:
               ui::message_box(ui::warning, "Printer Over Heat");
               break;

            case AccessDenied:
               ui::message_box(ui::fatal_error, "Printer Access Denied");
               break;

            default:
               ui::message_box(ui::error, "Printer not ready");
               break;
         }
      }

      bool printer_t::print()
      {
         error_code = TP_PrnString();
         switch (error_code)
         {
            case PrintSucceed:
               return true;

            default:
               break;
         }

         return false;
      }

   }

   printer::printer_t printer_o;

}

#endif//PRINTER_H
 
