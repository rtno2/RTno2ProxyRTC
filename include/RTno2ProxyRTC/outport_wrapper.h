#pragma once

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

#include <stdint.h>
#include <string>
#include <memory>

#include "rtno2/profile.h"

class outport_wrapper_base_t
{
protected:
  const std::string name_;
  const std::string typename_;
  const std::shared_ptr<RTC::OutPortBase> port_;
  ssr::rtno2::TYPECODE typecode_;
  const bool is_avr_;

public:
  RTC::OutPortBase &get_port_base() { return *port_; }

  operator RTC::OutPortBase &() { return *port_; }

  std::string name() { return name_; }
  // std::string typename() { return typename_; }
  ssr::rtno2::TYPECODE typecode() { return typecode_; }

  bool is_avr() const { return is_avr_; }

public:
  outport_wrapper_base_t(const std::string &name, const std::string &type_name, std::shared_ptr<RTC::OutPortBase> &&port, bool is_avr) : name_(name), typename_(type_name), port_(std::move(port)), is_avr_(is_avr) {}
  virtual ~outport_wrapper_base_t() {}

public:
  template <typename DataType>
  RTC::OutPort<DataType> *get_port()
  {
    return std::dynamic_pointer_cast<RTC::OutPort<DataType>>(port_).get();
  }

  template <typename DataType>
  const RTC::OutPort<DataType> *get_port() const
  {
    return std::static_pointer_cast<RTC::OutPort<DataType>>(port_).get();
  }

  virtual int32_t write(void *buffer, size_t buffer_size) = 0;
  virtual size_t get_length(void) const = 0;
  virtual size_t get_type_size(void) const = 0;
};

template <class DataType, typename VType>
class outport_wrapper_t : public outport_wrapper_base_t
{
private:
  DataType value_;

public:
  outport_wrapper_t(const std::string &name, const bool is_avr = false) : outport_wrapper_base_t(name, CORBA_Util::toTypename<DataType>(), std::make_shared<RTC::OutPort<DataType>>(name.c_str(), value_), is_avr)
  {
  }

  virtual ~outport_wrapper_t()
  {
  }

public:
  virtual size_t get_length(void) const override { return 1; }
  virtual size_t get_type_size(void) const override { return sizeof(VType); }

  virtual int32_t write(void *buffer, size_t buffer_size) override
  {
    return _write_raw_data((VType *)buffer, buffer_size);
  }

private:
  int32_t _write_raw_data(VType *buffer, uint32_t buffer_size)
  {
    if (is_avr() && typename_ == "TimedDouble")
    {
      if (buffer_size != 4)
      {
        return 0;
      }
      value_.data = (double)*((float *)buffer);
    }
    else
    {
      if (buffer_size != sizeof(VType))
      {
        return 0;
      }
      value_.data = *buffer;
    }
    return get_port<DataType>()->write();
  }
};

template <class DataType, typename VType>
class seq_outport_wrapper_t : public outport_wrapper_base_t
{
private:
  DataType value_;

public:
  seq_outport_wrapper_t(const std::string &name, const bool is_avr = false) : outport_wrapper_base_t(name, CORBA_Util::toTypename<DataType>(), std::make_shared<RTC::OutPort<DataType>>(name.c_str(), value_), is_avr)
  {
  }

  virtual ~seq_outport_wrapper_t()
  {
  }

public:
  virtual size_t get_length(void) const override { return value_.data.length(); }
  virtual size_t get_type_size(void) const override { return sizeof(VType); }

  virtual int32_t write(void *buffer, size_t buffer_size) override
  {
    return _write_raw_data((VType *)buffer, buffer_size);
  }

private:
  int32_t _write_raw_data(VType *value, size_t buffer_size)
  {
    if (is_avr() && typename_ == "TimedDoubleSeq")
    {
      size_t length = buffer_size / 4;
      value_.data.length(length);
      float *pData = (float *)value;
      for (size_t i = 0; i < length; i++)
      {
        value_.data[i] = (double)pData[i];
      }
    }
    else
    {
      size_t length = buffer_size / sizeof(VType);
      memcpy(&(value_.data[0]), value, length * sizeof(VType));
    }
    return get_port<DataType>()->write();
  }
};

#if 0

class OutPortWrapperBase {
protected:
  std::string m_TypeCode;
  RTC::OutPortBase* m_pPort;
  std::string m_Name;
 public:
  RTC::OutPortBase& GetPort() { return *m_pPort; }

  operator RTC::OutPortBase&(){return *m_pPort;}

  std::string& GetName() {return m_Name;}
  std::string& GetTypeCode() {return m_TypeCode;}
 public:
  OutPortWrapperBase(const std::string& name){m_Name = name;}
  virtual ~OutPortWrapperBase(){}

 public:
  virtual int32_t getTypesize() = 0;
  virtual int32_t getTypeSizeInArduino() = 0;
  virtual int32_t Write(void* data, uint32_t size) = 0;
};


template<class DataType, typename VType>
class OutPortWrapper : public OutPortWrapperBase {
 private:
  DataType m_Value;

 public:
 OutPortWrapper(const std::string& name) : OutPortWrapperBase(name) {
    //m_TypeCode = toTypename<DataType>();
    m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pPort  = new RTC::OutPort<DataType>(name.c_str(), m_Value);
  }

  virtual ~OutPortWrapper() {
    delete m_pPort;
  }

 public:

  int32_t getTypesize() {return sizeof(VType);}
  int32_t getTypeSizeInArduino() {
    if(m_TypeCode == "TimedDouble") {
      return 4;
    } else {
      return getTypesize();
    }
  }

  int32_t Write(void* data, uint32_t size) {
    VType *buf = (VType*)data;
    return Write(buf, size);
  }

 private:
  int32_t Write(VType* data, uint32_t size)
  {
    if(size != 1) return -1;
    if(m_TypeCode == "TimedDouble") {
      m_Value.data = *((float*)data);
    }else {
      m_Value.data = *data;
    }
    return ((RTC::OutPort<DataType>*)m_pPort)->write();
  }
};

template<class DataType, class VType>
class SeqOutPortWrapper : public OutPortWrapperBase {
private:
  DataType m_Value;

 public:
 SeqOutPortWrapper(const std::string& name) : OutPortWrapperBase(name) {
    //m_TypeCode = toTypename<DataType>();
    m_TypeCode = CORBA_Util::toTypename<DataType>();
    m_pPort  = new RTC::OutPort<DataType>(name.c_str(), m_Value);
  }

  virtual ~SeqOutPortWrapper() {
    delete m_pPort;
  }

 public:
  int32_t getTypesize() {return sizeof(VType);}
  int32_t getTypeSizeInArduino() {
    if(m_TypeCode == "TimedDoubleSeq") {
      return 4;
    } else {
      return getTypesize();
    }
  }

  int32_t Write(void* data, uint32_t size) {
    VType* buf = (VType*)data;
    return Write(buf, size);
  }

 private:
  int32_t Write(VType* value, uint32_t size) {
    m_Value.data.length(size);
    if(m_TypeCode == "TimedDoubleSeq") {
      float* pData = (float*)value;
      for(unsigned int i = 0;i < size;i++) {
	m_Value.data[i] = pData[i];
      }
    } else {
      memcpy(&(m_Value.data[0]), value, size*sizeof(VType));
      //for(unsigned int i = 0;i < m_Value.data.length();i++) {
      //}
    }
    return ((RTC::OutPort<DataType>*)m_pPort)->write();
  }
};

#endif