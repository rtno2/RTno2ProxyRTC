#pragma once

#include "rtno2/profile.h"

#include "RTno2ProxyRTC.h"
#include "inport_wrapper.h"
#include "outport_wrapper.h"
#include "interface_container.h"

RTC::ReturnCode_t parse_profile(const std::shared_ptr<interface_container_t> &intfs, const ssr::rtno2::profile_t &profile, const ssr::rtno2::STATE &state, const ssr::rtno2::EC_TYPE &ec_type);
RTC::ReturnCode_t finalize_profile(const std::shared_ptr<interface_container_t> &intfs);