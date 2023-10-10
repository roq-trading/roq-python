/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/udp/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/codec/udp/details.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace codec {
namespace udp {

void Module::create(pybind11::module_ &module) {
  utils::create_enum<roq::codec::udp::Encoding>(module);
  utils::create_enum<roq::codec::udp::Channel>(module);

  utils::create_struct<roq::python::codec::udp::Header>(module);
}

}  // namespace udp
}  // namespace codec
}  // namespace python
}  // namespace roq
