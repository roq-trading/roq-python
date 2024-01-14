/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/details.hpp"

#include "roq/python/utils.hpp"

namespace roq {
namespace python {

// helpers

template <>
void utils::create_struct<roq::Fill>(pybind11::module_ &module) {
  using value_type = roq::Fill;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly("external_trade_id", [](value_type const &value) { return value.external_trade_id; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("liquidity", [](value_type const &value) { return value.liquidity; })
      .def("__repr__", [](value_type const &value) {
        using namespace std::literals;
        return fmt::format("{}"sv, value);
      });
}

// XXX it would be very convenient here to return tuple for unpack
template <>
void utils::create_struct<roq::Layer>(pybind11::module_ &module) {
  using value_type = roq::Layer;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly("bid_price", [](value_type const &value) { return value.bid_price; })
      .def_property_readonly("bid_quantity", [](value_type const &value) { return value.bid_quantity; })
      .def_property_readonly("ask_price", [](value_type const &value) { return value.ask_price; })
      .def_property_readonly("ask_quantity", [](value_type const &value) { return value.ask_quantity; })
      .def(
          "astuple",
          [](value_type const &value) {
            return pybind11::make_tuple(value.bid_price, value.bid_quantity, value.ask_price, value.ask_quantity);
          })
      .def("__repr__", [](value_type const &value) {
        using namespace std::literals;
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_struct<roq::MBPUpdate>(pybind11::module_ &module) {
  using value_type = roq::MBPUpdate;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("implied_quantity", [](value_type const &value) { return value.implied_quantity; })
      .def_property_readonly("number_of_orders", [](value_type const &value) { return value.number_of_orders; })
      .def_property_readonly("update_action", [](value_type const &value) { return value.update_action; })
      .def_property_readonly("price_level", [](value_type const &value) { return value.price_level; })
      .def("__repr__", [](value_type const &value) {
        using namespace std::literals;
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_struct<roq::MBOUpdate>(pybind11::module_ &module) {
  using value_type = roq::MBOUpdate;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("priority", [](value_type const &value) { return value.priority; })
      .def_property_readonly("order_id", [](value_type const &value) { return value.order_id; })
      .def_property_readonly("side", [](value_type const &value) { return value.side; })
      .def_property_readonly("action", [](value_type const &value) { return value.action; })
      .def_property_readonly("reason", [](value_type const &value) { return value.reason; })
      .def("__repr__", [](value_type const &value) {
        using namespace std::literals;
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_struct<roq::Measurement>(pybind11::module_ &module) {
  using value_type = roq::Measurement;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly("name", [](value_type const &value) { return value.name; })
      .def_property_readonly("value", [](value_type const &value) { return value.value; })
      .def("__repr__", [](value_type const &value) {
        using namespace std::literals;
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_struct<roq::Statistics>(pybind11::module_ &module) {
  using value_type = roq::Statistics;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly("type", [](value_type const &value) { return value.type; })
      .def_property_readonly("value", [](value_type const &value) { return value.value; })
      .def_property_readonly("begin_time_utc", [](value_type const &value) { return value.begin_time_utc; })
      .def_property_readonly("end_time_utc", [](value_type const &value) { return value.end_time_utc; })
      .def("__repr__", [](value_type const &value) {
        using namespace std::literals;
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_struct<roq::Trade>(pybind11::module_ &module) {
  using value_type = roq::Trade;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly("side", [](value_type const &value) { return value.side; })
      .def_property_readonly("price", [](value_type const &value) { return value.price; })
      .def_property_readonly("quantity", [](value_type const &value) { return value.quantity; })
      .def_property_readonly("trade_id", [](value_type const &value) { return value.trade_id; })
      .def("__repr__", [](value_type const &value) {
        using namespace std::literals;
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_struct<roq::UUID>(pybind11::module_ &module) {
  using value_type = roq::UUID;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str()).def("__repr__", [](value_type const &obj) {
    using namespace std::literals;
    return fmt::format("{}"sv, obj);
  });
}

// transport

template <>
void utils::create_ref_struct<roq::MessageInfo>(pybind11::module_ &module) {
  using value_type = roq::MessageInfo;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

// messages

template <>
void utils::create_ref_struct<roq::Start>(pybind11::module_ &module) {
  using value_type = roq::Start;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str()).def("__repr__", [](ref_type const &obj) {
    using namespace std::literals;
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}

template <>
void utils::create_ref_struct<roq::Stop>(pybind11::module_ &module) {
  using value_type = roq::Stop;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str()).def("__repr__", [](ref_type const &obj) {
    using namespace std::literals;
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}

template <>
void utils::create_ref_struct<roq::Timer>(pybind11::module_ &module) {
  using value_type = roq::Timer;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
      .def_property_readonly(
          "now",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.now;
          })
      .def("__repr__", [](ref_type const &obj) {
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::Connected>(pybind11::module_ &module) {
  using value_type = roq::Connected;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str()).def("__repr__", [](ref_type const &obj) {
    using namespace std::literals;
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}

template <>
void utils::create_ref_struct<roq::Disconnected>(pybind11::module_ &module) {
  using value_type = roq::Disconnected;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str()).def("__repr__", [](ref_type const &obj) {
    using namespace std::literals;
    auto &value = static_cast<const value_type &>(obj);
    return fmt::format("{}"sv, value);
  });
}

template <>
void utils::create_ref_struct<roq::DownloadBegin>(pybind11::module_ &module) {
  using value_type = roq::DownloadBegin;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def("__repr__", [](ref_type const &obj) {
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::DownloadEnd>(pybind11::module_ &module) {
  using value_type = roq::DownloadEnd;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::ExternalLatency>(pybind11::module_ &module) {
  using value_type = roq::ExternalLatency;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::GatewaySettings>(pybind11::module_ &module) {
  using value_type = roq::GatewaySettings;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::GatewayStatus>(pybind11::module_ &module) {
  using value_type = roq::GatewayStatus;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::MarketByPriceUpdate>(pybind11::module_ &module) {
  using value_type = roq::MarketByPriceUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::MarketByOrderUpdate>(pybind11::module_ &module) {
  using value_type = roq::MarketByOrderUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::MarketStatus>(pybind11::module_ &module) {
  using value_type = roq::MarketStatus;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::RateLimitTrigger>(pybind11::module_ &module) {
  using value_type = roq::RateLimitTrigger;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::ReferenceData>(pybind11::module_ &module) {
  using value_type = roq::ReferenceData;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::StatisticsUpdate>(pybind11::module_ &module) {
  using value_type = roq::StatisticsUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::StreamStatus>(pybind11::module_ &module) {
  using value_type = roq::StreamStatus;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::TopOfBook>(pybind11::module_ &module) {
  using value_type = roq::TopOfBook;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::TradeSummary>(pybind11::module_ &module) {
  using value_type = roq::TradeSummary;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

// ...

template <>
void utils::create_ref_struct<roq::CreateOrder>(pybind11::module_ &module) {
  using value_type = roq::CreateOrder;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
          "margin_mode",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.margin_mode;
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::ModifyOrder>(pybind11::module_ &module) {
  using value_type = roq::ModifyOrder;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::CancelOrder>(pybind11::module_ &module) {
  using value_type = roq::CancelOrder;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::CancelAllOrders>(pybind11::module_ &module) {
  using value_type = roq::CancelAllOrders;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
      .def_property_readonly(
          "account",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.account;
          })
      .def("__repr__", [](ref_type const &obj) {
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

// ...

template <>
void utils::create_ref_struct<roq::CancelAllOrdersAck>(pybind11::module_ &module) {
  using value_type = roq::CancelAllOrdersAck;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
          "round_trip_latency",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.round_trip_latency;
          })
      .def("__repr__", [](ref_type const &obj) {
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::OrderAck>(pybind11::module_ &module) {
  using value_type = roq::OrderAck;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::OrderUpdate>(pybind11::module_ &module) {
  using value_type = roq::OrderUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
          "margin_mode",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.margin_mode;
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::TradeUpdate>(pybind11::module_ &module) {
  using value_type = roq::TradeUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
          "margin_mode",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.margin_mode;
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::PositionUpdate>(pybind11::module_ &module) {
  using value_type = roq::PositionUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
          "margin_mode",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.margin_mode;
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

template <>
void utils::create_ref_struct<roq::FundsUpdate>(pybind11::module_ &module) {
  using value_type = roq::FundsUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
          "margin_mode",
          [](ref_type const &obj) {
            auto &value = static_cast<const value_type &>(obj);
            return value.margin_mode;
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

//

template <>
void utils::create_ref_struct<roq::CustomMetricsUpdate>(pybind11::module_ &module) {
  using value_type = roq::CustomMetricsUpdate;
  using ref_type = utils::Ref<value_type>;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<ref_type>(module, name.c_str())
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
        using namespace std::literals;
        auto &value = static_cast<const value_type &>(obj);
        return fmt::format("{}"sv, value);
      });
}

}  // namespace python
}  // namespace roq
