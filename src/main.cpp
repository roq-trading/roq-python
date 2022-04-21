/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <pybind11/pybind11.h>

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <magic_enum.hpp>

#include <nameof.hpp>

#include "roq/client.hpp"

#include "roq/logging.hpp"

using namespace std::literals;         // NOLINT
using namespace std::chrono_literals;  // NOLINT

namespace py = pybind11;  // convention

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// verify nameof reflection (just testing some relevant samples)
static_assert(nameof::nameof_short_type<roq::ConnectionStatus>() == "ConnectionStatus"sv);
static_assert(nameof::nameof_short_type<roq::Fill>() == "Fill"sv);
static_assert(nameof::nameof_short_type<roq::MessageInfo>() == "MessageInfo"sv);

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
void create_enum(auto &context) {
  std::string name{nameof::nameof_short_type<T>()};
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
// note! copy values
template <typename T>
void create_struct(py::module_ &context);
// note! reference to an underlying object (user is therefore not allowed to keep handles)
template <typename T>
void create_ref_struct(py::module_ &context);
// helpers
template <>
void create_struct<roq::Fill>(py::module_ &context) {
  using value_type = roq::Fill;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("external_trade_id", [](const value_type &value) { return value.external_trade_id; })
      .def_property_readonly("quantity", [](const value_type &value) { return value.quantity; })
      .def_property_readonly("price", [](const value_type &value) { return value.price; })
      .def_property_readonly("liquidity", [](const value_type &value) { return value.liquidity; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::Layer>(py::module_ &context) {
  using value_type = roq::Layer;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("bid_price", [](const value_type &value) { return value.bid_price; })
      .def_property_readonly("bid_quantity", [](const value_type &value) { return value.bid_quantity; })
      .def_property_readonly("ask_price", [](const value_type &value) { return value.ask_price; })
      .def_property_readonly("ask_quantity", [](const value_type &value) { return value.ask_quantity; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::MBPUpdate>(py::module_ &context) {
  using value_type = roq::MBPUpdate;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("price", [](const value_type &value) { return value.price; })
      .def_property_readonly("quantity", [](const value_type &value) { return value.quantity; })
      .def_property_readonly("implied_quantity", [](const value_type &value) { return value.implied_quantity; })
      .def_property_readonly("price_level", [](const value_type &value) { return value.price_level; })
      .def_property_readonly("number_of_orders", [](const value_type &value) { return value.number_of_orders; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::Measurement>(py::module_ &context) {
  using value_type = roq::Measurement;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("name", [](const value_type &value) { return value.name; })
      .def_property_readonly("value", [](const value_type &value) { return value.value; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::Statistics>(py::module_ &context) {
  using value_type = roq::Statistics;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("type", [](const value_type &value) { return value.type; })
      .def_property_readonly("value", [](const value_type &value) { return value.value; })
      .def_property_readonly("begin_time_utc", [](const value_type &value) { return value.begin_time_utc; })
      .def_property_readonly("end_time_utc", [](const value_type &value) { return value.end_time_utc; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::Trade>(py::module_ &context) {
  using value_type = roq::Trade;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("side", [](const value_type &value) { return value.side; })
      .def_property_readonly("price", [](const value_type &value) { return value.price; })
      .def_property_readonly("quantity", [](const value_type &value) { return value.quantity; })
      .def_property_readonly("trade_id", [](const value_type &value) { return value.trade_id; })
      .def("__repr__", [](const value_type &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::UUID>(py::module_ &context) {
  using value_type = roq::UUID;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str()).def("__repr__", [](const value_type &obj) {
    return fmt::format("{}"sv, obj);
  });
}
// transport
template <>
void create_ref_struct<roq::MessageInfo>(py::module_ &context) {
  using value_type = roq::MessageInfo;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::ExternalLatency>(py::module_ &context) {
  using value_type = roq::ExternalLatency;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::GatewaySettings>(py::module_ &context) {
  using value_type = roq::GatewaySettings;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::GatewayStatus>(py::module_ &context) {
  using value_type = roq::GatewayStatus;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::MarketByPriceUpdate>(py::module_ &context) {
  using value_type = roq::MarketByPriceUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::MarketStatus>(py::module_ &context) {
  using value_type = roq::MarketStatus;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::RateLimitTrigger>(py::module_ &context) {
  using value_type = roq::RateLimitTrigger;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::ReferenceData>(py::module_ &context) {
  using value_type = roq::ReferenceData;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::StatisticsUpdate>(py::module_ &context) {
  using value_type = roq::StatisticsUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::StreamStatus>(py::module_ &context) {
  using value_type = roq::StreamStatus;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
          "transport",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.transport;
          })
      .def_property_readonly(
          "protocol",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.protocol;
          })
      .def_property_readonly(
          "encoding",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.encoding;
          })
      .def_property_readonly(
          "priority",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.priority;
          })
      .def_property_readonly(
          "connection_status",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.connection_status;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::TopOfBook>(py::module_ &context) {
  using value_type = roq::TopOfBook;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
template <>
void create_ref_struct<roq::TradeSummary>(py::module_ &context) {
  using value_type = roq::TradeSummary;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
// ...
template <>
void create_ref_struct<roq::CreateOrder>(py::module_ &context) {
  using value_type = roq::CreateOrder;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
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
          "side",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "position_effect",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.position_effect;
          })
      .def_property_readonly(
          "max_show_quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_show_quantity;
          })
      .def_property_readonly(
          "order_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_type;
          })
      .def_property_readonly(
          "time_in_force",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.time_in_force;
          })
      .def_property_readonly(
          "execution_instructions",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.execution_instructions);
          })
      .def_property_readonly(
          "order_template",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_template;
          })
      .def_property_readonly(
          "quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity;
          })
      .def_property_readonly(
          "price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price;
          })
      .def_property_readonly(
          "stop_price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stop_price;
          })
      .def_property_readonly(
          "routing_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::ModifyOrder>(py::module_ &context) {
  using value_type = roq::ModifyOrder;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity;
          })
      .def_property_readonly(
          "price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price;
          })
      .def_property_readonly(
          "routing_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.version;
          })
      .def_property_readonly(
          "conditional_on_version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.conditional_on_version;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::CancelOrder>(py::module_ &context) {
  using value_type = roq::CancelOrder;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "routing_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.version;
          })
      .def_property_readonly(
          "conditional_on_version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.conditional_on_version;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::CancelAllOrders>(py::module_ &context) {
  using value_type = roq::CancelAllOrders;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
// ...
template <>
void create_ref_struct<roq::OrderAck>(py::module_ &context) {
  using value_type = roq::OrderAck;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
          "order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
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
          "side",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.type;
          })
      .def_property_readonly(
          "origin",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin;
          })
      .def_property_readonly(
          "status",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.status;
          })
      .def_property_readonly(
          "error",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.error;
          })
      .def_property_readonly(
          "text",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.text;
          })
      .def_property_readonly(
          "request_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.request_id;
          })
      .def_property_readonly(
          "external_account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "external_order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_order_id;
          })
      .def_property_readonly(
          "routing_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.version;
          })
      .def_property_readonly(
          "round_trip_latency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.round_trip_latency;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::OrderUpdate>(py::module_ &context) {
  using value_type = roq::OrderUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
          "order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
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
          "side",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "position_effect",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.position_effect;
          })
      .def_property_readonly(
          "max_show_quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_show_quantity;
          })
      .def_property_readonly(
          "order_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_type;
          })
      .def_property_readonly(
          "time_in_force",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.time_in_force;
          })
      .def_property_readonly(
          "execution_instructions",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.execution_instructions);
          })
      .def_property_readonly(
          "order_template",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_template;
          })
      .def_property_readonly(
          "create_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.create_time_utc;
          })
      .def_property_readonly(
          "update_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_time_utc;
          })
      .def_property_readonly(
          "external_account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "external_order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_order_id;
          })
      .def_property_readonly(
          "status",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.status;
          })
      .def_property_readonly(
          "quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity;
          })
      .def_property_readonly(
          "price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price;
          })
      .def_property_readonly(
          "stop_price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stop_price;
          })
      .def_property_readonly(
          "remaining_quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.remaining_quantity;
          })
      .def_property_readonly(
          "traded_quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.traded_quantity;
          })
      .def_property_readonly(
          "average_traded_price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.average_traded_price;
          })
      .def_property_readonly(
          "last_traded_quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.last_traded_quantity;
          })
      .def_property_readonly(
          "last_traded_price",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.last_traded_price;
          })
      .def_property_readonly(
          "last_liquidity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.last_liquidity;
          })
      .def_property_readonly(
          "routing_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "max_request_version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_request_version;
          })
      .def_property_readonly(
          "max_response_version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_response_version;
          })
      .def_property_readonly(
          "max_accepted_version",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_accepted_version;
          })
      .def_property_readonly(
          "update_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::TradeUpdate>(py::module_ &context) {
  using value_type = roq::TradeUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
          "order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
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
          "side",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "position_effect",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.position_effect;
          })
      .def_property_readonly(
          "create_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.create_time_utc;
          })
      .def_property_readonly(
          "update_time_utc",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_time_utc;
          })
      .def_property_readonly(
          "external_account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "external_order_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_order_id;
          })
      .def_property_readonly(
          "fills",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.fills);
          })
      .def_property_readonly(
          "routing_id",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "update_type",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::PositionUpdate>(py::module_ &context) {
  using value_type = roq::PositionUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
          "external_account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "long_quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.long_quantity;
          })
      .def_property_readonly(
          "short_quantity",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.short_quantity;
          })
      .def_property_readonly(
          "long_quantity_begin",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.long_quantity_begin;
          })
      .def_property_readonly(
          "short_quantity_begin",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.short_quantity_begin;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::FundsUpdate>(py::module_ &context) {
  using value_type = roq::FundsUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
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
          "currency",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.currency;
          })
      .def_property_readonly(
          "balance",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.balance;
          })
      .def_property_readonly(
          "hold",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.hold;
          })
      .def_property_readonly(
          "external_account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
//
template <>
void create_ref_struct<roq::CustomMetricsUpdate>(py::module_ &context) {
  using value_type = roq::CustomMetricsUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "user",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.user;
          })
      .def_property_readonly(
          "label",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.label;
          })
      .def_property_readonly(
          "account",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
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
          "measurements",
          [](const ref_type &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.measurements);
          })
      .def("__repr__", [](const ref_type &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
namespace client {
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
struct Handler : public roq::client::Handler {
  virtual ~Handler() {}
};
namespace {
// XXX this is **NOT** the final solution
struct Bridge final : public roq::client::Handler {
  explicit Bridge(roq::client::Dispatcher &, python::client::Handler &handler) : handler_(handler) {}

 protected:
  void operator()(const Event<Connected> &) {}
  void operator()(const Event<Disconnected> &) {}

 private:
  python::client::Handler &handler_;
};
}  // namespace
struct Manager final {
  // XXX reference to config?
  Manager(const python::client::Config &config, const std::vector<std::string> &connections)
      : config_(config), connections_(connections) {}

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
  static bool dispatch(Manager &manager, Handler &handler) {
    // XXX we need an external loop here
    // XXX for now, only exceptions can break the dispatch loop
    try {
      std::vector<std::string_view> connections;
      for (auto &connection : manager.connections_)
        connections.emplace_back(connection);
      roq::client::Trader(manager.config_, std::span{connections}).dispatch<Bridge>(handler);
    } catch (py::error_already_set &) {
      /*
      log::warn("caught exception!"sv);
      return true;  // break
      */
      throw;
    }
    return false;
  }
  void send(const roq::CreateOrder &, [[maybe_unused]] uint8_t source) {
    // XXX send
  }
  void send(const roq::ModifyOrder &, [[maybe_unused]] uint8_t source) {
    // XXX send
  }
  void send(const roq::CancelOrder &, [[maybe_unused]] uint8_t source) {
    // XXX send
  }
  void send(const roq::CancelAllOrders &, [[maybe_unused]] uint8_t source) {
    // XXX send
  }

 private:
  const python::client::Config config_;
  const std::vector<std::string> connections_;
};
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

    void operator()(const Event<roq::CreateOrder> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::ModifyOrder> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::CancelOrder> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::CancelAllOrders> &event) override { dispatch(event.message_info, event.value); }

    void operator()(const Event<roq::OrderAck> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::OrderUpdate> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::TradeUpdate> &event) override { dispatch(event.message_info, event.value); }

    void operator()(const Event<roq::PositionUpdate> &event) override { dispatch(event.message_info, event.value); }
    void operator()(const Event<roq::FundsUpdate> &event) override { dispatch(event.message_info, event.value); }

    void operator()(const Event<roq::CustomMetricsUpdate> &event) override {
      dispatch(event.message_info, event.value);
    }

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
}  // namespace client
template <>
void create_struct<roq::client::Settings>(py::module_ &context) {
  using value_type = roq::client::Settings;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(
          py::init<OrderCancelPolicy, OrderManagement>(),
          py::arg("order_cancel_policy") = OrderCancelPolicy::UNDEFINED,
          py::arg("order_management") = OrderManagement::UNDEFINED)
      .def_readonly("order_cancel_policy", &value_type::order_cancel_policy)
      .def_readonly("order_management", &value_type::order_management)
      .def("__repr__", [](const value_type &value) { return fmt::format("{}", value); });
}
template <>
void create_struct<client::Config>(py::module_ &context) {
  using value_type = client::Config;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(
          py::init<roq::client::Settings, std::set<std::string>, std::map<std::string, std::set<std::string>>>(),
          py::arg("settings"),
          py::arg("accounts"),
          py::arg("symbols"));
}
template <>
void create_struct<client::Manager>(py::module_ &context) {
  using value_type = client::Manager;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(
          py::init<const python::client::Config &, const std::vector<std::string> &>(),
          py::arg("config"),
          py::arg("connections"))
      .def_static(
          "dispatch",
          [](value_type &manager, python::client::Handler &handler) {
            return [&](auto &manager, auto &handler) { value_type::dispatch(manager, handler); };
          },
          py::arg("manager"),
          py::arg("handler"))
      .def(
          "create_order",
          [](value_type &obj,
             const std::string_view &account,
             uint32_t order_id,
             const std::string_view &exchange,
             const std::string_view &symbol,
             roq::Side side,
             roq::PositionEffect position_effect,
             double max_show_quantity,
             roq::OrderType order_type,
             roq::TimeInForce time_in_force,
             const roq::Mask<roq::ExecutionInstruction> &execution_instructions,
             const std::string_view &order_template,
             double quantity,
             double price,
             double stop_price,
             const std::string_view &routing_id,
             uint8_t source) {
            roq::CreateOrder create_order{
                .account = account,
                .order_id = order_id,
                .exchange = exchange,
                .symbol = symbol,
                .side = side,
                .position_effect = position_effect,
                .max_show_quantity = max_show_quantity,
                .order_type = order_type,
                .time_in_force = time_in_force,
                .execution_instructions = execution_instructions,
                .order_template = order_template,
                .quantity = quantity,
                .price = price,
                .stop_price = stop_price,
                .routing_id = routing_id,
            };
            obj.send(create_order, source);
          },
          py::arg("account"),
          py::arg("order_id"),
          py::arg("exchange"),
          py::arg("symbol"),
          py::arg("side"),
          py::arg("position_effect") = roq::PositionEffect::UNDEFINED,
          py::arg("max_show_quantity") = NaN,
          py::arg("order_type"),
          py::arg("time_in_force") = roq::TimeInForce::UNDEFINED,
          py::arg("execution_instructions"),  // = roq::Mask<roq::ExecutionInstruction>{},
          py::arg("order_template") = "",
          py::arg("quantity"),
          py::arg("price") = NaN,
          py::arg("stop_price") = NaN,
          py::arg("routing_id") = "",
          py::arg("source"))
      .def(
          "modify_order",
          [](value_type &obj,
             const std::string_view &account,
             uint32_t order_id,
             double quantity,
             double price,
             const std::string_view &routing_id,
             uint32_t version,
             uint32_t conditional_on_version,
             uint8_t source) {
            roq::ModifyOrder modify_order{
                .account = account,
                .order_id = order_id,
                .quantity = quantity,
                .price = price,
                .routing_id = routing_id,
                .version = version,
                .conditional_on_version = conditional_on_version,
            };
            obj.send(modify_order, source);
          },
          py::arg("account"),
          py::arg("order_id"),
          py::arg("quantity") = NaN,
          py::arg("price") = NaN,
          py::arg("routing_id") = "",
          py::arg("version") = 0,
          py::arg("conditional_on_version") = 0,
          py::arg("source"))
      .def(
          "cancel_order",
          [](value_type &obj,
             const std::string_view &account,
             uint32_t order_id,
             const std::string_view &routing_id,
             uint32_t version,
             uint32_t conditional_on_version,
             uint8_t source) {
            roq::CancelOrder cancel_order{
                .account = account,
                .order_id = order_id,
                .routing_id = routing_id,
                .version = version,
                .conditional_on_version = conditional_on_version,
            };
            obj.send(cancel_order, source);
          },
          py::arg("account"),
          py::arg("order_id"),
          py::arg("routing_id") = "",
          py::arg("version") = 0,
          py::arg("conditional_on_version") = 0,
          py::arg("source"))
      .def(
          "cancel_all_orders",
          [](value_type &obj, const std::string_view &account, uint8_t source) {
            roq::CancelAllOrders cancel_all_orders{
                .account = account,
            };
            obj.send(cancel_all_orders, source);
          },
          py::arg("account") = "",
          py::arg("source"));
}
template <>
void create_struct<client::EventLogReader>(py::module_ &context) {
  using value_type = client::EventLogReader;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(py::init<const std::string_view &, size_t>(), py::arg("path"), py::arg("buffer_size") = 0)
      // note! the callback signature **MUST** be py::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &obj, std::function<void(py::object, py::object)> &callback) { return obj.dispatch(callback); },
          py::arg("callback"));
}
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
}  // namespace cache
template <>
void create_struct<cache::MarketByPrice>(py::module_ &context) {
  using value_type = cache::MarketByPrice;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(py::init<const std::string_view &, const std::string_view &>(), py::arg("exchange") = "", py::arg("symbol"))
      .def(
          "apply",
          [](value_type &value, const utils::Ref<MarketByPriceUpdate> &market_by_price_update) {
            value(market_by_price_update);
          })
      .def("extract", [](const value_type &value, size_t depth) { return value.extract(depth); });
}
}  // namespace python
}  // namespace roq

