#ifndef telpo_src_server_h
#define telpo_src_server_h

#undef NO_NULLPTR_SUPPORT 1

#include <TPNet.h>
#include <etl/array.h>
#include "network.h"

namespace gpos
{
   class socket_t
   {
      public:
         enum socket_mode { tcp, udp };
         socket_t() : host_addr(), ip_addr(), m_base_url(), port_num(8085), socket_num(-1) { }

         bool connect_default_tcp() { return connect(tcp); }
         bool connect_default_udp() { return connect(udp); }

         bool disconnect();

         uchar* base_url() { return m_base_url; }
         void   set_base_url(cchar* _addr) { estr::assign<16, uchar>(m_base_url, _addr); }

         uchar* host_address() { return host_addr; }
         void   set_host_address(cchar* _addr) { estr::assign<128, uchar>(host_addr, _addr); }

         void ip_address();
         void set_ip_address(cuchar*);

         uint16& port() { return port_num; }
         void set_port(const uint16& _port) { port_num = _port; };

         bool receive(uchar* );
         bool send(cuchar* );
         bool send(cuchar*, const uint32&);
         bool send_n_receive(cuchar*, uchar*);

         bool set_socket_address();
         bool set_socket_address(const uchar*, const uint16& );
         void set_socket_mode(socket_mode);

         void set_socket_number(const int16& num) { socket_num= num; }
         int16& socket_number() { return socket_num; }

      private:
         bool connect(socket_mode);

      private:
         uchar  host_addr[128], ip_addr[16], m_base_url[16];
         uint16 port_num;
         int16  socket_num;
         TP_SockAddrT socket_addr;
   };

   using namespace gpos;

   bool socket_t::disconnect()
   {
      int16 result = TP_NetCloseSocket(socket_num);
      if (result < 0)
      {
         if (result == -9)
            socket_num = -1;
      }
      else
      {
         socket_num = -1;
         return true;
      }

      return false;
   }

   /** Connect To a socket Server using parameters provided by ServerParam_t */
   bool socket_t::connect(socket_mode mode)
   {
      if (TP_Kbhit() == 0xFF && TP_GetKey() == TP_KEY_CANCEL)
      {
         network::gprs_network.disconnect();
         return disconnect();
      }

      /*if (socket_num == -1)
         set_socket_mode(mode);*/

      if (mode == tcp)
         socket_num = TP_NetSocket(AF_INET, NET_SOCK_STREAM, 0);
      else
         socket_num = TP_NetSocket(AF_INET, NET_SOCK_DGRAM, 0);

      if (socket_num < 0)
      {
         TP_NetCloseSocket(socket_num);
         error_code = socket_num;
         socket_num = -1;
         return false;
      }
      else
      {
         TP_NetIOCtl(socket_num, CMD_IO_SET, 1);

         set_socket_address();

         TP_DNSServer(0, "8.8.8.8", 80);

         int16 ret = TP_NetConnect(socket_num, &socket_addr, sizeof(TP_SockAddrT));

         if (ret < 0)
         {
            if (ret == -9)
               socket_num = -1;
         }
         else
            return true;
      }
      return false;
   }

   bool socket_t::receive(uchar* response_buf)
   {
      int8 retries = 0;
      while (retries++ < 3)
      {
         error_code = TP_NetRecv(socket_num, response_buf, _20KB, 0);
         if (error_code >= 0) ///Sent
            return true;

         else
         {
            ///handle errors
         }
      }
      return false;
   }

   bool socket_t::send(cuchar* request_buf)
   {
      return send(request_buf, strlen((char*)request_buf));
   }

   bool socket_t::send(cuchar* request_buf, const uint32& sz)
   {
      int8 retries = 0;
      while (retries++ < 1)
      {
         error_code = TP_NetSend(socket_num, request_buf, sz, 0);
         if (error_code >= 0) ///Sent
            return true;

         else
         {
            ///handle errors
         }
      }
      return false;
   }

   bool socket_t::send_n_receive(cuchar* request_buf, uchar* response_buf)
   {
      retries = 0;
      while (retries++ < 3)
      {
         if (TP_NetSend(socket_num, request_buf, strlen((char*)request_buf), 0) >= 0) ///Sent
         {
            estr::set_null(response_buf, strlen((char*)response_buf) + 1);
            timer::start_ms(500);
            if (TP_NetRecv(socket_num, response_buf, _20KB, 0) >= 0) ///Sent
               return true;
         }

         else
         {
            ///handle errors
         }
      }
      return false;
   }

   void socket_t::set_ip_address(cuchar* ip)
   {
      estr::append<0, 16, 16>(ip_addr, ip);
   }

   bool socket_t::set_socket_address()
   {
      if (TP_SockAddrSet(&socket_addr, ip_addr, port_num) == 0)
         return true;
      return false;
   }

   bool socket_t::set_socket_address(const uchar* ip, const uint16& port)
   {
      set_ip_address(ip);
      set_port(port);
      return set_socket_address();
   }

   void socket_t::set_socket_mode(socket_mode mode)
   {
      if (mode == tcp)
         socket_num = TP_NetSocket(AF_INET, NET_SOCK_STREAM, 0);
      else
         socket_num = TP_NetSocket(AF_INET, NET_SOCK_DGRAM, 0);
   }

