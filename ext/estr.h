#ifndef ESTR_H
#define ESTR_H

#include <cstring>
#include <stdarg.h>
#include "typedefs.h"

using namespace std;

int findSubstr(char *inpText, char *pattern)
{
   int inplen = strlen(inpText);
   while(inpText != NULL)
   {
      char *remTxt = inpText,
           *remPat = pattern;

      if(inplen < (int)strlen(remPat)) {
         /*printf ("length issue remTxt %s \nremPath %s \n",remTxt,remPat);*/
         return -1;
      }

      while (*remTxt++ == *remPat++) {
         printf ("remTxt %s \nremPath %s \n",remTxt,remPat);
         if(*remPat == '\0' ) {
            printf ("match found \n");
            return inplen - strlen(inpText + 1) ;
         }
         if(remTxt == NULL) {
            return -1;
         }

      }
      remPat = pattern;

      inpText++;
   }
   return -1;
}

namespace estr
{
   typedef char*  iterator;
   typedef cchar* const_iterator;

   int sprintf(char * str, const char * format, ... )
   {// Here you can redfine your input before continuing to compy with standard inputs
       va_list args;
       va_start(args, format);
       int ret = vsprintf(str,format, args);// This still uses standaes formating
       va_end(args);
       return ret;// Before return you can redefine it back if you want...
   }

   char *strrev(char *_string)
   {
      int length = strlen(_string);
      char *result = (char*)malloc(length + 1);
      if( result != NULL )
      {
         int i,j;
         result[length] = '\0';
         for ( i = length - 1, j = 0;   i >= 0;   i--, j++ )
            result[j] = _string[i];
      }
      return result;
   }

   template <class T>
   size_t strnlen (register T *s, size_t maxlen)
   {
      register T *e;
      size_t n;

      for (e = s, n = 0; *e && n < maxlen; e++, n++)
         ;
      return n;
   }

   template <size_t N, class T>
   T* set_null(T* ptr)
   {
      for (size_t i = 0; i < N; ptr[i++] = NULL)
         ;
      return ptr;
   }

   template <class T>
   T* set_null(T* ptr, size_t size)
   {
      for (size_t i = 0; i < size; ptr[i++] = NULL)
         ;
      return ptr;
   }

   template <class T>
   inline size_t size(T* val)
   {
      return strlen((char*)val);
   }

   template <class T>
   inline size_t length(T* val)
   {
      size_t i;
      for (i = 0; val[i++] != '\0'; )
         ;
      return i - 1;
   }

   template <size_t CAP_DEST, class T>
   T* append(T* dest, char val)
   {
      size_t i = strlen((char*)dest);
      if (i < CAP_DEST)
         dest[i] = val;

      return dest;
   }

   template <class T>
   T* append(T* dest, size_t Cap, char val)
   {
      size_t i = strlen((char*)dest);
      if (i < Cap)
         dest[i] = val;

      return dest;
   }

   template <size_t CAP_DEST, class T, class T2>
   T* append(T* dest, T2* val)
   {
      size_t sz_val = strlen((char*)val);
      for(size_t i = strlen((char*)dest), j = 0; j != sz_val && i != CAP_DEST; ++i, ++j)
      {
         if (val[j] != '\0')
            dest[i] = val[j];
      }

      return dest;
   }
/**
   * Append string (val) to another string(dest).
   *
   * @author Ibroheem
   *
   * @param START[template]: where append begins in dest
   * @param CAP_DEST[template]: dest's capacity
   * @param CAP_VAL[template]:  val's capacity
   *
   * @param des: where string is copied to
   * @param val: where string is copied from
   *
   * @return void
   *
   * @date 2016
*/
   template <size_t START, size_t CAP_DEST, size_t CAP_VAL, class T, class T2>
   T* append(T* dest, T2* val)
   {
      for(size_t i = START, j = 0; i != CAP_DEST && j < CAP_VAL; ++i, ++j)
      {
         if (val[j] != '\0')
            dest[i] = val[j];
      }
      return dest;
   }

/**
   * Append string (val) to another string(dest).
   *
   * @author Ibroheem
   *
   * @param des: where string is copied to
   * @param dest_cap: destination capacity
   * @param val: where string is copied from
   *
   * @return void
   *
   * @date 2016
*/

