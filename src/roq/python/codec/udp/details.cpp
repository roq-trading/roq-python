/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/udp/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/codec/udp/decoder.hpp"
#include "roq/codec/udp/header.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace codec {
namespace udp {
Header::Header(pybind11::bytes message) {
  auto tmp = static_cast<std::string_view>(message);
  std::span buffer{reinterpret_cast<std::byte const *>(std::data(tmp)), std::size(tmp)};
  roq::codec::udp::Decoder::decode(value_, buffer);
}
}  // namespace udp
}  // namespace codec

template <>
void utils::create_struct<roq::python::codec::udp::Header>(pybind11::module_ &module) {
  using value_type = roq::python::codec::udp::Header;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<pybind11::bytes>(), pybind11::arg("message"))
      .def_static("sizeof", []() { return sizeof(value_type::value_type); })
      .def_static(
          "get_sequence_number",
          [](pybind11::bytes message) {
            auto tmp = static_cast<std::string_view>(message);
            std::span buffer{reinterpret_cast<std::byte const *>(std::data(tmp)), std::size(tmp)};
            return roq::codec::udp::Decoder::get_sequence_number(buffer);
          },
          pybind11::arg("message"))
      .def_property_readonly(
          "control", [](value_type const &value) { return static_cast<value_type::value_type const &>(value).control; })
      .def_property_readonly(
          "object_type", [](value_type const &value) { return static_cast<value_type::value_type>(value).object_type; })
      .def_property_readonly(
          "session_id", [](value_type const &value) { return static_cast<value_type::value_type>(value).session_id; })
      .def_property_readonly(
          "sequence_number",
          [](value_type const &value) { return static_cast<value_type::value_type>(value).sequence_number; })
      .def_property_readonly(
          "fragment", [](value_type const &value) { return static_cast<value_type::value_type>(value).fragment; })
      .def_property_readonly(
          "fragment_max",
          [](value_type const &value) { return static_cast<value_type::value_type>(value).fragment_max; })
      .def_property_readonly(
          "object_id", [](value_type const &value) { return static_cast<value_type::value_type>(value).object_id; })
      .def_property_readonly(
          "last_sequence_number",
          [](value_type const &value) { return static_cast<value_type::value_type>(value).last_sequence_number; })
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

}  // namespace python
}  // namespace roq