   struct http_client : public socket_t
   {
         enum Method { CONNECT, DELETE, GET, POST, PUT };
         http_client() : request_str(buf_http_request), response_str(buf_http_response)
         {
            //srv.connect_default_tcp();
         }

         template <class T>
         void add_header_param(T*);

         ///Set Header Param name only, then adding values later
         template <class T>
         void add_header_param_name(T*);

         template <class T>
         void add_header_param_value(T*);

         template <class T, class T2>
         void add_header_param_value(T*, T2*, char);

         template <class T, class T2>
         void add_header_param(T*, T2*);

         template <class T, class T2>
         void add_header_param_ln(T*, T2*);

         template<class T, size_t N>
         void add_header_params(const etl::array<T, N> &, const etl::array<T, N> &);

         template <class T>
         void add_http_method(Method, T*);

         template <class T, class T2>
         void add_http_method(T*, T2*);

         void add_static_params();
         void clear_request() { estr::set_null(request_str, strlen((char*)request_str)); }
         void finalize() { estr::append(request_str, KB, "\r\n"); }

         template <class T>
         T* request() { return request_str; }
         size_t request_size() const { return strlen((char*)request_str); }

         template <class T>
         T* response() { return response_str; }
         size_t response_size() const { return strlen((char*)response_str); }

         bool send_n_receive_http() { return send_n_receive(request_str, response_str); }

      public:
         uchar* request_str, *response_str;
         //socket_t m_server;
   };

   template <class T>
   void http_client::add_header_param(T* name_n_value)
   {
      estr::append(request_str, KB, name_n_value);
   }

   template <class T, class T2>
   void http_client::add_header_param(T* name, T2* value)
   {
      estr::append(request_str, KB, name);
      estr::append(request_str, KB, ": ");
      estr::append(request_str, KB, value);
   }

   template <class T, class T2>
   void http_client::add_header_param_ln(T* name, T2* value)
   {
      estr::append(request_str, KB, name);
      estr::append(request_str, KB, ": ");
      estr::append(request_str, KB, value);
      estr::append(request_str, KB, "\r\n");
   }

   template<class T, size_t N>
   void http_client::add_header_params(const etl::array<T, N> & names,
                                       const etl::array<T, N> & values)
   {
      for (size_t i = 0; i < names.size(); ++i)
      {
         add_header_param(names[i], values[i]);
      }
   }

   template <class T>
   void http_client::add_header_param_name(T* name)
   {
      estr::append(request_str, KB, name);
      estr::append(request_str, KB, ": ");
   }

   template <class T, class T2>
   void http_client::add_header_param_value(T* name, T2* value, char sepearator)
   {
      estr::append(request_str, KB, name);
      estr::append<KB>(request_str, sepearator);
      estr::append(request_str, KB, value);
      estr::append(request_str, KB, ",");
   }

   template <class T>
   void http_client::add_http_method(Method mthd, T* value)
   {
      switch (mthd)
      {
         case CONNECT:
            break;

         case DELETE:
            break;

         case GET:
            break;

         case POST:
            estr::append(request_str, KB, "POST ");
            break;

         default:
            break;
      }

      estr::append(request_str, KB, "http://");
      estr::append(request_str, KB, host_address());
      estr::append(request_str, KB, base_url());
      if(value)
         estr::append(request_str, KB, value);
      estr::append(request_str, KB, " HTTP/1.1\r\n");
   }

   template <class T, class T2>
   void http_client::add_http_method(T* mth, T2* value)
   {
      estr::append(request_str, KB, mth);
      estr::append(request_str, KB, value);
      estr::append(request_str, KB, "HTTP/1.1\r\n");
   }

   void http_client::add_static_params()
   {
      add_header_param_ln("imei", device.get_imei<uchar>());
      add_header_param_ln("Host", host_address());
      add_header_param("Connection: keep-alive\r\n"
                       "Content-Length: 0\r\n"
                       "Cache-Control: no-cache\r\n"
                       "Accept: */*\r\n"
                       "Accept-Encoding", "gzip,deflate\r\n");
   }

}

namespace tests
{
   void socket_tests()
   {
      using namespace gpos::network;
      using namespace gpos;
      http_client srv;

      srv.set_ip_address((uchar*)"33.208.44.83");
      srv.set_port(8086);
      srv.set_host_address("33.208.44.83");
      srv.connect_default_tcp();

      srv.add_header_param("POST "
                     "http://33.208.44.83:8086/api/devices HTTP/1.1\r\n");
      srv.add_header_param("Host: 33.208.44.83:8085\r\n");
      srv.add_header_param(
                           "Connection: keep-alive\r\n"
                           "Content-Length: 0\r\n"
                           "User-Agent: Sample UA/2.0\r\n"
                           "Cache-Control: no-cache\r\n"
                           "Accept: * /*\r\n"
                           "Accept-Encoding", "gzip,deflate\r\n\r\n");

      if (srv.send_n_receive(srv.request_str, srv.response_str))
      {
         using namespace gpos;
         TP_PopupMessageTimed((uchar*)"Request Sent", 1000);
         TP_PopupMessageTimed((uchar*)"Response received", 1000);
         misc::extract_json(srv.response_str, misc::json_body);
      }
   }
}

#endif//telpo_src_server_h