   template <class T, class T2>
   T* append(T* dest, size_t dest_cap, T2* val)
   {
      size_t size_val  = strlen((char*)val),
             size_dest = strlen((char*)dest);

      for(size_t i = size_dest, j = 0; i != dest_cap && j < size_val; ++i, ++j)
      {
         if (val[j] != '\0')
            dest[i] = val[j];
      }
      return dest;
   }

   template <class T, class T2>
   T* append(T* dest, size_t src_cap, T2* val, size_t n_element)
   {
      size_t size_dest  = strlen((char*)dest);

      for(size_t i = size_dest, j = 0; i != src_cap && j < n_element; ++i, ++j)
      {
         if (val[j] != '\0')
            dest[i] = val[j];
      }
      return dest;
   }

   template <size_t N, class T, class T2>
   void assign_arr(T* rhs, T2 (&val)[N])
   {
      set_null<N>(rhs);
      append<0, N - 1 , sizeof(val)/sizeof(*val) - 1>(rhs, val);
   }

   template <size_t CAP_RHS, class T, class T2>
   void assign(T* rhs, T2* val)
   {
      set_null<CAP_RHS>(rhs);
      append(rhs, CAP_RHS - 1, val, strlen((char*)val));
   }

   template <size_t CAP_RHS, size_t N, class T, class T2>
   void assign(T* rhs, T2* val)
   {
      set_null<CAP_RHS>(rhs);
      append<0, CAP_RHS - 1, N - 1>(rhs, val);
   }

   template <class T, class T2>
   void assign(T* rhs, const size_t& cap, T2* val)
   {
      set_null(rhs, cap);
      append(rhs, cap, val);
   }

   template <class T>
   T*  begin(T* val) { return &val[0]; }
   template <class T>
   const T* cbegin(const T* val) { return &val[0]; }

   template <class T>
   T*  end(T* val) { return &val[strlen(val)]; }
   template <class T>
   const T* cend(const T* val) { return &val[strlen(val)]; }

   template <size_t N>
   char*  end_c(char* val) { return &val[N]; }

   template <size_t N>
   cchar* end_c(cchar* val){ return &val[N];}

   ///Returns true if rhs is found in lhs. For the template version Sizes must be correct
   template <size_t SIZE_LHS, size_t SIZE_RHS>
   bool contains(cchar* lhs, cchar* rhs)
   {
      bool ret = false;

      if (strstr(lhs, rhs) != NULL)
         ret = true;
      return ret;
   }

   template <class T, class T2>
   bool contains(T* lhs, T* rhs)
   {
      bool ret = false;

      if (strstr(lhs, rhs) != NULL)
         ret = true;
      return ret;
   }

   template <class T, class T2>
   int find(T *inpText, T2 *pattern)
   {
      int inplen = strlen(inpText);
      while(inpText != NULL)
      {
         cchar *remTxt = inpText,
              *remPat = pattern;

         if(inplen < (int)strlen(remPat))
            return -1;

         while (*remTxt++ == *remPat++)
         {
            if(*remPat == '\0' )
               return inplen - strlen(inpText+1) ;

            if(remTxt == NULL)
               return -1;
         }
         remPat = pattern;

         inpText++;
      }
      return -1;
   }

   template <size_t SIZE_RHS, class T, class T2>
   bool is_equal(T* lhs, T2* rhs)
   {
      bool b = true;
      for (size_t i = 0; i < SIZE_RHS; ++i)
      {
         if (lhs[i] != rhs[i])
            b = false;
         else
            b = true;
      }
      return b;
   }

   template <size_t SIZE_LHS, size_t SIZE_RHS, class T, class T2>
   bool is_equal(T* lhs, T2* rhs)
   {
      if (SIZE_LHS == SIZE_RHS)
      {
         for (size_t i = 0; i < SIZE_RHS; ++i)
         {
            if (lhs[i] != rhs[i])
               return false;
         }
         return true;
      }
      return false;
   }

   template <class T, class T2>
   bool is_equal(T* lhs, T2* rhs)
   {
      bool ret = false;
      size_t sz_rhs = strlen(rhs), sz_lhs = strlen(lhs);

      if (sz_lhs == sz_rhs)
      {
         for (size_t i = 0; i < sz_rhs; ++i)
         {
            if (lhs[i] != rhs[i])
               return false;
         }
         ret = true;
      }
      return ret;
   }

   bool is_less_than(cchar* lhs, cchar* rhs)
   {
      bool ret = false;

      if (strlen(lhs) < strlen(rhs))
         ret = true;

      return ret;
   }

