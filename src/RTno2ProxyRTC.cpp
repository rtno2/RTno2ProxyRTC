// -*- C++ -*-
// <rtc-template block="description">
/*!
 * @file  RTno2ProxyRTC.cpp
 * @brief RTno2Proxy
 *
 */
// </rtc-template>

#include "RTno2ProxyRTC.h"

ssr::SerialDevice *create_serial(const std::string &filename, const int int_arg)
{
  if (filename.substr(0, 6) == "tcp://")
  {
    try
    {
      return new ssr::EtherTcp(filename.substr(6).c_str(), int_arg);
    }
    catch (ssr::SocketException &se)
    {
      std::cerr << se.what() << std::endl;
      return nullptr;
    }
  }
  else
  {
    try
    {
      return new ssr::Serial(filename.c_str(), int_arg);
    }
    catch (net::ysuga::ComOpenException &ce)
    {
      std::cout << ce.what() << std::endl;
      return nullptr;
    }
  }
}

ssr::SerialDevice *create_serial_device_interactively(std::string port_name, int baudrate)
{
  std::string line_buffer;

  while (port_name.length() == 0)
  {
    // Interactive Mode for Serial port.
    std::cout << "Inpot COM port (ex., COM3, /dev/ttyUSB0, etc.) >" << std::ends;
    if (!std::getline(std::cin, line_buffer))
    {
      // Ctrl+D
      std::cerr << "EOF detected. Exit." << std::endl;
      return nullptr;
    }
    port_name = line_buffer;
  }

  while (baudrate < 0)
  {
    // Interactive Mode for Port argument
    if (port_name.length() > std::string("tcp://").length() && port_name.substr(0, 6) == "tcp://")
    {
      // TCP mode.
      std::cout << "TCPmode detected. Inpot port number. (ex., 9000, 12345, etc.) >" << std::ends;
    }
    else
    {
      std::cout << "Inpot baudrate (ex., 57600, 115200, etc.) >" << std::ends;
    }

    if (!std::getline(std::cin, line_buffer))
    {
      // Ctrl+D
      std::cerr << "EOF detected. Exit." << std::endl;
      return nullptr;
    }
    try
    {
      baudrate = std::stoi(line_buffer);
    }
    catch (std::exception const &e)
    {
      std::cerr << "Invalid value. Input number." << std::endl;
      continue;
    }
  }
  return create_serial(port_name, baudrate);
}

