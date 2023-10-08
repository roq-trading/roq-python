/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/sbe/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/sbe/encoder.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace sbe {

void Module::create(pybind11::module_ &module) {
  utils::create_struct<roq::python::sbe::Encoder>(module);
}

}  // namespace sbe
}  // namespace python
}  // namespace roq
