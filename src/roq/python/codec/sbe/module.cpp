/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/sbe/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/codec/sbe/decoder.hpp"
#include "roq/python/codec/sbe/encoder.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace codec {
namespace sbe {

void Module::create(pybind11::module_ &module) {
  utils::create_struct<roq::python::codec::sbe::Decoder>(module);
  utils::create_struct<roq::python::codec::sbe::Encoder>(module);
}

}  // namespace sbe
}  // namespace codec
}  // namespace python
}  // namespace roq
