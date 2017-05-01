#ifndef ESTR_H
#define ESTR_H

#include <string>
#include <stdarg.h>

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
         if(*remPat == '\0' )
         {
            printf ("match found \n");
            return inplen - strlen(inpText + 1) ;
         }
         if(remTxt == NULL)
         {
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
   typedef const char cchar;
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

   size_t strnlen (register const char *s, size_t maxlen)
   {
      register const char *e;
      size_t n;

      for (e = s, n = 0; *e && n < maxlen; e++, n++)
         ;
      return n;
   }

   template <size_t N, class T>
   void set_null(T* ptr)
   {
      for (size_t i = 0; i < N; ptr[i++] = NULL)
         ;
   }

   template <class T>
   void set_null(T* ptr, size_t size)
   {
      for (size_t i = 0; i < size; ptr[i++] = NULL)
         ;
   }

   inline size_t size(char* val)
   {
      return strlen(val);
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
      size_t size_val  = strlen(val),
             size_dest = strlen(dest);

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
      size_t size_dest  = strlen(dest);

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

   template <class T, class T2>
   void assign(T* rhs, size_t CAP_RHS, T2* val)
   {
      set_null(rhs, CAP_RHS);
      append(rhs, CAP_RHS - 1, val, strlen(val));
   }

   template <size_t CAP_RHS, class T, class T2>
   void assign(T* rhs, T2* val)
   {
      set_null<CAP_RHS>(rhs);
      append(rhs, CAP_RHS - 1, val, strlen(val));
   }

   template <size_t CAP_RHS, size_t N, class T, class T2>
   void assign(T* rhs, T2* val)
   {
      set_null<CAP_RHS>(rhs);
      append<0, CAP_RHS - 1, N - 1>(rhs, val);
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
   int find(T *inpText, T *pattern)
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

   template <size_t SIZE_LHS, size_t SIZE_RHS>
   bool is_equal(cchar* lhs, cchar* rhs)
   {
      bool ret = false;

      if (SIZE_LHS == SIZE_RHS)
      {
         for (size_t i = 0; i < SIZE_RHS; ++i)
         {
            if (lhs[i] != rhs[i])
               return false;
         }
         ret = true;
      }
      return ret;
   }

   bool is_equal(cchar* lhs, cchar* rhs)
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
         for(size_t i = start, j = 0; i < sz &&j < sz_dest; ++i, ++j)
            dest[j] = src[i];
   }

}

void estr_tests_contains()
{
   using namespace estr;

   char s[21], s2[21];

   set_null<21>(s);
   set_null<21>(s2);

   estr::append(s, 20, "Hello World");

   cout <<boolalpha <<estr::contains<4, 2>("HEAD", "HE") << endl;
   cout <<boolalpha <<estr::contains<4, 2>("HEAD", "ED") << endl;
   cout <<boolalpha <<estr::contains<4, 1>("HEAD", "H") << endl;
   //cout <<boolalpha <<findSubstr("HEAD", "H") << endl;
   cout <<"ing position in HEADing: " <<estr::find<cchar, cchar>("HEADing", "ing") << endl;
   cout <<"g position in HEADing: " <<estr::find<cchar, cchar>("HEADing", "g") << endl;

   copy_from_n(s, 0, s2, 10);
   assign<20, 9>(s2, "Goodlings");
   cout <<s2 << endl;

   assign<20, sizeof(s)/sizeof(*s)>(s2, s);
   cout <<s2 << endl;

   assign<21>(s2, s);
   cout <<s2 << endl;

   assign<21>(s2, "Salam Alaikum...@#$%D.............loping");
   cout <<s2 << endl;
}

void estr_tests()
{
   char str[30];// = {NULL};
   char str2[21];// = {NULL};

   estr::set_null(str, 30);
   estr::set_null<21>(str2);

   estr::append(str, 30, "Good Men Alive and healthy");
   estr::append(str2, 20, "Good Men Alive and h", 14);

   //for (estring<char, 20>::iterator b = str.begin(); b != str.end(); ++b)
   //cout << *b;

   cout <<"str: "  <<str  <<endl;
   cout <<"str2: " <<str2 <<endl;
   cout <<boolalpha <<"str == str2: " <<(str == str2) <<endl;
   cout <<(str == "Good Men Alive and healthy") <<endl;
   cout <<"is_equal: " <<estr::is_equal(str, "Good Men Alive and healthy") <<endl;
   cout <<"is_equal<>: " <<estr::is_equal<26, 26>(str, "Good Men Alive and healthy") <<endl;
   cout <<"is_less_than: " <<estr::is_less_than(str, "Good Men Alive and healthy") <<endl;
   cout <<estr::is_less_than(str2, "Good Men Alive and healthy") <<endl;

   //estr::const_iterator e = estr::end_c<30>(str);
   estr::const_iterator e = estr::end(str);
   for (estr::iterator b = estr::begin(str); b != e; ++b)
      cout << *b;
}

#endif//ESTR_H
