/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/client/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

using namespace std::literals;

namespace roq {
namespace python {

template <>
void utils::create_struct<roq::client::Settings>(pybind11::module_ &module) {
  using value_type = roq::client::Settings;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<OrderCancelPolicy, OrderManagement>(),
          pybind11::arg("order_cancel_policy") = OrderCancelPolicy::UNDEFINED,
          pybind11::arg("order_management") = OrderManagement::UNDEFINED)
      .def_readonly("order_cancel_policy", &value_type::order_cancel_policy)
      .def_readonly("order_management", &value_type::order_management)
      .def("__repr__", [](value_type const &value) { return fmt::format("{}", value); });
}

template <>
void utils::create_struct<client::Config>(pybind11::module_ &module) {
  using value_type = client::Config;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<roq::client::Settings, std::set<std::string>, std::map<std::string, std::set<std::string>>>(),
          pybind11::arg("settings") = roq::client::Settings(),
          pybind11::arg("accounts") = std::set<std::string>(),
          pybind11::arg("symbols"));
}

template <>
void utils::create_struct<client::Dispatcher>(pybind11::module_ &module) {
  using value_type = client::Dispatcher;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<pybind11::object, python::client::Config const &, std::vector<std::string> const &>(),
          pybind11::arg("handler"),
          pybind11::arg("config"),
          pybind11::arg("connections"))
      .def("start", [](value_type &self) { return self.start(); })
      .def("stop", [](value_type &self) { return self.stop(); })
      .def("dispatch", [](value_type &self) { return self.dispatch(); })
      .def(
          "create_order",
          [](value_type &self,
             std::string_view const &account,
             uint32_t order_id,
             std::string_view const &exchange,
             std::string_view const &symbol,
             roq::Side side,
             roq::PositionEffect position_effect,
             roq::MarginMode margin_mode,
             double max_show_quantity,
             roq::OrderType order_type,
             roq::TimeInForce time_in_force,
             roq::Mask<roq::ExecutionInstruction> const &execution_instructions,
             std::string_view const &request_template,
             double quantity,
             double price,
             double stop_price,
             std::string_view const &routing_id,
             uint32_t strategy_id,
             uint8_t source) {
            auto create_order = roq::CreateOrder{
                .account = account,
                .order_id = order_id,
                .exchange = exchange,
                .symbol = symbol,
                .side = side,
                .position_effect = position_effect,
                .margin_mode = margin_mode,
                .max_show_quantity = max_show_quantity,
                .order_type = order_type,
                .time_in_force = time_in_force,
                .execution_instructions = execution_instructions,
                .request_template = request_template,
                .quantity = quantity,
                .price = price,
                .stop_price = stop_price,
                .routing_id = routing_id,
                .strategy_id = strategy_id,
            };
            self.send(create_order, source);
          },
          pybind11::arg("account"),
          pybind11::arg("order_id"),
          pybind11::arg("exchange"),
          pybind11::arg("symbol"),
          pybind11::arg("side"),
          pybind11::arg("position_effect") = roq::PositionEffect::UNDEFINED,
          pybind11::arg("margin_mode") = roq::MarginMode::UNDEFINED,
          pybind11::arg("max_show_quantity") = NaN,
          pybind11::arg("order_type"),
          pybind11::arg("time_in_force") = roq::TimeInForce::UNDEFINED,
          pybind11::arg("execution_instructions"),  // = roq::Mask<roq::ExecutionInstruction>{},
          pybind11::arg("request_template") = "",
          pybind11::arg("quantity"),
          pybind11::arg("price") = NaN,
          pybind11::arg("stop_price") = NaN,
          pybind11::arg("routing_id") = "",
          pybind11::arg("strategy_id") = 0,
          pybind11::arg("source"))
      .def(
          "modify_order",
          [](value_type &self,
             std::string_view const &account,
             uint32_t order_id,
             std::string_view const &request_template,
             double quantity,
             double price,
             std::string_view const &routing_id,
             uint32_t version,
             uint32_t conditional_on_version,
             uint8_t source) {
            auto modify_order = roq::ModifyOrder{
                .account = account,
                .order_id = order_id,
                .request_template = request_template,
                .quantity = quantity,
                .price = price,
                .routing_id = routing_id,
                .version = version,
                .conditional_on_version = conditional_on_version,
            };
            self.send(modify_order, source);
          },
          pybind11::arg("account"),
          pybind11::arg("order_id"),
          pybind11::arg("request_template") = "",
          pybind11::arg("quantity") = NaN,
          pybind11::arg("price") = NaN,
          pybind11::arg("routing_id") = "",
          pybind11::arg("version") = 0,
          pybind11::arg("conditional_on_version") = 0,
          pybind11::arg("source"))
      .def(
          "cancel_order",
          [](value_type &self,
             std::string_view const &account,
             uint32_t order_id,
             std::string_view const &request_template,
             std::string_view const &routing_id,
             uint32_t version,
             uint32_t conditional_on_version,
             uint8_t source) {
            auto cancel_order = roq::CancelOrder{
                .account = account,
                .order_id = order_id,
                .request_template = request_template,
                .routing_id = routing_id,
                .version = version,
                .conditional_on_version = conditional_on_version,
            };
            self.send(cancel_order, source);
          },
          pybind11::arg("account"),
          pybind11::arg("order_id"),
          pybind11::arg("request_template") = "",
          pybind11::arg("routing_id") = "",
          pybind11::arg("version") = 0,
          pybind11::arg("conditional_on_version") = 0,
          pybind11::arg("source"))
      .def(
          "cancel_all_orders",
          [](value_type &self, std::string_view const &account, uint8_t source) {
            auto cancel_all_orders = roq::CancelAllOrders{
                .account = account,
                .order_id = {},
                .exchange = {},
                .symbol = {},
                .strategy_id = {},
                .side = {},
            };
            self.send(cancel_all_orders, source);
          },
          pybind11::arg("account") = "",
          pybind11::arg("source"));
}

template <>
void utils::create_struct<client::EventLogReader>(pybind11::module_ &module) {
  using value_type = client::EventLogReader;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<std::string_view const &>(), pybind11::arg("path"))
      // note! the callback signature **MUST** be pybind11::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &self, std::function<void(pybind11::object, pybind11::object)> &callback) {
            return self.dispatch(callback);
          },
          pybind11::arg("callback"));
}

template <>
void utils::create_struct<client::EventLogMultiplexer>(pybind11::module_ &module) {
  using value_type = client::EventLogMultiplexer;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<std::vector<std::string_view> const &>(), pybind11::arg("paths"))
      // note! the callback signature **MUST** be pybind11::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &self, std::function<void(pybind11::object, pybind11::object)> &callback) {
            return self.dispatch(callback);
          },
          pybind11::arg("callback"));
}

}  // namespace python
}  // namespace roq
