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

class inport_wrapper_base_t
{
protected:
    const std::string name_;
    const std::string typename_;
    const std::shared_ptr<RTC::InPortBase> port_;
    ssr::rtno2::TYPECODE typecode_;
    const bool is_avr_;

public:
    RTC::InPortBase &get_port_base() { return *port_; }

    operator RTC::InPortBase &() { return *port_; }

    std::string name() { return name_; }
    // std::string typename() { return typename_; }
    ssr::rtno2::TYPECODE typecode() { return typecode_; }

    bool is_avr() const { return is_avr_; }

public:
    // inport_wrapper_base(const std::string &name) : name_(name), typecode_() {}
    inport_wrapper_base_t(const std::string &name, const std::string &type_name, std::shared_ptr<RTC::InPortBase> &&port, bool is_avr) : name_(name), typename_(type_name), port_(std::move(port)), is_avr_(is_avr) {}
    virtual ~inport_wrapper_base_t() {}

public:
    template <typename DataType>
    RTC::InPort<DataType> *get_port()
    {
        return std::dynamic_pointer_cast<RTC::InPort<DataType>>(port_).get();
    }

    template <typename DataType>
    const RTC::InPort<DataType> *get_port() const
    {
        return std::static_pointer_cast<RTC::InPort<DataType>>(port_).get();
    }

    virtual bool is_new(void) = 0;
    virtual int32_t read(void) = 0;
    virtual size_t get_length(void) const = 0;
    virtual size_t get_type_size(void) const = 0;
    virtual size_t get_data(void *buffer, size_t max_buffer_size, size_t *read_size) = 0;
};

template <class DataType, typename VType>
class inport_wrapper_t : public inport_wrapper_base_t
{
private:
    DataType value_;

public:
    inport_wrapper_t(const std::string &name, const bool is_avr = false) : inport_wrapper_base_t(name, CORBA_Util::toTypename<DataType>(), std::make_shared<RTC::InPort<DataType>>(name.c_str(), value_), is_avr)
    {
    }

    virtual ~inport_wrapper_t()
    {
    }

public:
    virtual bool is_new(void) override { return get_port<DataType>()->isNew(); }
    virtual int32_t read(void) override { return get_port<DataType>()->read(); }
    virtual size_t get_length(void) const override { return 1; }
    virtual size_t get_type_size(void) const override { return sizeof(VType); }
    virtual size_t get_data(void *buffer, size_t max_buffer_size, size_t *read_size) override
    {
        return _get_raw_data((VType *)buffer, max_buffer_size, read_size);
    }

private:
    size_t _get_raw_data(VType *dst, size_t max_buffer_size, size_t *read_size)
    {
        if (is_avr() && typename_ == "TimedDouble")
        {
            if (max_buffer_size < 4)
            {
                return 0;
            }
            *((float *)dst) = value_.data;
            *read_size = 4;
        }
        else
        {
            if (max_buffer_size < sizeof(VType))
            {
                return 0;
            }
            *dst = value_.data;
            *read_size = sizeof(VType);
        }
        return sizeof(VType);
    }
};

template <class DataType, typename VType>
class seq_inport_wrapper_t : public inport_wrapper_base_t
{
private:
    DataType value_;

public:
    seq_inport_wrapper_t(const std::string &name, const bool is_avr = false) : inport_wrapper_base_t(name, CORBA_Util::toTypename<DataType>(), std::make_shared<RTC::InPort<DataType>>(name.c_str(), value_), is_avr)
    {
    }

    virtual ~seq_inport_wrapper_t()
    {
    }

public:
    virtual bool is_new(void) override { return get_port<DataType>()->isNew(); }
    virtual int32_t read(void) override { return get_port<DataType>()->read(); }
    virtual size_t get_length(void) const override { return value_.data.length(); }
    virtual size_t get_type_size(void) const override { return sizeof(VType); }
    virtual size_t get_data(void *buffer, size_t max_buffer_size, size_t *read_size) override
    {
        return _get_raw_data((VType *)buffer, max_buffer_size, read_size);
    }

private:
    size_t _get_raw_data(VType *dst, size_t max_buffer_size, size_t *read_size)
    {
        if (is_avr() && typename_ == "TimedDouble")
        {
            if (max_buffer_size < 4 * value_.data.length())
            {
                return 0;
            }
            float *pDst = (float *)dst;
            for (size_t i = 0; i < value_.data.length(); i++)
            {
                pDst[i] = value_.data[i];
            }
            *read_size = 4 * value_.data.length();
        }
        else
        {

            if (max_buffer_size < sizeof(VType) * value_.data.length())
            {
                return 0;
            }
            for (size_t i = 0; i < value_.data.length(); i++)
            {
                dst[i] = value_.data[i];
            }
            *read_size = sizeof(VType) * value_.data.length();
        }
        return sizeof(VType);
    }
};