   template <class T, class T2>
   void copy_from_n(T* src, size_t start, T2* dest, size_t sz_dest)
   {
      size_t sz = size(src);
      if (start < sz)
         for(size_t i = start, j = 0; i < sz && j < sz_dest; ++i, ++j)
            dest[j] = src[i];
   }

   template <class T, class T2>
   void copy_from_n(T*  src,  const size_t& start, const size_t& end,
                    T2* dest, size_t sz_dest)
   {
      if (start < strlen(src))
         for(size_t i = start, j = 0; i < end && j < sz_dest; ++i, ++j)
            dest[j] = src[i];
   }

   template <class POS_T, class T >
   void erase(T* str, POS_T index)
   {
      size_t sz = size(str);
      if (index < sz)
      {
         str[index] = '\0'; //set element to null
         if (index < sz - 1)
         {
            uchar tmp;
            for (size_t i = index + 1; i < sz; ++i) //move elements forward
            {
               tmp = str[i];
               str[index++] = tmp;
               str[index] = '\0';
            }
         }
      }
   }

   template <class POS_T, class T>
   void erase(T* str, POS_T start, POS_T end)
   {
      size_t sz = size(str);
      if (end < sz && start < end)
      {
         for (size_t i = start; i < end; ++i) //move elements backwards
            str[i] = '\0';

         if (start < sz - 1)
         {
            uchar tmp;
            for (size_t i = end + 1; i <= sz; ++i) //move elements backwards
            {
               tmp = str[i];
               str[start++] = tmp;
               str[start] = '\0';
            }
         }
      }
   }

   template <size_t Cap, class T>
   void create_space_in_front(T* src, size_t spaces = 1)
   {
      --spaces;
      size_t sz_src = strlen(src);
      if ((Cap - sz_src) > spaces)
      {
         char ctmp;
         for (size_t i = 0, j = spaces; i < sz_src + spaces; ++i)
         {
            ctmp = src[j];
            src[j] = src[i];
            src[i] = '\0';
         }
      }
   }

   template <class T, class T2>
   void prepend(T* dest, T2* src)
   {
      size_t sz_dest = strlen(dest),
             sz_src  = strlen(src);
      for (size_t i = 0, j = 0; i < sz_dest && j < sz_src; ++i, ++j)
      {
         if (src[j]!= '\0')
         {
            //dest[]
         }
      }
   }

   template <class T>
   T exponent(const int &num, const int &power)
   {
      T res = 1;
      for (int i = 0; i < power; ++i)
         res *= num;
      return res;
   }

   long to_int(const char *s)
   {
      size_t i_end = std::strlen(s);
      long res = 0;
      for (size_t i = 0; i < i_end; ++i)
      {
         if(s[i] == '.')
         {
            res = res / exponent<long>(10, i_end - i);
            break;
         }
         if (!isdigit(s[i]) != 0)
         {
            res /= exponent<long>(10, i_end - i);
            break;
         }
         res +=  (exponent<long>(10, i_end - i)) * (s[i] - '0');
      }
      return res/10;
   }

   double to_double(const char *s)
   {
      size_t i_end = std::strlen(s);
      double res = 0;
      for (size_t i = 0; i < i_end; ++i)
      {
         if(s[i] == '.')
         {
            res = res / exponent<long>(10, i_end - i);
            char tmp[12] = {PNULL};
            copy_from_n(s, i + 1, tmp, 12);
            double real = static_cast<double>(to_int(tmp)) / exponent<long>(10, strlen(tmp)-1);
            res += real;
            return res/10;
            break;
         }
         if (!isdigit(s[i]) != 0)
         {
            res /= exponent<long>(10, i_end - i);
            break;
         }
         res +=  (exponent<long>(10, i_end - i)) * (s[i] - '0');
      }
      return res/10;
   }

   template <class T>
   T str_index(const char *s, const char *t)
   {
      T i, j, k;
      for (i = 0; s[i] != '\0'; i++)
      {
         for (j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
            ;
         if (k > 0 && t[k] == '\0')
            return i;
      }
      return -1;
   }
}

template <class T>
T* begin(T* _buffer)
{
   return &_buffer[0];
}

template <size_t SIZE, class T>
T* end(T* _buffer)
{
   return &_buffer[SIZE];
}

#endif//ESTR_H
