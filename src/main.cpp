/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <pybind11/pybind11.h>

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <magic_enum.hpp>

#include "roq/client.hpp"

#include "roq/logging.hpp"

using namespace std::literals;         // NOLINT
using namespace std::chrono_literals;  // NOLINT

namespace py = pybind11;  // convention

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace roq {
namespace python {
namespace utils {
auto to_list(const auto &values) {
  using value_type = typename std::remove_cvref<decltype(values)>::type::value_type;
  py::list result;
  for (auto &item : values)
    result.append(value_type{item});
  return result;
}
// note! this is a work-around until IntFlag (?) can be supported properly
template <typename T>
auto to_int_flag(const Mask<T> &value) {
  return static_cast<T>(value.get());
}
template <typename T>
void create_enum(auto &context, const std::string &name) {
  pybind11::enum_<T> enum_(context, name.c_str());
  for (auto item : magic_enum::enum_values<T>()) {
    std::string name{magic_enum::enum_name(item)};
    enum_.value(name.c_str(), item);
  }
}
template <typename T>
struct Ref final {
  Ref() = delete;  // not allowed
  explicit Ref(const T &value) : value(value) {}

  operator const T &() const { return value; }

 private:
  const T &value;
};
}  // namespace utils
// helpers
// note! they all use copies
void create_fill(auto &context, const std::string &name) {
  using value_type = roq::Fill;
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("external_trade_id", [](const value_type &value) { return value.external_trade_id; })
      .def_property_readonly("quantity", [](const value_type &value) { return value.quantity; })
      .def_property_readonly("price", [](const value_type &value) { return value.price; })
      .def_property_readonly("liquidity", [](const value_type &value) { return value.liquidity; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
void create_layer(auto &context, const std::string &name) {
  using value_type = roq::Layer;
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("bid_price", [](const value_type &value) { return value.bid_price; })
      .def_property_readonly("bid_quantity", [](const value_type &value) { return value.bid_quantity; })
      .def_property_readonly("ask_price", [](const value_type &value) { return value.ask_price; })
      .def_property_readonly("ask_quantity", [](const value_type &value) { return value.ask_quantity; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
void create_mbp_update(auto &context, const std::string &name) {
  using value_type = roq::MBPUpdate;
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("price", [](const value_type &value) { return value.price; })
      .def_property_readonly("quantity", [](const value_type &value) { return value.quantity; })
      .def_property_readonly("implied_quantity", [](const value_type &value) { return value.implied_quantity; })
      .def_property_readonly("price_level", [](const value_type &value) { return value.price_level; })
      .def_property_readonly("number_of_orders", [](const value_type &value) { return value.number_of_orders; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
void create_measurement(auto &context, const std::string &name) {
  using value_type = roq::Measurement;
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("name", [](const value_type &value) { return value.name; })
      .def_property_readonly("value", [](const value_type &value) { return value.value; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
void create_statistics(auto &context, const std::string &name) {
  using value_type = roq::Statistics;
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("type", [](const value_type &value) { return value.type; })
      .def_property_readonly("value", [](const value_type &value) { return value.value; })
      .def_property_readonly("begin_time_utc", [](const value_type &value) { return value.begin_time_utc; })
      .def_property_readonly("end_time_utc", [](const value_type &value) { return value.end_time_utc; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
void create_trade(auto &context, const std::string &name) {
  using value_type = roq::Trade;
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("side", [](const value_type &value) { return value.side; })
      .def_property_readonly("price", [](const value_type &value) { return value.price; })
      .def_property_readonly("quantity", [](const value_type &value) { return value.quantity; })
      .def_property_readonly("trade_id", [](const value_type &value) { return value.trade_id; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
void create_uuid(auto &context, const std::string &name) {
  using value_type = roq::UUID;
  py::class_<value_type>(context, name.c_str()).def("__repr__", [](const value_type &obj) {
    return fmt::format("{}"sv, obj);
  });
}
// transport
void create_message_info(auto &context, const std::string &name) {
  using value_type = roq::MessageInfo;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "source",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source;
          })
      .def_property_readonly(
          "source_name",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_name;
          })
      .def_property_readonly(
          "source_session_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_session_id;
          })
      .def_property_readonly(
          "source_seqno",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_seqno;
          })
      .def_property_readonly(
          "receive_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.receive_time_utc;
          })
      .def_property_readonly(
          "receive_time",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.receive_time;
          })
      .def_property_readonly(
          "source_send_time",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_send_time;
          })
      .def_property_readonly(
          "source_receive_time",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_receive_time;
          })
      .def_property_readonly(
          "origin_create_time",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin_create_time;
          })
      .def_property_readonly(
          "origin_create_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin_create_time_utc;
          })
      .def_property_readonly(
          "is_last",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.is_last;
          })
      .def_property_readonly(
          "opaque",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.opaque;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
// messages
// note! wrappers storing references to underlying objects / user is therefore not allowed to store the objects
void create_external_latency(auto &context, const std::string &name) {
  using value_type = roq::ExternalLatency;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "stream_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "latency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.latency;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_gateway_settings(auto &context, const std::string &name) {
  using value_type = roq::GatewaySettings;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "supports",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.supports);
          })
      .def_property_readonly(
          "mbp_max_depth",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_max_depth;
          })
      .def_property_readonly(
          "mbp_tick_size_multiplier",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_tick_size_multiplier;
          })
      .def_property_readonly(
          "mbp_min_trade_vol_multiplier",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_min_trade_vol_multiplier;
          })
      .def_property_readonly(
          "mbp_allow_remove_non_existing",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_allow_remove_non_existing;
          })
      .def_property_readonly(
          "mbp_allow_price_inversion",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_allow_price_inversion;
          })
      .def_property_readonly(
          "oms_download_has_state",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.oms_download_has_state;
          })
      .def_property_readonly(
          "oms_download_has_routing_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.oms_download_has_routing_id;
          })
      .def_property_readonly(
          "oms_request_id_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.oms_request_id_type;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_gateway_status(auto &context, const std::string &name) {
  using value_type = roq::GatewayStatus;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "supported",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.supported);
          })
      .def_property_readonly(
          "available",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.available);
          })
      .def_property_readonly(
          "unavailable",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.unavailable);
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_market_by_price_update(auto &context, const std::string &name) {
  using value_type = roq::MarketByPriceUpdate;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "exchange",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "bids",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.bids);
          })
      .def_property_readonly(
          "asks",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.asks);
          })
      .def_property_readonly(
          "update_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def_property_readonly(
          "exchange_sequence",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_sequence;
          })
      .def_property_readonly(
          "price_decimals",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price_decimals;
          })
      .def_property_readonly(
          "quantity_decimals",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity_decimals;
          })
      .def_property_readonly(
          "max_depth",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_depth;
          })
      .def_property_readonly(
          "checksum",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.checksum;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_market_status(auto &context, const std::string &name) {
  using value_type = roq::MarketStatus;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "stream_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "exchange",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "trading_status",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.trading_status;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_rate_limit_trigger(auto &context, const std::string &name) {
  using value_type = roq::RateLimitTrigger;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "name",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.name;
          })
      .def_property_readonly(
          "origin",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin;
          })
      .def_property_readonly(
          "type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.type;
          })
      .def_property_readonly(
          "users",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.users);
          })
      .def_property_readonly(
          "accounts",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.accounts);
          })
      .def_property_readonly(
          "ban_expires",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.ban_expires;
          })
      .def_property_readonly(
          "triggered_by",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.triggered_by;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_reference_data(auto &context, const std::string &name) {
  using value_type = roq::ReferenceData;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "exchange",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "description",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.description;
          })
      .def_property_readonly(
          "security_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.security_type;
          })
      .def_property_readonly(
          "base_currency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.base_currency;
          })
      .def_property_readonly(
          "quote_currency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quote_currency;
          })
      .def_property_readonly(
          "margin_currency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.margin_currency;
          })
      .def_property_readonly(
          "commission_currency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.commission_currency;
          })
      .def_property_readonly(
          "tick_size",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.tick_size;
          })
      .def_property_readonly(
          "multiplier",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.multiplier;
          })
      .def_property_readonly(
          "min_trade_vol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.min_trade_vol;
          })
      .def_property_readonly(
          "max_trade_vol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_trade_vol;
          })
      .def_property_readonly(
          "trade_vol_step_size",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.trade_vol_step_size;
          })
      .def_property_readonly(
          "option_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.option_type;
          })
      .def_property_readonly(
          "strike_currency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.strike_currency;
          })
      .def_property_readonly(
          "strike_price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.strike_price;
          })
      .def_property_readonly(
          "underlying",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.underlying;
          })
      .def_property_readonly(
          "time_zone",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.time_zone;
          })
      .def_property_readonly(
          "issue_date",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.issue_date;
          })
      .def_property_readonly(
          "settlement_date",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.settlement_date;
          })
      .def_property_readonly(
          "expiry_datetime",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.expiry_datetime;
          })
      .def_property_readonly(
          "expiry_datetime_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.expiry_datetime_utc;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_statistics_update(auto &context, const std::string &name) {
  using value_type = roq::StatisticsUpdate;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "stream_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "exchange",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "statistics",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.statistics);
          })
      .def_property_readonly(
          "update_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_stream_status(auto &context, const std::string &name) {
  using value_type = roq::StreamStatus;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "stream_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "supports",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.supports;
          })
      .def_property_readonly(
          "status",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.status;
          })
      .def_property_readonly(
          "type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.type;
          })
      .def_property_readonly(
          "priority",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.priority;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_top_of_book(auto &context, const std::string &name) {
  using value_type = roq::TopOfBook;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "exchange",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "layer",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.layer;
          })
      .def_property_readonly(
          "update_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
void create_trade_summary(auto &context, const std::string &name) {
  using value_type = roq::TradeSummary;
  using ref_type = utils::Ref<value_type>;
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "stream_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "exchange",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "trades",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.trades);
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
namespace client {
void create_settings(auto &context, const std::string &name) {
  using value_type = roq::client::Settings;
  py::class_<value_type>(context, name.c_str())
      .def(
          py::init<OrderCancelPolicy, OrderManagement>(),
          py::arg("order_cancel_policy") = OrderCancelPolicy::UNDEFINED,
          py::arg("order_management") = OrderManagement::UNDEFINED)
      .def_readonly("order_cancel_policy", &value_type::order_cancel_policy)
      .def_readonly("order_management", &value_type::order_management)
      .def("__repr__", [](const value_type &value) { return fmt::format("{}", value); });
}
struct Config final : public roq::client::Config {
  Config(
      const roq::client::Settings &settings,
      const std::set<std::string> &accounts,
      const std::map<std::string, std::set<std::string>> &symbols)
      : settings_(settings), accounts_(accounts), symbols_(symbols) {}
  void dispatch(Handler &handler) const override {
    handler(settings_);
    for (auto &regex : accounts_) {
      roq::client::Account account{
          .regex = regex,
      };
      handler(account);
    }
    for (auto &[exchange, symbols] : symbols_)
      for (auto &regex : symbols) {
        roq::client::Symbol symbol{
            .regex = regex,
            .exchange = exchange,
        };
        handler(symbol);
      }
  }

 private:
  const roq::client::Settings settings_;
  const std::set<std::string> accounts_;
  const std::map<std::string, std::set<std::string>> symbols_;
};
void create_config(auto &context, const std::string &name) {
  using value_type = Config;
  py::class_<value_type>(context, name.c_str())
      .def(
          py::init<roq::client::Settings, std::set<std::string>, std::map<std::string, std::set<std::string>>>(),
          py::arg("settings"),
          py::arg("accounts"),
          py::arg("symbols"));
}
struct Manager final {
  Manager(const Config &, [[maybe_unused]] const std::vector<std::string> &connections) {}

 protected:
  template <typename T>
  void dispatch(const auto &callback, const auto &message_info, const T &value) {
    auto arg0 = py::cast(utils::Ref<MessageInfo>{message_info});
    auto arg1 = py::cast(utils::Ref<T>{value});
    callback(arg0, arg1);
    if (arg0.ref_count() > 1 || arg1.ref_count() > 1)
      throw std::runtime_error("Objects must not be stored"s);
  }

 public:
  bool dispatch(const std::function<void(py::object, py::object)> &callback) {
    try {
      // XXX exception must break the dispatch loop
      {
        roq::MessageInfo message_info;
        roq::ReferenceData reference_data;
        dispatch(callback, message_info, reference_data);
      }
      {
        roq::MessageInfo message_info;
        roq::TopOfBook top_of_book;
        dispatch(callback, message_info, top_of_book);
      }
    } catch (py::error_already_set &) {
      /*
      log::warn("caught exception!"sv);
      return true;  // break
      */
      throw;
    }
    return false;
  }
};
void create_manager(auto &context, const std::string &name) {
  using value_type = Manager;
  py::class_<value_type>(context, name.c_str())
      .def(py::init<const Config &, std::vector<std::string>>(), py::arg("config"), py::arg("connections"))
      // note! the callback signature **MUST** be py::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &obj, std::function<void(py::object, py::object)> &callback) { return obj.dispatch(callback); },
          py::arg("callback"));
}
struct EventLogReader final {
  template <typename Callback>
  struct Handler final : public roq::client::EventLogReader::Handler {
    explicit Handler(const Callback &callback) : callback_(callback) {}

   protected:
    template <typename T>
    void dispatch(const auto &message_info, const T &value) {
      auto arg0 = py::cast(utils::Ref<MessageInfo>{message_info});
      auto arg1 = py::cast(utils::Ref<T>{value});
      callback_(arg0, arg1);
      if (arg0.ref_count() > 1 || arg1.ref_count() > 1)
        throw std::runtime_error("Objects must not be stored"s);
    }

   protected:
    void operator()(const Event<roq::GatewaySettings> &event) override { dispatch(event.message_info, event.value); }

    void operator()(const Event<roq::StreamStatus> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::ExternalLatency> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::RateLimitTrigger> &event) override { dispatch(event.message_info, event.value); }

    void operator()(const Event<roq::GatewayStatus> &event) override { dispatch(event.message_info, event.value); }

    void operator()(const Event<roq::ReferenceData> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::MarketStatus> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::TopOfBook> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::MarketByPriceUpdate> &event) override {
      dispatch(event.message_info, event.value);
    }
    void operator()(const Event<roq::MarketByOrderUpdate> &) override {}
    void operator()(const Event<roq::TradeSummary> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::StatisticsUpdate> &event) override { dispatch(event.message_info, event.value); }

    void operator()(const Event<roq::CreateOrder> &) override {}
    void operator()(const Event<roq::ModifyOrder> &) override {}
    void operator()(const Event<roq::CancelOrder> &) override {}
    void operator()(const Event<roq::CancelAllOrders> &) override {}

    void operator()(const Event<roq::OrderAck> &) override {}
    void operator()(const Event<roq::OrderUpdate> &) override {}
    void operator()(const Event<roq::TradeUpdate> &) override {}

    void operator()(const Event<roq::PositionUpdate> &) override {}
    void operator()(const Event<roq::FundsUpdate> &) override {}

    void operator()(const Event<roq::CustomMetricsUpdate> &) override {}

   private:
    const Callback &callback_;
  };
  EventLogReader(const std::string_view &path, size_t buffer_size)
      : reader_(roq::client::EventLogReaderFactory::create(path, buffer_size)) {}

  template <typename Callback>
  bool dispatch(const Callback &callback) {
    try {
      Handler handler(callback);
      for (;;) {
        if ((*reader_).dispatch(handler))
          continue;
      }
    } catch (py::error_already_set &) {
      /*
      log::warn("caught exception!"sv);
      return false;  // break
      */
      throw;
    }
    return false;
  }

 private:
  std::unique_ptr<roq::client::EventLogReader> reader_;
};
void create_event_log_reader(auto &context, const std::string &name) {
  using value_type = EventLogReader;
  py::class_<value_type>(context, name.c_str())
      .def(py::init<const std::string_view &, size_t>(), py::arg("path"), py::arg("buffer_size") = 0)
      // note! the callback signature **MUST** be py::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &obj, std::function<void(py::object, py::object)> &callback) { return obj.dispatch(callback); },
          py::arg("callback"));
}
}  // namespace client
namespace cache {
struct MarketByPrice final {
  MarketByPrice(const std::string_view &exchange, const std::string_view &symbol)
      : market_by_price_(roq::client::MarketByPriceFactory::create(exchange, symbol)) {}

