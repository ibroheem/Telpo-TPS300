
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <TPFile.h>

namespace gpos
{
   bool delete_file(cchar* file_name)
   {
      if (TP_FExist(file_name))
         if (TP_Delete(file_name) == TRUE)
            return true;

      return false;
   }

   int32 size_kb(const int32 &num) { return num / KB; }

   class file_system_t
   {
      public:
         file_system_t() : file_name(), file_descriptor(-1), max_file_size(0), file_size(-1) { }

         bool append(cuchar*, cuint32&);
         bool create();
         bool create_overwrite();
         bool close() { return (TP_Close(file_descriptor)) == TRUE ? true : false; }
         bool delete_file() { return gpos::delete_file(file_name); }
         int32& descriptor() { return file_descriptor; }
         bool exists();

         int32& size() { return file_size; }
         int32& max_size() { return max_file_size; }

         bool is_open() { return err == FSERR_OPENED; }

         bool open();
         bool read_w_open(uint8*);
         bool read(uint8*, cuint32&);

         bool seek(cuint32&, cuint8&, cuint8&);
         bool seek_end(cuint8& direction)   { return seek(0, direction, SEEK_END); }
         bool seek_start(cuint8& direction) { return seek(0, direction, SEEK_SET); }

         void   set_file_name(cchar*);
         void   set_file_size(const int32& sz) { file_size = sz; }
         void   set_max_size(const int32& sz)  { max_file_size = sz; }
         int32& size_w_open();
         int32& size_wo_open();

         bool truncate(const uint32& len) { return TP_Truncate(file_descriptor, len); };
         bool write(cuchar*, cuint32&);
         bool write_w_open(cuchar*, cuint32&);

      private:
         char  file_name[17];
         int32 file_descriptor;
         /** Stores return status (which might also be read size, created file size) **/
         int32 max_file_size, file_size, err;
   } ;

   bool file_system_t::append(cuchar* buf, cuint32& sz)
   {
      return (seek_end(SEEK_FOREWARD)) && write(buf, sz);
   }

   bool file_system_t::create()
   {
      if (exists())
         return true;

      file_size = TP_Create(file_name, max_file_size);

      if (file_size == 0)
         return true;
      return true;
   }

   bool file_system_t::create_overwrite()
   {
      TP_Delete(file_name);
      file_size = TP_Create(file_name, max_file_size);
      if (file_size <= 0)
         return false;
      return true;
   }

   bool file_system_t::exists()
   {
      if(TP_FExist(file_name) == TRUE)
         return true;

      return false;
   }

   void file_system_t::set_file_name(cchar* name)
   {
      estr::assign<17, 16>(file_name, name);
   }

   bool file_system_t::open()
   {
      file_descriptor = TP_Open(file_name);
      if (file_descriptor >= 0)
         return true;
      err = file_descriptor;
      return false;
   }

   bool file_system_t::read_w_open(uchar* buf)
   {
      if (open())
      {
         const size_t& fsize = size_wo_open();

         if (fsize > 0 && read(buf, fsize))
         {
            close();
            return true;
         }
      }
      return false;
   }

   bool file_system_t::read(uint8* data, cuint32& read_sz)
   {
      if (exists() == TRUE)
         if (TP_Read(file_descriptor, data, read_sz) >= 0)
            return true;
      return false;
   }

   bool file_system_t::seek(cuint32& offset, cuint8& direction, cuint8& start_pos)
   {
      return TP_Seek(file_descriptor, offset, direction, start_pos) == TRUE;
   }

   int32& file_system_t::size_w_open()
   {
      if (open())
         file_size = TP_FSize(file_descriptor);

      else
         file_size = -1;

      close();
      return file_size;
   }

   int32& file_system_t::size_wo_open()
   {
      file_size = TP_FSize(file_descriptor);
      return file_size;
   }

   bool file_system_t::write(cuchar* out_buf, cuint32& write_sz)
   {
      if(TP_Write(file_descriptor, out_buf, write_sz) >= 0)
         return true;
      return false;
   }

   bool file_system_t::write_w_open(cuchar* out_buf, cuint32& write_sz)
   {
      if (open())
      {
         if (write_sz > 0 && write(out_buf, write_sz))
         {
            close();
            return true;
         }
      }
      return false;
   }

}

using namespace gpos;

namespace tests
{
   void write_w_open()
   {
#define HASH_SIZE 100
      /*uchar *buf_read = NULL;
      const char  *buf = "{ \"foo\": [-123, \"true\", true, false, null, \"strval\"] }";
      file_system_t fs;

      fs.set_file_name("config_test.cfg");

      if (fs.open())
      {
         int32& fsize = fs.size_wo_open();
         TP_AllocMemory(fsize + 1, (void**)&buf_read);
         estr::set_null(buf_read, fsize + 1);
         TP_LcdPrintf((uchar*)"File Size: %d", fsize);
         TP_LcdPrintf((uchar*)"\nFree Size: %d", TP_FFreeSize(fs.descriptor()));
         TP_LcdPrintf((uchar*)"\n\nStrlen Size: %d", strlen(buf));
         fs.read(buf_read, fsize);

         TP_ShowTextFullScr(&display_info, (uchar*)"%s", buf_read);
      }

      else
      {
         fs.close();
         fs.set_max_size(10 * KB);
         if (fs.create())
         {
            if (fs.open())
            {
               fs.write((uint8*)buf, strlen(buf));
               TP_LcdPrintf((uchar*)"Size: %d", fs.size_wo_open());
               TP_LcdPrintf((uchar*)"\nMax Size: %d", fs.max_size());
            }
         }
      }*/
   }

   void seek_tests()
   {
      uchar js[] = {"\"status\":\"400\""}, out[256] = {NULL};
      file_system_t fl;
      fl.set_file_name("seek");
      fl.set_max_size(_64B);
      fl.delete_file();
      fl.create_overwrite();
      fl.open();
      fl.write(js, sizeof(js) - 1);
      fl.seek(sizeof(js) - 1, SEEK_FOREWARD, SEEK_SET);
      fl.write((uchar*)"message:200", 12);
      //fl.append(js, sizeof js - 1);
      //fl.append(js, sizeof js - 1);
      fl.close();
      fl.open();
      fl.read(out, sizeof out);
      TP_ShowTextFullScr(&display_info, (uchar*)"%s", out);
   }

}

/*void read_tests()
{
   using namespace ArduinoJson;

   uchar* buf_json = PNULL;
   FS_File file = {NULL, 0, 0, 0};;
   FS_Set_File_Name(&file, "activation.cfg");
   file.file_size = FS_File_Size_Open(&file);

   //buf_json = FS_Open_n_Read_dyn(&file, buf_json);
   TP_AllocMemory(sizeof(uchar) * file.file_size + 1, (void**)&buf_json);
   estr::set_null(buf_json, file.file_size + 1);
   FS_Open_n_Read(&file, buf_json, file.file_size);

   if (buf_json)
   {
      cJSON* root = NULL;

      root = cJSON_Parse(buf_json);
      TP_ShowTextFullScr(&display_info, buf_json);

      if (find_toplevel_bool(root, "activated", cJSON_True) == TRUE)
      {
         TP_ShowTextFullScr(&display_info, "File Size: %d", FS_File_Size(&file));
      }
      TP_FreeMemory(&buf_json);
   }
}

void Fs_test_func()
{
   //write_w_open();
   read_tests();
}
*/

#endif//FILE_SYSTEM_H
