/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/udp/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/api.hpp"

using namespace std::literals;

namespace roq {
namespace python {

template <>
void utils::create_struct<roq::python::codec::udp::Header>(pybind11::module_ &module) {
  using value_type = roq::python::codec::udp::Header;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str()).def("__repr__", [](value_type const &value) {
    return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
  });
}
/*
template <>
void utils::create_struct<roq::python::codec::udp::Decoder>(pybind11::module_ &module) {
  using value_type = roq::python::codec::udp::Decoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          "dispatch",
          [](value_type &self,
             std::function<void(pybind11::object, pybind11::object)> &callback,
             pybind11::bytes message) { return self.dispatch(callback, message); },
          pybind11::arg("callback"),
          pybind11::arg("message"));
}
*/
}  // namespace python
}  // namespace roq