  template <typename T>
  void operator()(const T &value) {
    (*market_by_price_)(value);
  }

  auto extract(size_t depth) const {
    std::vector<Layer> tmp(depth);
    (*market_by_price_).extract(tmp);
    return utils::to_list(tmp);
  }

 private:
  std::unique_ptr<roq::cache::MarketByPrice> market_by_price_;
};
void create_market_by_price(auto &context, const std::string &name) {
  using value_type = MarketByPrice;
  py::class_<MarketByPrice>(context, name.c_str())
      .def(py::init<const std::string_view &, const std::string_view &>(), py::arg("exchange") = "", py::arg("symbol"))
      .def(
          "apply",
          [](value_type &value, const utils::Ref<MarketByPriceUpdate> &market_by_price_update) {
            value(market_by_price_update);
          })
      .def("extract", [](const value_type &value, size_t depth) { return value.extract(depth); });
}
}  // namespace cache
}  // namespace python
}  // namespace roq

PYBIND11_MODULE(roq, m) {
  m.doc() = "Roq Python Tools";

  // enums

  roq::python::utils::create_enum<roq::ConnectionStatus>(m, "ConnectionStatus"s);
  roq::python::utils::create_enum<roq::Decimals>(m, "Decimals"s);
  roq::python::utils::create_enum<roq::Error>(m, "Error"s);
  roq::python::utils::create_enum<roq::ExecutionInstruction>(m, "ExecutionInstruction"s);
  roq::python::utils::create_enum<roq::Liquidity>(m, "Liquidity"s);
  roq::python::utils::create_enum<roq::OptionType>(m, "OptionType"s);
  roq::python::utils::create_enum<roq::OrderCancelPolicy>(m, "OrderCancelPolicy"s);
  roq::python::utils::create_enum<roq::OrderManagement>(m, "OrderManagement"s);
  roq::python::utils::create_enum<roq::OrderStatus>(m, "OrderStatus"s);
  roq::python::utils::create_enum<roq::OrderType>(m, "OrderType"s);
  roq::python::utils::create_enum<roq::OrderUpdateAction>(m, "OrderUpdateAction"s);
  roq::python::utils::create_enum<roq::Origin>(m, "Origin"s);
  roq::python::utils::create_enum<roq::PositionEffect>(m, "PositionEffect"s);
  roq::python::utils::create_enum<roq::Priority>(m, "Priority"s);
  roq::python::utils::create_enum<roq::RateLimitType>(m, "RateLimitType"s);
  roq::python::utils::create_enum<roq::RequestIdType>(m, "RequestIdType"s);
  roq::python::utils::create_enum<roq::RequestStatus>(m, "RequestStatus"s);
  roq::python::utils::create_enum<roq::RequestType>(m, "RequestType"s);
  roq::python::utils::create_enum<roq::SecurityType>(m, "SecurityType"s);
  roq::python::utils::create_enum<roq::Side>(m, "Side"s);
  roq::python::utils::create_enum<roq::StatisticsType>(m, "StatisticsType"s);
  roq::python::utils::create_enum<roq::StreamType>(m, "StreamType"s);
  roq::python::utils::create_enum<roq::SupportType>(m, "SupportType"s);
  roq::python::utils::create_enum<roq::TimeInForce>(m, "TimeInForce"s);
  roq::python::utils::create_enum<roq::TradingStatus>(m, "TradingStatus"s);
  roq::python::utils::create_enum<roq::UpdateType>(m, "UpdateType"s);

  // helpers

  roq::python::create_fill(m, "Fill"s);
  roq::python::create_layer(m, "Layer"s);
  roq::python::create_mbp_update(m, "MBPUpdate"s);
  roq::python::create_measurement(m, "Measurement"s);
  roq::python::create_statistics(m, "Statistics"s);
  roq::python::create_trade(m, "Trade"s);
  roq::python::create_uuid(m, "UUID"s);

  // transport

  roq::python::create_message_info(m, "MessageInfo"s);

  // struct

  roq::python::create_external_latency(m, "ExternalLatency"s);
  roq::python::create_gateway_settings(m, "GatewaySettings"s);
  roq::python::create_gateway_status(m, "GatewayStatus"s);
  roq::python::create_market_by_price_update(m, "MarketByPriceUpdate"s);
  roq::python::create_market_status(m, "MarketStatus"s);
  roq::python::create_rate_limit_trigger(m, "RateLimitTrigger"s);
  roq::python::create_reference_data(m, "ReferenceData"s);
  roq::python::create_statistics_update(m, "StatisticsUpdate"s);
  roq::python::create_stream_status(m, "StreamStatus"s);
  roq::python::create_top_of_book(m, "TopOfBook"s);
  roq::python::create_trade_summary(m, "TradeSummary"s);

  auto client = m.def_submodule("client");

  roq::python::client::create_settings(client, "Settings"s);
  roq::python::client::create_config(client, "Config"s);
  roq::python::client::create_manager(client, "Manager"s);

  roq::python::client::create_event_log_reader(client, "EventLogReader"s);

  auto cache = m.def_submodule("cache");

  roq::python::cache::create_market_by_price(cache, "MarketByPrice"s);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "head";
#endif
}
