/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/logging/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/logging/details.hpp"

#include "roq/logging/level.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace logging {

void Module::create(pybind11::module_ &module) {
  utils::create_enum<roq::logging::Level>(module);

  module.def("set_handler", &roq::python::logging::set_handler, "WIP", pybind11::arg("callback"));
}

}  // namespace logging
}  // namespace python
}  // namespace roq