PYBIND11_MODULE(roq, m) {
  m.doc() = "Roq Python Tools";

  // enums

  roq::python::utils::create_enum<roq::ConnectionStatus>(m);
  roq::python::utils::create_enum<roq::Decimals>(m);
  roq::python::utils::create_enum<roq::Encoding>(m);
  roq::python::utils::create_enum<roq::Error>(m);
  roq::python::utils::create_enum<roq::ExecutionInstruction>(m);
  roq::python::utils::create_enum<roq::Liquidity>(m);
  roq::python::utils::create_enum<roq::OptionType>(m);
  roq::python::utils::create_enum<roq::OrderCancelPolicy>(m);
  roq::python::utils::create_enum<roq::OrderManagement>(m);
  roq::python::utils::create_enum<roq::OrderStatus>(m);
  roq::python::utils::create_enum<roq::OrderType>(m);
  roq::python::utils::create_enum<roq::OrderUpdateAction>(m);
  roq::python::utils::create_enum<roq::Origin>(m);
  roq::python::utils::create_enum<roq::PositionEffect>(m);
  roq::python::utils::create_enum<roq::Priority>(m);
  roq::python::utils::create_enum<roq::Protocol>(m);
  roq::python::utils::create_enum<roq::RateLimitType>(m);
  roq::python::utils::create_enum<roq::RequestIdType>(m);
  roq::python::utils::create_enum<roq::RequestStatus>(m);
  roq::python::utils::create_enum<roq::RequestType>(m);
  roq::python::utils::create_enum<roq::SecurityType>(m);
  roq::python::utils::create_enum<roq::Side>(m);
  roq::python::utils::create_enum<roq::StatisticsType>(m);
  roq::python::utils::create_enum<roq::SupportType>(m);
  roq::python::utils::create_enum<roq::TimeInForce>(m);
  roq::python::utils::create_enum<roq::TradingStatus>(m);
  roq::python::utils::create_enum<roq::Transport>(m);
  roq::python::utils::create_enum<roq::UpdateType>(m);

  // helpers

  roq::python::create_struct<roq::Fill>(m);
  roq::python::create_struct<roq::Layer>(m);
  roq::python::create_struct<roq::MBPUpdate>(m);
  roq::python::create_struct<roq::Measurement>(m);
  roq::python::create_struct<roq::Statistics>(m);
  roq::python::create_struct<roq::Trade>(m);
  roq::python::create_struct<roq::UUID>(m);

  // transport

  roq::python::create_ref_struct<roq::MessageInfo>(m);

  // struct

  roq::python::create_ref_struct<roq::ExternalLatency>(m);
  roq::python::create_ref_struct<roq::GatewaySettings>(m);
  roq::python::create_ref_struct<roq::GatewayStatus>(m);
  roq::python::create_ref_struct<roq::MarketByPriceUpdate>(m);
  roq::python::create_ref_struct<roq::MarketStatus>(m);
  roq::python::create_ref_struct<roq::RateLimitTrigger>(m);
  roq::python::create_ref_struct<roq::ReferenceData>(m);
  roq::python::create_ref_struct<roq::StatisticsUpdate>(m);
  roq::python::create_ref_struct<roq::StreamStatus>(m);
  roq::python::create_ref_struct<roq::TopOfBook>(m);
  roq::python::create_ref_struct<roq::TradeSummary>(m);

  roq::python::create_ref_struct<roq::CreateOrder>(m);
  roq::python::create_ref_struct<roq::ModifyOrder>(m);
  roq::python::create_ref_struct<roq::CancelOrder>(m);
  roq::python::create_ref_struct<roq::CancelAllOrders>(m);

  roq::python::create_ref_struct<roq::OrderAck>(m);
  roq::python::create_ref_struct<roq::OrderUpdate>(m);
  roq::python::create_ref_struct<roq::TradeUpdate>(m);

  roq::python::create_ref_struct<roq::PositionUpdate>(m);
  roq::python::create_ref_struct<roq::FundsUpdate>(m);

  roq::python::create_ref_struct<roq::CustomMetricsUpdate>(m);

  auto client = m.def_submodule("client");

  py::class_<roq::python::client::Handler>(client, "Handler").def(py::init<>());

  roq::python::create_struct<roq::client::Settings>(client);
  roq::python::create_struct<roq::python::client::Config>(client);
  roq::python::create_struct<roq::python::client::Manager>(client);

  /*
  m.def(
      "create", [](py::object x) { fmt::print("test"sv); }, py::arg("client_type"));
  */

  roq::python::create_struct<roq::python::client::EventLogReader>(client);

  auto cache = m.def_submodule("cache");

  roq::python::create_struct<roq::python::cache::MarketByPrice>(cache);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "head";
#endif
}
