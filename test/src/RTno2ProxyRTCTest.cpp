// -*- C++ -*-
// <rtc-template block="description">
/*!
 * @file  RTno2ProxyRTCTest.cpp
 * @brief RTno2Proxy (test code)
 *
 */
// </rtc-template>

#include "RTno2ProxyRTCTest.h"

// Module specification
// <rtc-template block="module_spec">
#if RTM_MAJOR_VERSION >= 2
static const char* const rtno2proxyrtc_spec[] =
#else
static const char* rtno2proxyrtc_spec[] =
#endif
  {
    "implementation_id", "RTno2ProxyRTCTest",
    "type_name",         "RTno2ProxyRTCTest",
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
RTno2ProxyRTCTest::RTno2ProxyRTCTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_in0Out("in0", m_in0),
    m_out0In("out0", m_out0)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RTno2ProxyRTCTest::~RTno2ProxyRTCTest()
{
}



RTC::ReturnCode_t RTno2ProxyRTCTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("out0", m_out0In);
  
  // Set OutPort buffer
  addOutPort("in0", m_in0Out);
  
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
RTC::ReturnCode_t RTno2ProxyRTCTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/


//RTC::ReturnCode_t RTno2ProxyRTCTest::onStartup(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTCTest::onShutdown(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


RTC::ReturnCode_t RTno2ProxyRTCTest::onActivated(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTno2ProxyRTCTest::onDeactivated(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTno2ProxyRTCTest::onExecute(RTC::UniqueId /*ec_id*/)
{
  return RTC::RTC_OK;
}


//RTC::ReturnCode_t RTno2ProxyRTCTest::onAborting(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTCTest::onError(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTCTest::onReset(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTCTest::onStateUpdate(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


//RTC::ReturnCode_t RTno2ProxyRTCTest::onRateChanged(RTC::UniqueId /*ec_id*/)
//{
//  return RTC::RTC_OK;
//}


bool RTno2ProxyRTCTest::runTest()
{
    return true;
}


extern "C"
{
 
  void RTno2ProxyRTCTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(rtno2proxyrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<RTno2ProxyRTCTest>,
                             RTC::Delete<RTno2ProxyRTCTest>);
  }
  
}