// Module specification
// <rtc-template block="module_spec">
#if RTM_MAJOR_VERSION >= 2
static const char *const rtno2proxyrtc_spec[] =
#else
static const char *rtno2proxyrtc_spec[] =
#endif
    {
        "implementation_id", "RTno2ProxyRTC",
        "type_name", "RTno2ProxyRTC",
        "description", "RTno2Proxy",
        "version", "1.0.0",
        "vendor", "sugarsweetrobotics",
        "category", "rtno",
        "activity_type", "PERIODIC",
        "kind", "DataFlowComponent",
        "max_instance", "1",
        "language", "C++",
        "lang_type", "compile",
        // Configuration variables
        "conf.default.port_name", "", // If port_name is blank (length==0), interactive mode standby
        "conf.default.baudrate", "57600",

        // Widget
        "conf.__widget__.port_name", "text",
        "conf.__widget__.baudrate", "text",
        // Constraints

        "conf.__type__.port_name", "string",
        "conf.__type__.baudrate", "int",

        ""};
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RTno2ProxyRTC::RTno2ProxyRTC(RTC::Manager *manager)
    // <rtc-template block="initializer">
    : RTC::DataFlowComponentBase(manager),
      m_in0In("in0", m_in0),
      m_out0Out("out0", m_out0),
      rtno2(nullptr),
      serial_device(nullptr),
    interface_container_(std::make_shared<interface_container_t>())
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
  // addInPort("in0", m_in0In);

  // Set OutPort buffer
  // addOutPort("out0", m_out0Out);

  // Set service provider to Ports

  // Set service consumers to Ports

  // Set CORBA Service Ports

  // </rtc-template>

  updateParameters("default");

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("port_name", m_port_name, "");
  bindParameter("baudrate", m_baudrate, "-1");
  // </rtc-template>

  this->serial_device = create_serial_device_interactively(m_port_name, m_baudrate);
  if (this->serial_device)
  {
      std::cout << "Serial Device Open Success." << std::endl;
      this->rtno2 = new ssr::rtno2::protocol_t(serial_device);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  std::cout << "Starting RTno....." << std::endl;
  const uint32_t wait_usec = 20 * 1000;
  const int try_count = 10;
  const auto prof = rtno2->get_profile(wait_usec, try_count);
  if (prof.result != ssr::rtno2::RESULT::OK)
  {
    std::cout << "ERROR in get_profile: " << ssr::rtno2::result_to_string(prof.result);
    return RTC::RTC_ERROR;
  }
  const auto state = rtno2->get_state(wait_usec, try_count);
  if (state.result != ssr::rtno2::RESULT::OK)
  {
    std::cout << "ERROR in get_state: " << ssr::rtno2::result_to_string(state.result);
    return RTC::RTC_ERROR;
  }
  const auto ec_type = rtno2->get_ec_type(wait_usec, try_count);
  if (ec_type.result != ssr::rtno2::RESULT::OK)
  {
    std::cout << "ERROR in get_ec_type: " << ssr::rtno2::result_to_string(ec_type.result);
    return RTC::RTC_ERROR;
  }

  return this->parse_rtno_infos(prof.value.value(), state.value.value(), ec_type.value.value());
}

RTC::ReturnCode_t RTno2ProxyRTC::parse_rtno_infos(const ssr::rtno2::profile_t &prof, const ssr::rtno2::STATE &state, const ssr::rtno2::EC_TYPE &ec_type)
{
  std::cout << "Parsing RTno profile...." << std::endl;
  if (parse_profile(interface_container_, prof, state, ec_type) != RTC::RTC_OK)
  {
    return RTC::RTC_ERROR;
  }

  for (auto port : interface_container_->inports_)
  {
    std::cout << "addInPort(" << port->name() << ")" << std::endl;
    addInPort(port->name().c_str(), port->get_port_base());
  }

  for (auto port : interface_container_->outports_)
  {
    std::cout << "addOutPort(" << port->name() << ")" << std::endl;
    addOutPort(port->name().c_str(), port->get_port_base());
  }

  return RTC::RTC_OK;
}

RTC::ReturnCode_t RTno2ProxyRTC::finalize_rtno_setup()
{
  for (auto port : interface_container_->inports_)
  {
    removeInPort(port->get_port_base());
  }

  for (auto port : interface_container_->outports_)
  {
    removeOutPort(port->get_port_base());
  }

  return finalize_profile(interface_container_);
}

RTC::ReturnCode_t RTno2ProxyRTC::onFinalize()
{
  finalize_rtno_setup();
  return RTC::RTC_OK;
}

// RTC::ReturnCode_t RTno2ProxyRTC::onStartup(RTC::UniqueId /*ec_id*/)
//{
//   return RTC::RTC_OK;
// }

// RTC::ReturnCode_t RTno2ProxyRTC::onShutdown(RTC::UniqueId /*ec_id*/)
//{
//   return RTC::RTC_OK;
// }

RTC::ReturnCode_t RTno2ProxyRTC::onActivated(RTC::UniqueId /*ec_id*/)
{
  static const uint32_t wait_usec = 20 * 1000;
  static const uint32_t try_count = 10;
  if (rtno2 == nullptr)
  {
    return RTC::RTC_ERROR;
  }
  if (rtno2->activate(wait_usec, try_count) != ssr::rtno2::RESULT::OK) {
    std::cout << "RTno2ProxyRTC : activate failed" << std::endl;
    return RTC::RTC_ERROR;
  }
  std::cout << "RTno2ProxyRTC activated" << std::endl;
  return RTC::RTC_OK;
}

