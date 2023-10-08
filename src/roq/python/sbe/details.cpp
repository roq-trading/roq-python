/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/sbe/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/api.hpp"

using namespace std::literals;

namespace roq {
namespace python {

template <>
void utils::create_struct<roq::python::sbe::Encoder>(pybind11::module_ &module) {
  using value_type = roq::python::sbe::Encoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<>())
      .def(
          "encode",
          [](value_type &self, utils::Ref<ReferenceData> &reference_data) {
            MessageInfo message_info;                                               // XXX HANS
            Event event{message_info, static_cast<ReferenceData>(reference_data)};  // XXX HANS
            auto message = self.encode(event);
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("reference_data"));
}

}  // namespace python
}  // namespace roq
