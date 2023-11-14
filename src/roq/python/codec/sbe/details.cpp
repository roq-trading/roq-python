/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/sbe/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/api.hpp"

#include "roq/python/codec/sbe/decoder.hpp"

using namespace std::literals;

namespace roq {
namespace python {

template <>
void utils::create_struct<roq::python::codec::sbe::Encoder>(pybind11::module_ &module) {
  using value_type = roq::python::codec::sbe::Encoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<>())
      .def(
          "encode",
          [](value_type &self, utils::Ref<MessageInfo> &message_info, utils::Ref<ReferenceData> &reference_data) {
            auto message = self(static_cast<MessageInfo>(message_info), static_cast<ReferenceData>(reference_data));
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("message_info"),
          pybind11::arg("reference_data"))
      .def(
          "encode",
          [](value_type &self, utils::Ref<MessageInfo> &message_info, utils::Ref<MarketStatus> &market_status) {
            auto message = self(static_cast<MessageInfo>(message_info), static_cast<MarketStatus>(market_status));
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("message_info"),
          pybind11::arg("market_status"))
      .def(
          "encode",
          [](value_type &self, utils::Ref<MessageInfo> &message_info, utils::Ref<TopOfBook> &top_of_book) {
            auto message = self(static_cast<MessageInfo>(message_info), static_cast<TopOfBook>(top_of_book));
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("message_info"),
          pybind11::arg("top_of_book"))
      .def(
          "encode",
          [](value_type &self,
             utils::Ref<MessageInfo> &message_info,
             utils::Ref<MarketByPriceUpdate> &market_by_price_update) {
            auto message =
                self(static_cast<MessageInfo>(message_info), static_cast<MarketByPriceUpdate>(market_by_price_update));
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("message_info"),
          pybind11::arg("market_by_price_update"))
      .def(
          "encode",
          [](value_type &self,
             utils::Ref<MessageInfo> &message_info,
             utils::Ref<MarketByOrderUpdate> &market_by_order_update) {
            auto message =
                self(static_cast<MessageInfo>(message_info), static_cast<MarketByOrderUpdate>(market_by_order_update));
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("message_info"),
          pybind11::arg("market_by_order_update"))
      .def(
          "encode",
          [](value_type &self, utils::Ref<MessageInfo> &message_info, utils::Ref<TradeSummary> &trade_summary) {
            auto message = self(static_cast<MessageInfo>(message_info), static_cast<TradeSummary>(trade_summary));
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("message_info"),
          pybind11::arg("trade_summary"))
      .def(
          "encode",
          [](value_type &self, utils::Ref<MessageInfo> &message_info, utils::Ref<StatisticsUpdate> &statistic_update) {
            auto message =
                self(static_cast<MessageInfo>(message_info), static_cast<StatisticsUpdate>(statistic_update));
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("message_info"),
          pybind11::arg("statistic_update"));
}

template <>
void utils::create_struct<roq::python::codec::sbe::Decoder>(pybind11::module_ &module) {
  using value_type = roq::python::codec::sbe::Decoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<>())
      // note! the callback signature **MUST** be pybind11::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &self,
             std::function<void(pybind11::object, pybind11::object)> &callback,
             pybind11::bytes message) { return self.dispatch(callback, message); },
          pybind11::arg("callback"),
          pybind11::arg("message"));
}

}  // namespace python
}  // namespace roq
