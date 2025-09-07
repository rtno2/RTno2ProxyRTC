#pragma once

#include <vector>
#include <string>
#include "inport_wrapper.h"
#include "outport_wrapper.h"

struct interface_container_t
{
    std::vector<std::shared_ptr<inport_wrapper_base_t>> inports_;
    std::vector<std::shared_ptr<outport_wrapper_base_t>> outports_;
};
