// -*- C++ -*-
// <rtc-template block="description">
/*!
 * @file  RTno2ProxyRTC.cpp
 * @brief RTno2Proxy
 *
 */
// </rtc-template>

#include "RTno2ProxyRTC.h"

// Module specification
// <rtc-template block="module_spec">
#if RTM_MAJOR_VERSION >= 2
static const char* const rtno2proxyrtc_spec[] =
#else
static const char* rtno2proxyrtc_spec[] =
#endif
  {
    "implementation_id", "RTno2ProxyRTC",
    "type_name",         "RTno2ProxyRTC",
    "description",       "RTno2Proxy",
    "version",           "1.0.0",
    "vendor",            "sugarsweetrobotics",
    "category",          "rtno",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.port_name", "COM3",
    "conf.default.baudrate", "57600",

    // Widget
    "conf.__widget__.port_name", "text",
    "conf.__widget__.baudrate", "text",
    // Constraints

    "conf.__type__.port_name", "string",
    "conf.__type__.baudrate", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RTno2ProxyRTC::RTno2ProxyRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_in0In("in0", m_in0),
    m_out0Out("out0", m_out0)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RTno2ProxyRTC::~RTno2ProxyRTC()
{
}



RTC::ReturnCode_t RTno2ProxyRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("in0", m_in0In);
  
  // Set OutPort buffer
  addOutPort("out0", m_out0Out);

  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("port_name", m_port_name, "COM3");
  bindParameter("baudrate", m_baudrate, "57600");
  // </rtc-template>

  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RTno2ProxyRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/


//RTC::ReturnCode_t RTno2ProxyRTC::onStartup(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTC::onShutdown(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


RTC::ReturnCode_t RTno2ProxyRTC::onActivated(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTno2ProxyRTC::onDeactivated(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTno2ProxyRTC::onExecute(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


//RTC::ReturnCode_t RTno2ProxyRTC::onAborting(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTC::onError(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTC::onReset(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTC::onStateUpdate(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTC::onRateChanged(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}



extern "C"
{
 
  void RTno2ProxyRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(rtno2proxyrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<RTno2ProxyRTC>,
                             RTC::Delete<RTno2ProxyRTC>);
  }
  
}
