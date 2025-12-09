

#include "RTno2ProxyRTC/profile_parser.h"

#include <rtm/idl/BasicDataTypeSkel.h>

class inport_parse_failed_exception : public std::exception
{
public:
    inport_parse_failed_exception() {}
};

class outport_parse_failed_exception : public std::exception
{
public:
    outport_parse_failed_exception() {}
};

class inport_datatype_unknown_exception : public std::exception
{
public:
    inport_datatype_unknown_exception() {}
};

std::shared_ptr<inport_wrapper_base_t> _create_inport_wrapper(const ssr::rtno2::port_profile_t &inport)
{
    using namespace ssr::rtno2;
    switch (inport.typecode())
    {
    case TYPECODE::TIMED_BOOLEAN:
        return std::make_shared<inport_wrapper_t<RTC::TimedBoolean, bool>>(inport.name());
    case TYPECODE::TIMED_CHAR:
        return std::make_shared<inport_wrapper_t<RTC::TimedChar, int8_t>>(inport.name());
    case TYPECODE::TIMED_OCTET:
        return std::make_shared<inport_wrapper_t<RTC::TimedOctet, uint8_t>>(inport.name());
    case TYPECODE::TIMED_LONG:
        return std::make_shared<inport_wrapper_t<RTC::TimedLong, int32_t>>(inport.name());
    case TYPECODE::TIMED_FLOAT:
        return std::make_shared<inport_wrapper_t<RTC::TimedFloat, float>>(inport.name());
    case TYPECODE::TIMED_DOUBLE:
        return std::make_shared<inport_wrapper_t<RTC::TimedDouble, double>>(inport.name());
    case TYPECODE::TIMED_BOOLEAN_SEQ:
        return std::make_shared<seq_inport_wrapper_t<RTC::TimedBooleanSeq, bool>>(inport.name());
    case TYPECODE::TIMED_OCTET_SEQ:
        return std::make_shared<seq_inport_wrapper_t<RTC::TimedOctetSeq, uint8_t>>(inport.name());
    case TYPECODE::TIMED_CHAR_SEQ:
        return std::make_shared<seq_inport_wrapper_t<RTC::TimedCharSeq, int8_t>>(inport.name());
    case TYPECODE::TIMED_LONG_SEQ:
        return std::make_shared<seq_inport_wrapper_t<RTC::TimedLongSeq, int32_t>>(inport.name());
    case TYPECODE::TIMED_FLOAT_SEQ:
        return std::make_shared<seq_inport_wrapper_t<RTC::TimedFloatSeq, float>>(inport.name());
    case TYPECODE::TIMED_DOUBLE_SEQ:
        return std::make_shared<seq_inport_wrapper_t<RTC::TimedDoubleSeq, double>>(inport.name());
    default:
        throw inport_parse_failed_exception();
    }
}

std::shared_ptr<outport_wrapper_base_t> _create_outport_wrapper(const ssr::rtno2::port_profile_t &outport)
{
    using namespace ssr::rtno2;
    switch (outport.typecode())
    {
    case TYPECODE::TIMED_BOOLEAN:
        return std::make_shared<outport_wrapper_t<RTC::TimedBoolean, bool>>(outport.name());
    case TYPECODE::TIMED_CHAR:
        return std::make_shared<outport_wrapper_t<RTC::TimedChar, int8_t>>(outport.name());
    case TYPECODE::TIMED_OCTET:
        return std::make_shared<outport_wrapper_t<RTC::TimedOctet, uint8_t>>(outport.name());
    case TYPECODE::TIMED_LONG:
        return std::make_shared<outport_wrapper_t<RTC::TimedLong, int32_t>>(outport.name());
    case TYPECODE::TIMED_FLOAT:
        return std::make_shared<outport_wrapper_t<RTC::TimedFloat, float>>(outport.name());
    case TYPECODE::TIMED_DOUBLE:
        return std::make_shared<outport_wrapper_t<RTC::TimedDouble, double>>(outport.name());
    case TYPECODE::TIMED_BOOLEAN_SEQ:
        return std::make_shared<seq_outport_wrapper_t<RTC::TimedBooleanSeq, bool>>(outport.name());
    case TYPECODE::TIMED_OCTET_SEQ:
        return std::make_shared<seq_outport_wrapper_t<RTC::TimedOctetSeq, uint8_t>>(outport.name());
    case TYPECODE::TIMED_CHAR_SEQ:
        return std::make_shared<seq_outport_wrapper_t<RTC::TimedCharSeq, int8_t>>(outport.name());
    case TYPECODE::TIMED_LONG_SEQ:
        return std::make_shared<seq_outport_wrapper_t<RTC::TimedLongSeq, int32_t>>(outport.name());
    case TYPECODE::TIMED_FLOAT_SEQ:
        return std::make_shared<seq_outport_wrapper_t<RTC::TimedFloatSeq, float>>(outport.name());
    case TYPECODE::TIMED_DOUBLE_SEQ:
        return std::make_shared<seq_outport_wrapper_t<RTC::TimedDoubleSeq, double>>(outport.name());
    default:
        throw outport_parse_failed_exception();
    }
}

std::shared_ptr<inport_wrapper_base_t> _into_inport_wrapper(const ssr::rtno2::port_profile_t &inport)
{
    return _create_inport_wrapper(inport);
}

std::shared_ptr<outport_wrapper_base_t> _into_outport_wrapper(const ssr::rtno2::port_profile_t &outport)
{
    return _create_outport_wrapper(outport);
}

RTC::ReturnCode_t _parse_inport(const std::shared_ptr<interface_container_t> &interface_container, const ssr::rtno2::port_profile_t &inport)
{
    try
    {
        interface_container->inports_.push_back(_into_inport_wrapper(inport));
    }
    catch (inport_parse_failed_exception &ex)
    {
        return RTC::RTC_ERROR;
    }
    return RTC::RTC_OK;
}

RTC::ReturnCode_t _parse_outport(const std::shared_ptr<interface_container_t> &interface_container, const ssr::rtno2::port_profile_t &outport)
{
    try
    {
        interface_container->outports_.emplace_back(_into_outport_wrapper(outport));
    }
    catch (outport_parse_failed_exception &ex)
    {
        return RTC::RTC_ERROR;
    }
    return RTC::RTC_OK;
}

RTC::ReturnCode_t parse_profile(const std::shared_ptr<interface_container_t> &interface_container, const ssr::rtno2::profile_t &profile, const ssr::rtno2::STATE &state, const ssr::rtno2::EC_TYPE &ec_type)
{
    for (auto inport_profile : profile.inports_)
    {
        if (_parse_inport(interface_container, inport_profile) != RTC::RTC_OK)
        {
            return RTC::RTC_ERROR;
        }
    }

    for (auto outport_profile : profile.outports_)
    {
        if (_parse_outport(interface_container, outport_profile) != RTC::RTC_OK)
        {
            return RTC::RTC_ERROR;
        }
    }
    return RTC::RTC_OK;
}

RTC::ReturnCode_t finalize_profile(const std::shared_ptr<interface_container_t> &interface_container)
{
    interface_container->inports_.clear();
    interface_container->outports_.clear();
    return RTC::RTC_OK;
}