RTC::ReturnCode_t RTno2ProxyRTC::onDeactivated(RTC::UniqueId /*ec_id*/)
{
  static const uint32_t wait_usec = 20 * 1000;
  static const uint32_t try_count = 10;
  if (rtno2 == nullptr)
  {
    return RTC::RTC_OK;
  }
  rtno2->deactivate(wait_usec, try_count);
  std::cout << "RTno2ProxyRTC deactivated" << std::endl;
  return RTC::RTC_OK;
}

RTC::ReturnCode_t RTno2ProxyRTC::onExecute(RTC::UniqueId /*ec_id*/)
{

  static const uint32_t wait_usec = 20 * 1000;
  static const uint32_t try_count = 10;
  if (this->rtno2->execute(wait_usec, try_count) != ssr::rtno2::RESULT::OK) {
    std::cout << "RTno2ProxyRTC execute failed" << std::endl;
    return RTC::RTC_ERROR;
  }
      
  for (auto port : interface_container_->inports_)
  {
    if (port->is_new())
    {
      uint8_t buffer[ssr::rtno2::MAX_PACKET_SIZE];
      size_t max_buffer_size = ssr::rtno2::MAX_PACKET_SIZE;
      size_t buffer_written_size;
      port->read();
      if (port->get_data(buffer, max_buffer_size, &buffer_written_size) == 0)
      {
        // Error
        return RTC::RTC_ERROR;
      }
      ssr::rtno2::RESULT result;
      if ((result = this->rtno2->send_inport_data(port->name(), buffer, buffer_written_size, wait_usec, try_count)) != ssr::rtno2::RESULT::OK)
      {
        return RTC::RTC_ERROR;
      }
    }
  }

  for (auto port : interface_container_->outports_)
  {
    uint8_t buffer[ssr::rtno2::MAX_PACKET_SIZE];
    const size_t max_buffer_size = ssr::rtno2::MAX_PACKET_SIZE;
    uint8_t buffer_written_size;
    ssr::rtno2::RESULT result;
    if ((result = this->rtno2->receive_outport_data(port->name(), buffer, max_buffer_size, &buffer_written_size, wait_usec, try_count)) != ssr::rtno2::RESULT::OK)
    {
      if (result == ssr::rtno2::RESULT::OUTPORT_BUFFER_EMPTY)
      {
        // Do nothing
        continue;
      }

    } else {
        if (port->write(buffer, buffer_written_size) == 0)
        {
            return RTC::RTC_ERROR;
        }

    }
  }
  return RTC::RTC_OK;
}

// RTC::ReturnCode_t RTno2ProxyRTC::onAborting(RTC::UniqueId /*ec_id*/)
//{
//   return RTC::RTC_OK;
// }

// RTC::ReturnCode_t RTno2ProxyRTC::onError(RTC::UniqueId /*ec_id*/)
//{
//   return RTC::RTC_OK;
// }

// RTC::ReturnCode_t RTno2ProxyRTC::onReset(RTC::UniqueId /*ec_id*/)
//{
//   return RTC::RTC_OK;
// }

// RTC::ReturnCode_t RTno2ProxyRTC::onStateUpdate(RTC::UniqueId /*ec_id*/)
//{
//   return RTC::RTC_OK;
// }

// RTC::ReturnCode_t RTno2ProxyRTC::onRateChanged(RTC::UniqueId /*ec_id*/)
//{
//   return RTC::RTC_OK;
// }

extern "C"
{

  void RTno2ProxyRTCInit(RTC::Manager *manager)
  {
    coil::Properties profile(rtno2proxyrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<RTno2ProxyRTC>,
                             RTC::Delete<RTno2ProxyRTC>);
  }
}
