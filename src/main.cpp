/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <pybind11/pybind11.h>

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

// XXX shouldn't be here...
#include <absl/flags/parse.h>

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

#include <magic_enum.hpp>

#include <nameof.hpp>

#include "roq/client.hpp"

#include "roq/logging.hpp"

/*
 * notes:
 * - absl flags is an issue... how to define from python?
 * - need client external loop implementation
 */

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
auto to_list(auto const &values) {
  using value_type = typename std::remove_cvref<decltype(values)>::type::value_type;
  py::list result;
  for (auto &item : values)
    result.append(value_type{item});
  return result;
}
// note! this is a work-around until IntFlag (?) can be supported properly
template <typename T>
auto to_int_flag(Mask<T> const &value) {
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
  explicit Ref(T const &value) : value{value} {}

  operator T const &() const { return value; }

 private:
  T const &value;
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
      .def_property_readonly("external_trade_id", [](value_type const &value) { return value.external_trade_id; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("liquidity", [](value_type const &value) { return value.liquidity; })
      .def("__repr__", [](value_type const &value) { return fmt::format("{}"sv, value); });
}
// XXX it would be very convenient here to return tuple for unpack
template <>
void create_struct<roq::Layer>(py::module_ &context) {
  using value_type = roq::Layer;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("bid_price", [](value_type const &value) { return value.bid_price; })
      .def_property_readonly("bid_quantity", [](value_type const &value) { return value.bid_quantity; })
      .def_property_readonly("ask_price", [](value_type const &value) { return value.ask_price; })
      .def_property_readonly("ask_quantity", [](value_type const &value) { return value.ask_quantity; })
      .def(
          "astuple",
          [](value_type const &value) {
            return py::make_tuple(value.bid_price, value.bid_quantity, value.ask_price, value.ask_quantity);
          })
      .def("__repr__", [](value_type const &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::MBPUpdate>(py::module_ &context) {
  using value_type = roq::MBPUpdate;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("implied_quantity", [](value_type const &value) { return value.implied_quantity; })
      .def_property_readonly("number_of_orders", [](value_type const &value) { return value.number_of_orders; })
      .def_property_readonly("update_action", [](value_type const &value) { return value.update_action; })
      .def_property_readonly("price_level", [](value_type const &value) { return value.price_level; })
      .def("__repr__", [](value_type const &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::MBOUpdate>(py::module_ &context) {
  using value_type = roq::MBOUpdate;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("priority", [](value_type const &value) { return value.priority; })
      .def_property_readonly("order_id", [](value_type const &value) { return value.order_id; })
      .def_property_readonly("side", [](value_type const &value) { return value.side; })
      .def_property_readonly("action", [](value_type const &value) { return value.action; })
      .def_property_readonly("reason", [](value_type const &value) { return value.reason; })
      .def("__repr__", [](value_type const &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::Measurement>(py::module_ &context) {
  using value_type = roq::Measurement;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("name", [](value_type const &value) { return value.name; })
      .def_property_readonly("value", [](value_type const &value) { return value.value; })
      .def("__repr__", [](value_type const &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::Statistics>(py::module_ &context) {
  using value_type = roq::Statistics;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("type", [](value_type const &value) { return value.type; })
      .def_property_readonly("value", [](value_type const &value) { return value.value; })
      .def_property_readonly("begin_time_utc", [](value_type const &value) { return value.begin_time_utc; })
      .def_property_readonly("end_time_utc", [](value_type const &value) { return value.end_time_utc; })
      .def("__repr__", [](value_type const &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::Trade>(py::module_ &context) {
  using value_type = roq::Trade;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def_property_readonly("side", [](value_type const &value) { return value.side; })
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("trade_id", [](value_type const &value) { return value.trade_id; })
      .def("__repr__", [](value_type const &value) { return fmt::format("{}"sv, value); });
}
template <>
void create_struct<roq::UUID>(py::module_ &context) {
  using value_type = roq::UUID;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str()).def("__repr__", [](value_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source;
          })
      .def_property_readonly(
          "source_name",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_name;
          })
      .def_property_readonly(
          "source_session_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_session_id;
          })
      .def_property_readonly(
          "source_seqno",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_seqno;
          })
      .def_property_readonly(
          "receive_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.receive_time_utc;
          })
      .def_property_readonly(
          "receive_time",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.receive_time;
          })
      .def_property_readonly(
          "source_send_time",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_send_time;
          })
      .def_property_readonly(
          "source_receive_time",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.source_receive_time;
          })
      .def_property_readonly(
          "origin_create_time",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin_create_time;
          })
      .def_property_readonly(
          "origin_create_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin_create_time_utc;
          })
      .def_property_readonly(
          "is_last",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.is_last;
          })
      .def_property_readonly(
          "opaque",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.opaque;
          })
      .def("__repr__", [](ref_type const &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
// messages
template <>
void create_ref_struct<roq::Start>(py::module_ &context) {
  using value_type = roq::Start;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str()).def("__repr__", [](ref_type const &obj) {
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}
template <>
void create_ref_struct<roq::Stop>(py::module_ &context) {
  using value_type = roq::Stop;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str()).def("__repr__", [](ref_type const &obj) {
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}
template <>
void create_ref_struct<roq::Timer>(py::module_ &context) {
  using value_type = roq::Timer;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "now",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.now;
          })
      .def("__repr__", [](ref_type const &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::Connected>(py::module_ &context) {
  using value_type = roq::Connected;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str()).def("__repr__", [](ref_type const &obj) {
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}
template <>
void create_ref_struct<roq::Disconnected>(py::module_ &context) {
  using value_type = roq::Disconnected;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str()).def("__repr__", [](ref_type const &obj) {
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}
template <>
void create_ref_struct<roq::DownloadBegin>(py::module_ &context) {
  using value_type = roq::DownloadBegin;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def("__repr__", [](ref_type const &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::DownloadEnd>(py::module_ &context) {
  using value_type = roq::DownloadEnd;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "max_order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_order_id;
          })
      .def("__repr__", [](ref_type const &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::ExternalLatency>(py::module_ &context) {
  using value_type = roq::ExternalLatency;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "stream_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "latency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.latency;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.supports);
          })
      .def_property_readonly(
          "mbp_max_depth",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_max_depth;
          })
      .def_property_readonly(
          "mbp_tick_size_multiplier",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_tick_size_multiplier;
          })
      .def_property_readonly(
          "mbp_min_trade_vol_multiplier",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_min_trade_vol_multiplier;
          })
      .def_property_readonly(
          "mbp_allow_remove_non_existing",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_allow_remove_non_existing;
          })
      .def_property_readonly(
          "mbp_allow_price_inversion",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.mbp_allow_price_inversion;
          })
      .def_property_readonly(
          "oms_download_has_state",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.oms_download_has_state;
          })
      .def_property_readonly(
          "oms_download_has_routing_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.oms_download_has_routing_id;
          })
      .def_property_readonly(
          "oms_request_id_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.oms_request_id_type;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "supported",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.supported);
          })
      .def_property_readonly(
          "available",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.available);
          })
      .def_property_readonly(
          "unavailable",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.unavailable);
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "bids",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.bids);
          })
      .def_property_readonly(
          "asks",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.asks);
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def_property_readonly(
          "exchange_sequence",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_sequence;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def_property_readonly(
          "price_decimals",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price_decimals;
          })
      .def_property_readonly(
          "quantity_decimals",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity_decimals;
          })
      .def_property_readonly(
          "max_depth",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_depth;
          })
      .def_property_readonly(
          "checksum",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.checksum;
          })
      .def("__repr__", [](ref_type const &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
template <>
void create_ref_struct<roq::MarketByOrderUpdate>(py::module_ &context) {
  using value_type = roq::MarketByOrderUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<ref_type>(context, name.c_str())
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "orders",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.orders);
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def_property_readonly(
          "exchange_sequence",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_sequence;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def_property_readonly(
          "price_decimals",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price_decimals;
          })
      .def_property_readonly(
          "quantity_decimals",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity_decimals;
          })
      .def_property_readonly(
          "max_depth",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_depth;
          })
      .def_property_readonly(
          "checksum",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.checksum;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "trading_status",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.trading_status;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.name;
          })
      .def_property_readonly(
          "origin",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin;
          })
      .def_property_readonly(
          "type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.type;
          })
      .def_property_readonly(
          "users",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.users);
          })
      .def_property_readonly(
          "accounts",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.accounts);
          })
      .def_property_readonly(
          "ban_expires",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.ban_expires;
          })
      .def_property_readonly(
          "triggered_by",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.triggered_by;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "description",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.description;
          })
      .def_property_readonly(
          "security_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.security_type;
          })
      .def_property_readonly(
          "base_currency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.base_currency;
          })
      .def_property_readonly(
          "quote_currency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quote_currency;
          })
      .def_property_readonly(
          "margin_currency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.margin_currency;
          })
      .def_property_readonly(
          "commission_currency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.commission_currency;
          })
      .def_property_readonly(
          "tick_size",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.tick_size;
          })
      .def_property_readonly(
          "multiplier",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.multiplier;
          })
      .def_property_readonly(
          "min_trade_vol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.min_trade_vol;
          })
      .def_property_readonly(
          "max_trade_vol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_trade_vol;
          })
      .def_property_readonly(
          "trade_vol_step_size",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.trade_vol_step_size;
          })
      .def_property_readonly(
          "option_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.option_type;
          })
      .def_property_readonly(
          "strike_currency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.strike_currency;
          })
      .def_property_readonly(
          "strike_price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.strike_price;
          })
      .def_property_readonly(
          "underlying",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.underlying;
          })
      .def_property_readonly(
          "time_zone",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.time_zone;
          })
      .def_property_readonly(
          "issue_date",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.issue_date;
          })
      .def_property_readonly(
          "settlement_date",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.settlement_date;
          })
      .def_property_readonly(
          "expiry_datetime",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.expiry_datetime;
          })
      .def_property_readonly(
          "expiry_datetime_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.expiry_datetime_utc;
          })
      .def_property_readonly(
          "discard",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.discard;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "statistics",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.statistics);
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def_property_readonly(
          "exchange_sequence",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_sequence;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "supports",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.supports;
          })
      .def_property_readonly(
          "transport",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.transport;
          })
      .def_property_readonly(
          "protocol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.protocol;
          })
      .def_property_readonly(
          "encoding",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.encoding;
          })
      .def_property_readonly(
          "priority",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.priority;
          })
      .def_property_readonly(
          "connection_status",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.connection_status;
          })
      .def_property_readonly(
          "interface",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.interface;
          })
      .def_property_readonly(
          "authority",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.authority;
          })
      .def_property_readonly(
          "path",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.path;
          })
      .def_property_readonly(
          "proxy",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.proxy;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "layer",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.layer;
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def_property_readonly(
          "exchange_sequence",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_sequence;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "trades",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.trades);
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "side",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "position_effect",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.position_effect;
          })
      .def_property_readonly(
          "max_show_quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_show_quantity;
          })
      .def_property_readonly(
          "order_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_type;
          })
      .def_property_readonly(
          "time_in_force",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.time_in_force;
          })
      .def_property_readonly(
          "execution_instructions",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.execution_instructions);
          })
      .def_property_readonly(
          "request_template",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.request_template;
          })
      .def_property_readonly(
          "quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity;
          })
      .def_property_readonly(
          "price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price;
          })
      .def_property_readonly(
          "stop_price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stop_price;
          })
      .def_property_readonly(
          "routing_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "request_template",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.request_template;
          })
      .def_property_readonly(
          "quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity;
          })
      .def_property_readonly(
          "price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price;
          })
      .def_property_readonly(
          "routing_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.version;
          })
      .def_property_readonly(
          "conditional_on_version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.conditional_on_version;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "request_template",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.request_template;
          })
      .def_property_readonly(
          "routing_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.version;
          })
      .def_property_readonly(
          "conditional_on_version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.conditional_on_version;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "side",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.type;
          })
      .def_property_readonly(
          "origin",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.origin;
          })
      .def_property_readonly(
          "status",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.status;
          })
      .def_property_readonly(
          "error",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.error;
          })
      .def_property_readonly(
          "text",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.text;
          })
      .def_property_readonly(
          "request_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.request_id;
          })
      .def_property_readonly(
          "external_account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "external_order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_order_id;
          })
      .def_property_readonly(
          "routing_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.version;
          })
      .def_property_readonly(
          "round_trip_latency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.round_trip_latency;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "side",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "position_effect",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.position_effect;
          })
      .def_property_readonly(
          "max_show_quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_show_quantity;
          })
      .def_property_readonly(
          "order_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_type;
          })
      .def_property_readonly(
          "time_in_force",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.time_in_force;
          })
      .def_property_readonly(
          "execution_instructions",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_int_flag(value.execution_instructions);
          })
      .def_property_readonly(
          "create_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.create_time_utc;
          })
      .def_property_readonly(
          "update_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_time_utc;
          })
      .def_property_readonly(
          "external_account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "external_order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_order_id;
          })
      .def_property_readonly(
          "client_order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.client_order_id;
          })
      .def_property_readonly(
          "status",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.status;
          })
      .def_property_readonly(
          "quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.quantity;
          })
      .def_property_readonly(
          "price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.price;
          })
      .def_property_readonly(
          "stop_price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stop_price;
          })
      .def_property_readonly(
          "remaining_quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.remaining_quantity;
          })
      .def_property_readonly(
          "traded_quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.traded_quantity;
          })
      .def_property_readonly(
          "average_traded_price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.average_traded_price;
          })
      .def_property_readonly(
          "last_traded_quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.last_traded_quantity;
          })
      .def_property_readonly(
          "last_traded_price",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.last_traded_price;
          })
      .def_property_readonly(
          "last_liquidity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.last_liquidity;
          })
      .def_property_readonly(
          "routing_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "max_request_version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_request_version;
          })
      .def_property_readonly(
          "max_response_version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_response_version;
          })
      .def_property_readonly(
          "max_accepted_version",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.max_accepted_version;
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def_property_readonly(
          "user",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.user;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.order_id;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "side",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.side;
          })
      .def_property_readonly(
          "position_effect",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.position_effect;
          })
      .def_property_readonly(
          "create_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.create_time_utc;
          })
      .def_property_readonly(
          "update_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_time_utc;
          })
      .def_property_readonly(
          "external_account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "external_order_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_order_id;
          })
      .def_property_readonly(
          "fills",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.fills);
          })
      .def_property_readonly(
          "routing_id",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.routing_id;
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def_property_readonly(
          "user",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.user;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "external_account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "long_quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.long_quantity;
          })
      .def_property_readonly(
          "short_quantity",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.short_quantity;
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.stream_id;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "currency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.currency;
          })
      .def_property_readonly(
          "balance",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.balance;
          })
      .def_property_readonly(
          "hold",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.hold;
          })
      .def_property_readonly(
          "external_account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.external_account;
          })
      .def_property_readonly(
          "update_type",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.update_type;
          })
      .def_property_readonly(
          "exchange_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange_time_utc;
          })
      .def_property_readonly(
          "sending_time_utc",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.sending_time_utc;
          })
      .def("__repr__", [](ref_type const &obj) {
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
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.user;
          })
      .def_property_readonly(
          "label",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.label;
          })
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def_property_readonly(
          "exchange",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.exchange;
          })
      .def_property_readonly(
          "symbol",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.symbol;
          })
      .def_property_readonly(
          "measurements",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return utils::to_list(value.measurements);
          })
      .def("__repr__", [](ref_type const &obj) {
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}
namespace client {
struct Config final : public roq::client::Config {
  Config(
      roq::client::Settings const &settings,
      std::set<std::string> const &accounts,
      std::map<std::string, std::set<std::string>> const &symbols)
      : settings_{settings}, accounts_{accounts}, symbols_{symbols} {}
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
  roq::client::Settings const settings_;
  std::set<std::string> const accounts_;
  std::map<std::string, std::set<std::string>> const symbols_;
};
// base class as callback handler
struct Handler : public roq::client::Handler {
  virtual ~Handler() {}
  virtual void callback(py::object const &, py::object const &) = 0;
};
// trampoline
struct PyHandler : public Handler {
  using Handler::Handler;

  void callback(py::object const &message_info, py::object const &value) override {
    PYBIND11_OVERRIDE_PURE(void, Handler, callback, message_info, value);
  }
};
namespace {
struct Bridge final : public roq::client::Handler {
  explicit Bridge(roq::client::Dispatcher &, python::client::Handler &handler) : handler_{handler} {}

 protected:
  template <typename T>
  void dispatch(auto const &message_info, T const &value) {
    auto arg0 = py::cast(utils::Ref<MessageInfo>{message_info});
    auto arg1 = py::cast(utils::Ref<T>{value});
    handler_.callback(arg0, arg1);
    if (arg0.ref_count() > 1 || arg1.ref_count() > 1)
      throw std::runtime_error{"Objects must not be stored"s};
  }

 protected:
  void operator()(Event<roq::Start> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::Stop> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::Timer> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::Connected> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::Disconnected> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::DownloadBegin> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::DownloadEnd> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::GatewaySettings> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::StreamStatus> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::ExternalLatency> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::RateLimitTrigger> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::GatewayStatus> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::ReferenceData> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketStatus> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::TopOfBook> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketByPriceUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketByOrderUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::TradeSummary> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::StatisticsUpdate> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::OrderAck> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::OrderUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::TradeUpdate> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::PositionUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::FundsUpdate> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::CustomMetricsUpdate> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::ParametersUpdate> const &) override {
    // XXX TODO
  }

  void operator()(Event<roq::PortfolioUpdate> const &) override {
    // XXX TODO
  }

  void operator()(Event<roq::RiskLimitsUpdate> const &) override {
    // XXX TODO
  }

 private:
  python::client::Handler &handler_;
};
}  // namespace
struct Manager final {
  Manager(py::object handler, python::client::Config const &config, std::vector<std::string> const &connections)
      : config_{config}, connections_{connections}, handler_{handler(123)} {
    py::cast<python::client::Handler &>(handler_);  // will throw exception if not inherited from
  }

 protected:
  template <typename T>
  void dispatch(auto const &message_info, T const &value) {
    auto arg0 = py::cast(utils::Ref<MessageInfo>{message_info});
    auto arg1 = py::cast(utils::Ref<T>{value});
    handler_(arg0, arg1);
    if (arg0.ref_count() > 1 || arg1.ref_count() > 1)
      throw std::runtime_error{"Objects must not be stored"s};
  }

 public:
  bool dispatch() {
    // XXX we need an external loop here
    // XXX for now, only exceptions can break the dispatch loop
    try {
      std::vector<std::string_view> connections;
      for (auto &connection : connections_)
        connections.emplace_back(connection);
      auto &handler = py::cast<python::client::Handler &>(handler_);
      roq::client::Trader(config_, std::span{connections}).dispatch<Bridge>(handler);
    } catch (py::error_already_set &) {
      /*
      log::warn("caught exception!"sv);
      return true;  // break
      */
      throw;
    }
    return false;
  }
  void send(roq::CreateOrder const &, [[maybe_unused]] uint8_t source) {
    log::info("HERE"sv);
    // XXX send
  }
  void send(roq::ModifyOrder const &, [[maybe_unused]] uint8_t source) {
    log::info("HERE"sv);
    // XXX send
  }
  void send(roq::CancelOrder const &, [[maybe_unused]] uint8_t source) {
    log::info("HERE"sv);
    // XXX send
  }
  void send(roq::CancelAllOrders const &, [[maybe_unused]] uint8_t source) {
    log::info("HERE"sv);
    // XXX send
  }

 private:
  python::client::Config const config_;
  std::vector<std::string> const connections_;
  py::object handler_;
};
void set_flags(py::dict const &key_value_pairs) {
  // buffer (represents the command-line)
  std::vector<char> buffer;
  // - program
  buffer.emplace_back('\0');
  for (auto &[key, value] : key_value_pairs) {
    // - key
    auto k = py::cast<std::string>(key);
    buffer.emplace_back('-');
    buffer.emplace_back('-');
    std::for_each(std::begin(k), std::end(k), [&](auto c) { buffer.emplace_back(c); });
    buffer.emplace_back('\0');
    // - value
    auto v = py::cast<std::string>(value);
    std::for_each(std::begin(v), std::end(v), [&](auto c) { buffer.emplace_back(c); });
    buffer.emplace_back('\0');
  }
  assert(!std::empty(buffer));
  // - args
  std::vector<char *> args;
  args.emplace_back(&buffer[0]);
  bool insert = true;
  for (size_t i = 1; i < std::size(buffer); ++i) {
    if (insert) {
      args.emplace_back(&buffer[i]);
      insert = false;
    }
    if (buffer[i] == '\0')
      insert = true;
  }
  // initialize or override absl flags
  absl::ParseCommandLine(std::size(args), std::data(args));
}
struct EventLogReader final {
  template <typename Callback>
  struct Handler final : public roq::client::EventLogReader::Handler {
    explicit Handler(Callback const &callback) : callback_{callback} {}

   protected:
    template <typename T>
    void dispatch(auto const &message_info, T const &value) {
      auto arg0 = py::cast(utils::Ref<MessageInfo>{message_info});
      auto arg1 = py::cast(utils::Ref<T>{value});
      callback_(arg0, arg1);
      if (arg0.ref_count() > 1 || arg1.ref_count() > 1)
        throw std::runtime_error{"Objects must not be stored"s};
    }

   protected:
    void operator()(Event<roq::GatewaySettings> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::StreamStatus> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::ExternalLatency> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::RateLimitTrigger> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::GatewayStatus> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::ReferenceData> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::MarketStatus> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::TopOfBook> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::MarketByPriceUpdate> const &event) override {
      dispatch(event.message_info, event.value);
    }
    void operator()(Event<roq::MarketByOrderUpdate> const &event) override {
      dispatch(event.message_info, event.value);
    }
    void operator()(Event<roq::TradeSummary> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::StatisticsUpdate> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::CreateOrder> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::ModifyOrder> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::CancelOrder> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::CancelAllOrders> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::OrderAck> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::OrderUpdate> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::TradeUpdate> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::PositionUpdate> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::FundsUpdate> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::RiskLimits> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::RiskLimitsUpdate> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::CustomMetricsUpdate> const &event) override {
      dispatch(event.message_info, event.value);
    }
    void operator()(Event<roq::CustomMatrixUpdate> const &) override {
      // XXX TODO
    }

    void operator()(Event<roq::ParametersUpdate> const &) override {
      // XXX TODO
    }

    void operator()(Event<roq::PortfolioUpdate> const &) override {
      // XXX TODO
    }

   private:
    Callback const &callback_;
  };
  EventLogReader(std::string_view const &path) : reader_(roq::client::EventLogReaderFactory::create(path)) {}

  template <typename Callback>
  bool dispatch(Callback const &callback) {
    try {
      Handler handler(callback);
      for (;;) {
        if (!(*reader_).dispatch(handler))
          break;
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
struct EventLogMultiplexer final {
  template <typename Callback>
  struct Handler final : public roq::client::EventLogMultiplexer::Handler {
    explicit Handler(Callback const &callback) : callback_{callback} {}

   protected:
    template <typename T>
    void dispatch(auto const &message_info, T const &value) {
      auto arg0 = py::cast(utils::Ref<MessageInfo>{message_info});
      auto arg1 = py::cast(utils::Ref<T>{value});
      callback_(arg0, arg1);
      if (arg0.ref_count() > 1 || arg1.ref_count() > 1)
        throw std::runtime_error{"Objects must not be stored"s};
    }

   protected:
    void operator()(Event<roq::GatewaySettings> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::StreamStatus> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::ExternalLatency> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::RateLimitTrigger> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::GatewayStatus> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::ReferenceData> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::MarketStatus> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::TopOfBook> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::MarketByPriceUpdate> const &event) override {
      dispatch(event.message_info, event.value);
    }
    void operator()(Event<roq::MarketByOrderUpdate> const &event) override {
      dispatch(event.message_info, event.value);
    }
    void operator()(Event<roq::TradeSummary> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::StatisticsUpdate> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::CreateOrder> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::ModifyOrder> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::CancelOrder> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::CancelAllOrders> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::OrderAck> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::OrderUpdate> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::TradeUpdate> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::PositionUpdate> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::FundsUpdate> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::CustomMetricsUpdate> const &event) override {
      dispatch(event.message_info, event.value);
    }

    void operator()(Event<roq::CustomMatrixUpdate> const &) override {
      // XXX TODO
    }

    void operator()(Event<roq::ParametersUpdate> const &) override {
      // XXX TODO
    }

    void operator()(Event<roq::PortfolioUpdate> const &) override {
      // XXX TODO
    }

    void operator()(Event<roq::RiskLimits> const &) override {
      // XXX TODO
    }
    void operator()(Event<roq::RiskLimitsUpdate> const &) override {
      // XXX TODO
    }

   private:
    Callback const &callback_;
  };
  EventLogMultiplexer(std::vector<std::string_view> const &paths)
      : multiplexer_(roq::client::EventLogMultiplexerFactory::create(paths)) {}

  template <typename Callback>
  bool dispatch(Callback const &callback) {
    try {
      Handler handler(callback);
      for (;;) {
        if (!(*multiplexer_).dispatch(handler))
          break;
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
  std::unique_ptr<roq::client::EventLogMultiplexer> multiplexer_;
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
      .def("__repr__", [](value_type const &value) { return fmt::format("{}", value); });
}
template <>
void create_struct<client::Config>(py::module_ &context) {
  using value_type = client::Config;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(
          py::init<roq::client::Settings, std::set<std::string>, std::map<std::string, std::set<std::string>>>(),
          py::arg("settings") = roq::client::Settings(),
          py::arg("accounts") = std::set<std::string>(),
          py::arg("symbols"));
}
template <>
void create_struct<client::Manager>(py::module_ &context) {
  using value_type = client::Manager;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(
          py::init<py::object, python::client::Config const &, std::vector<std::string> const &>(),
          py::arg("handler"),
          py::arg("config"),
          py::arg("connections"))
      .def("dispatch", [](value_type &obj) { return obj.dispatch(); })
      .def(
          "create_order",
          [](value_type &obj,
             std::string_view const &account,
             uint32_t order_id,
             std::string_view const &exchange,
             std::string_view const &symbol,
             roq::Side side,
             roq::PositionEffect position_effect,
             double max_show_quantity,
             roq::OrderType order_type,
             roq::TimeInForce time_in_force,
             roq::Mask<roq::ExecutionInstruction> const &execution_instructions,
             std::string_view const &request_template,
             double quantity,
             double price,
             double stop_price,
             std::string_view const &routing_id,
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
                .request_template = request_template,
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
          py::arg("request_template") = "",
          py::arg("quantity"),
          py::arg("price") = NaN,
          py::arg("stop_price") = NaN,
          py::arg("routing_id") = "",
          py::arg("source"))
      .def(
          "modify_order",
          [](value_type &obj,
             std::string_view const &account,
             uint32_t order_id,
             std::string_view const &request_template,
             double quantity,
             double price,
             std::string_view const &routing_id,
             uint32_t version,
             uint32_t conditional_on_version,
             uint8_t source) {
            roq::ModifyOrder modify_order{
                .account = account,
                .order_id = order_id,
                .request_template = request_template,
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
          py::arg("request_template") = "",
          py::arg("quantity") = NaN,
          py::arg("price") = NaN,
          py::arg("routing_id") = "",
          py::arg("version") = 0,
          py::arg("conditional_on_version") = 0,
          py::arg("source"))
      .def(
          "cancel_order",
          [](value_type &obj,
             std::string_view const &account,
             uint32_t order_id,
             std::string_view const &request_template,
             std::string_view const &routing_id,
             uint32_t version,
             uint32_t conditional_on_version,
             uint8_t source) {
            roq::CancelOrder cancel_order{
                .account = account,
                .order_id = order_id,
                .request_template = request_template,
                .routing_id = routing_id,
                .version = version,
                .conditional_on_version = conditional_on_version,
            };
            obj.send(cancel_order, source);
          },
          py::arg("account"),
          py::arg("order_id"),
          py::arg("request_template") = "",
          py::arg("routing_id") = "",
          py::arg("version") = 0,
          py::arg("conditional_on_version") = 0,
          py::arg("source"))
      .def(
          "cancel_all_orders",
          [](value_type &obj, std::string_view const &account, uint8_t source) {
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
      .def(py::init<std::string_view const &>(), py::arg("path"))
      // note! the callback signature **MUST** be py::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &obj, std::function<void(py::object, py::object)> &callback) { return obj.dispatch(callback); },
          py::arg("callback"));
}
template <>
void create_struct<client::EventLogMultiplexer>(py::module_ &context) {
  using value_type = client::EventLogMultiplexer;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(py::init<std::vector<std::string_view> const &>(), py::arg("paths"))
      // note! the callback signature **MUST** be py::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &obj, std::function<void(py::object, py::object)> &callback) { return obj.dispatch(callback); },
          py::arg("callback"));
}
namespace cache {
struct MarketByPrice final {
  MarketByPrice(std::string_view const &exchange, std::string_view const &symbol)
      : market_by_price_{roq::client::MarketByPriceFactory::create(exchange, symbol)} {}

  template <typename T>
  void operator()(T const &value) {
    (*market_by_price_)(value);
  }

  auto extract(size_t depth) const {
    std::vector<Layer> tmp(depth);
    (*market_by_price_).extract(tmp);
    return utils::to_list(tmp);
  }

  auto print() const {
    std::string result;
    std::vector<MBPUpdate> bids, asks;
    (*market_by_price_).create_snapshot(bids, asks, [&result](auto &market_by_price_update) {
      result = fmt::format("{}"sv, market_by_price_update);
    });
    return result;
  }

 private:
  std::unique_ptr<roq::cache::MarketByPrice> market_by_price_;
};
struct MarketByOrder final {
  MarketByOrder(std::string_view const &exchange, std::string_view const &symbol)
      : market_by_order_{roq::client::MarketByOrderFactory::create(
            roq::client::MarketByOrderFactory::Type::SIMPLE_2, {}, exchange, symbol)} {}

  template <typename T>
  void operator()(T const &value) {
    (*market_by_order_)(value);
  }

  auto extract(size_t depth) const {
    std::vector<Layer> tmp(depth);
    (*market_by_order_).extract(tmp);
    return utils::to_list(tmp);
  }

  auto print() const {
    std::string result;
    std::vector<MBOUpdate> orders;
    (*market_by_order_).create_snapshot(orders, [&result](auto &market_by_order_update) {
      result = fmt::format("{}"sv, market_by_order_update);
    });
    return result;
  }

 private:
  std::unique_ptr<roq::cache::MarketByOrder> market_by_order_;
};
}  // namespace cache
template <>
void create_struct<cache::MarketByPrice>(py::module_ &context) {
  using value_type = cache::MarketByPrice;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(py::init<std::string_view const &, std::string_view const &>(), py::arg("exchange") = "", py::arg("symbol"))
      .def(
          "apply",
          [](value_type &value, utils::Ref<MarketByPriceUpdate> const &market_by_price_update) {
            value(market_by_price_update);
          })
      .def("extract", [](value_type const &value, size_t depth) { return value.extract(depth); })
      .def("__repr__", [](value_type const &value) { return value.print(); });
}
template <>
void create_struct<cache::MarketByOrder>(py::module_ &context) {
  using value_type = cache::MarketByOrder;
  std::string name{nameof::nameof_short_type<value_type>()};
  py::class_<value_type>(context, name.c_str())
      .def(py::init<std::string_view const &, std::string_view const &>(), py::arg("exchange") = "", py::arg("symbol"))
      .def(
          "apply",
          [](value_type &value, utils::Ref<MarketByOrderUpdate> const &market_by_order_update) {
            value(market_by_order_update);
          })
      .def("extract", [](value_type const &value, size_t depth) { return value.extract(depth); })
      .def("__repr__", [](value_type const &value) { return value.print(); });
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
  roq::python::utils::create_enum<roq::UpdateAction>(m);
  roq::python::utils::create_enum<roq::UpdateType>(m);

  // helpers

  roq::python::create_struct<roq::Fill>(m);
  roq::python::create_struct<roq::Layer>(m);
  roq::python::create_struct<roq::MBPUpdate>(m);
  roq::python::create_struct<roq::MBOUpdate>(m);
  roq::python::create_struct<roq::Measurement>(m);
  roq::python::create_struct<roq::Statistics>(m);
  roq::python::create_struct<roq::Trade>(m);
  roq::python::create_struct<roq::UUID>(m);

  // transport

  roq::python::create_ref_struct<roq::MessageInfo>(m);

  // struct

  roq::python::create_ref_struct<roq::Start>(m);
  roq::python::create_ref_struct<roq::Stop>(m);
  roq::python::create_ref_struct<roq::Timer>(m);
  roq::python::create_ref_struct<roq::Connected>(m);
  roq::python::create_ref_struct<roq::Disconnected>(m);

  roq::python::create_ref_struct<roq::DownloadBegin>(m);
  roq::python::create_ref_struct<roq::DownloadEnd>(m);

  roq::python::create_ref_struct<roq::GatewaySettings>(m);

  roq::python::create_ref_struct<roq::StreamStatus>(m);
  roq::python::create_ref_struct<roq::ExternalLatency>(m);
  roq::python::create_ref_struct<roq::RateLimitTrigger>(m);

  roq::python::create_ref_struct<roq::GatewayStatus>(m);

  roq::python::create_ref_struct<roq::ReferenceData>(m);
  roq::python::create_ref_struct<roq::MarketStatus>(m);
  roq::python::create_ref_struct<roq::TopOfBook>(m);
  roq::python::create_ref_struct<roq::MarketByPriceUpdate>(m);
  roq::python::create_ref_struct<roq::MarketByOrderUpdate>(m);
  roq::python::create_ref_struct<roq::TradeSummary>(m);
  roq::python::create_ref_struct<roq::StatisticsUpdate>(m);

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

  py::class_<roq::python::client::Handler, roq::python::client::PyHandler>(client, "Handler")
      .def(py::init<>())
      .def("callback", &roq::python::client::Handler::callback);

  roq::python::create_struct<roq::client::Settings>(client);
  roq::python::create_struct<roq::python::client::Config>(client);
  roq::python::create_struct<roq::python::client::Manager>(client);

  client.def("set_flags", &roq::python::client::set_flags, "WORKAROUND", py::arg("flags"));

  roq::python::create_struct<roq::python::client::EventLogReader>(client);
  roq::python::create_struct<roq::python::client::EventLogMultiplexer>(client);

  auto cache = m.def_submodule("cache");

  roq::python::create_struct<roq::python::cache::MarketByPrice>(cache);
  roq::python::create_struct<roq::python::cache::MarketByOrder>(cache);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "head";
#endif
}
