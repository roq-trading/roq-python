/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/utils/mbp/details.hpp"

#include <pybind11/pybind11.h>

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace roq {
namespace python {

template <>
void utils::create_struct<utils::mbp::MarketByPrice>(pybind11::module_ &module) {
  using value_type = utils::mbp::MarketByPrice;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<std::string_view const &, std::string_view const &>(),
          pybind11::arg("exchange") = "",
          pybind11::arg("symbol"))
      .def(
          "apply",
          [](value_type &value, utils::Ref<MarketByPriceUpdate> const &market_by_price_update) {
            value(market_by_price_update);
          })
      .def("extract", [](value_type const &value, size_t depth) { return value.extract(depth); })
      .def("__repr__", [](value_type const &value) { return value.print(); });
}

}  // namespace python
}  // namespace roq
