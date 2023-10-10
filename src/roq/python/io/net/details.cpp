/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/io/net/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/io/net/reorder_buffer.hpp"

using namespace std::literals;

namespace roq {
namespace python {
/*
template <>
void utils::create_struct<roq::python::io::net::ReorderBuffer>(pybind11::module_ &module) {
  using value_type = roq::python::io::net::ReorderBuffer;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<>())
      // note! the callback signature **MUST** be pybind11::object so we can verify the reference count hasn't increased
      .def(
          "test", [](value_type &self, pybind11::bytes message) { return size_t{0}; }, pybind11::arg("message"));
}
*/
}  // namespace python
}  // namespace roq
