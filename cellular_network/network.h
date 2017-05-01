#ifndef NETWORK_H
#define NETWORK_H

#include <TPPPP.h>
#include <TPNet.h>
#include <TPNetwork.h>

#define NETWORK_MAX_TIMEOUT 10000
#define NETWORK_TIMEOUT_TILL_SUCCESS -2

namespace gpos
{
   namespace network
   {
      int16 status_w_code()
      {
         int16 result = TP_WlPPPCheck();
         estr::set_null<_64B>(err_buf);

         switch (result)
         {
            case WL_RET_ERR_NOSIM:
               sprintf((char*)err_buf, "No SIM card");
               return WL_RET_ERR_NOSIM;
            case WL_RET_ERR_NOREG:
               sprintf((char*)err_buf, "Not registered to network");
               return WL_RET_ERR_NOREG;
            case WL_RET_ERR_REGING:
               sprintf((char*)err_buf, "Registering to network");
               break;
            case WL_RET_ERR_LINEOFF:
               sprintf((char*)err_buf, "Idle");
               break;
            case WL_RET_ERR_PPP_BRK:
               sprintf((char*)err_buf, "PPP connection has been resetted");
               break;
            case WL_RET_ERR_DIALING:
               result = WL_RET_ERR_DIALING;
               sprintf((char*)err_buf, "PPP dialing");
               break;

            case 0:
               result = TP_NetworkGetStatus();
               switch (result)
               {
                  case SRVCSTATUS_REGISTERED:
                  {
                     sprintf((char*)err_buf, "PPP connected");
                     result = RT_SUCCESS_BY_ONE;
                  } break;

                  case SRVCSTATUS_NETWORK_REJECTED:
                  case SRVCSTATUS_NOT_REGISTER:
                  case SRVCSTATUS_REGISTERING:
                  case SRVCSTATUS_UNKNOWN:
                  {
                     sprintf((char*)err_buf, "PPP connected, but service unavailable");
                     result = -1;
                  } break;

                  default:
                     sprintf((char*)err_buf, "Unknow Status");
                     break;
               }
               break;

            default:
               sprintf((char*)err_buf, "Unknow Status");
               break;
         }
         return result;
      }

      bool status() { return (status_w_code() == RT_SUCCESS_BY_ONE) ? true : false; }

      class gprs_network_t
      {
         public:
            enum Status { Connected, Disconnected, max = 2 };
            gprs_network_t() : m_apn(), m_username(), m_password(), m_is_connected(false) { }

            uchar* apn() { return m_apn; }
            uchar* password() { return m_password; }
            uchar* username() { return m_username; }

            void set_apn(cchar* _apn)        { estr::assign<64, uchar>(m_apn, _apn); }
            void set_password(cchar* _pass)  { estr::assign<24, uchar>(m_password, _pass); }
            void set_username(cchar* _uname) { estr::assign<24, uchar>(m_username, _uname); }

            uint8& signal_level();

            bool is_connected() { return m_is_connected; }

            bool connect();
            bool disconnect();

         private:
            uint8 m_signal_level;
            uchar m_apn[64], m_username[24], m_password[24];
            bool  m_is_connected;
      };

      using namespace gpos::network;

      bool gprs_network_t::connect()
      {
         retries = 0;
         while (retries++ < 1)
         {
            if (TP_Kbhit() == 0xFF && TP_GetKey() == TP_KEY_CANCEL)
               return disconnect();

            //disconnect();
            m_is_connected = TP_WlPPPLogin(m_apn, m_username, m_password, 0, 0, 99999) == 0;
            if (m_is_connected)
               break;
         }
         return m_is_connected;
      }

      bool gprs_network_t::disconnect()
      {
         return TP_WlPPPLogout() == 0;
      }

      uint8& gprs_network_t::signal_level()
      {
         int16 ret = TP_WlGetSignal(&m_signal_level);
         if (ret < 0)
            return m_signal_level;
         return m_signal_level;
      }

      gprs_network_t gprs_network;
   }
}

namespace tests
{
   void gprs_network_tests()
   {
      using namespace gpos::timer;
      using namespace gpos::network;

      int16 ret = 0;

      gprs_network_t net;
      net.set_apn("web.gprs.mtnnigeria.net");
      net.set_username("web");
      net.set_password("web");

      if (net.connect())
      {
         ret = status_w_code();
      }
      TP_ScrCls();
      TP_ScrDrawRect(0, 0, 127, 63);//draw border
      TP_ScrGotoxy(3, 0);
      TP_LcdPrintf((uint8*)"Network Status:", ret);
      TP_ScrGotoxy(3, 1);
      TP_LcdPrintf((uint8*)"%s", err_buf);
      TP_ScrGotoxy(3, 2);
      TP_LcdPrintf((uint8*)"Signal level: %d", net.signal_level());
      start_sec(1);
      net.disconnect();
      //doOperatorSelectManual();
   }
}

#endif//NETWORK_H
