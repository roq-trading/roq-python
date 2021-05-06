#include <pybind11/pybind11.h>

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <magic_enum.hpp>

#include "roq/client.hpp"

#include "roq/logging.hpp"

using namespace std::literals;
using namespace std::chrono_literals;

namespace py = pybind11;  // convention

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace roq {
namespace python {
template <typename T>
void create_enum(auto &context, const std::string &name) {
  pybind11::enum_<T> enum_(context, name.c_str());
  for (auto item : magic_enum::enum_values<T>()) {
    std::string name{magic_enum::enum_name(item)};
    enum_.value(name.c_str(), item);
  }
}
// helpers
// note! they all use copies
void create_fill(auto &context) {
  py::class_<roq::Fill>(context, "Fill")
      .def_property_readonly("external_trade_id", [](roq::Fill const *obj) { return (*obj).external_trade_id; })
      .def_property_readonly("quantity", [](roq::Fill const *obj) { return (*obj).quantity; })
      .def_property_readonly("price", [](roq::Fill const *obj) { return (*obj).price; })
      .def_property_readonly("liquidity", [](roq::Fill const *obj) { return (*obj).liquidity; })
      .def("__repr__", [](const roq::Fill &obj) { return fmt::format("{}"sv, obj); });
}
void create_layer(auto &context) {
  py::class_<roq::Layer>(context, "Layer")
      .def_property_readonly("bid_price", [](roq::Layer const *obj) { return (*obj).bid_price; })
      .def_property_readonly("bid_quantity", [](roq::Layer const *obj) { return (*obj).bid_quantity; })
      .def_property_readonly("ask_price", [](roq::Layer const *obj) { return (*obj).ask_price; })
      .def_property_readonly("ask_quantity", [](roq::Layer const *obj) { return (*obj).ask_quantity; })
      .def("__repr__", [](const roq::Layer &obj) { return fmt::format("{}"sv, obj); });
}
void create_mbp_update(auto &context) {
  py::class_<roq::MBPUpdate>(context, "MBPUpdate")
      .def_property_readonly("price", [](roq::MBPUpdate const *obj) { return (*obj).price; })
      .def_property_readonly("quantity", [](roq::MBPUpdate const *obj) { return (*obj).quantity; })
      .def_property_readonly("implied_quantity", [](roq::MBPUpdate const *obj) { return (*obj).implied_quantity; })
      .def_property_readonly("price_level", [](roq::MBPUpdate const *obj) { return (*obj).price_level; })
      .def_property_readonly("number_of_orders", [](roq::MBPUpdate const *obj) { return (*obj).number_of_orders; })
      .def("__repr__", [](const MBPUpdate &obj) { return fmt::format("{}"sv, obj); });
}
void create_measurement(auto &context) {
  py::class_<roq::Measurement>(context, "Measurement")
      .def_property_readonly("name", [](roq::Measurement const *obj) { return (*obj).name; })
      .def_property_readonly("value", [](roq::Measurement const *obj) { return (*obj).value; })
      .def("__repr__", [](const Measurement &obj) { return fmt::format("{}"sv, obj); });
}
void create_statistics(auto &context) {
  py::class_<roq::Statistics>(context, "Statistics")
      .def_property_readonly("type", [](roq::Statistics const *obj) { return (*obj).type; })
      .def_property_readonly("value", [](roq::Statistics const *obj) { return (*obj).value; })
      .def_property_readonly("begin_time_utc", [](roq::Statistics const *obj) { return (*obj).begin_time_utc; })
      .def_property_readonly("end_time_utc", [](roq::Statistics const *obj) { return (*obj).end_time_utc; })
      .def("__repr__", [](const roq::Statistics &obj) { return fmt::format("{}"sv, obj); });
}
void create_trade(auto &context) {
  py::class_<roq::Trade>(context, "Trade")
      .def_property_readonly("side", [](roq::Trade const *obj) { return (*obj).side; })
      .def_property_readonly("price", [](roq::Trade const *obj) { return (*obj).price; })
      .def_property_readonly("quantity", [](roq::Trade const *obj) { return (*obj).quantity; })
      .def_property_readonly("trade_id", [](roq::Trade const *obj) { return (*obj).trade_id; })
      .def("__repr__", [](const roq::Trade &obj) { return fmt::format("{}"sv, obj); });
}
void create_uuid(auto &context) {
  py::class_<roq::UUID>(context, "UUID").def("__repr__", [](const roq::UUID &obj) { return fmt::format("{}"sv, obj); });
}
// transport
struct MessageInfo final {
  MessageInfo() = delete;
  explicit MessageInfo(const roq::MessageInfo &message_info) : message_info(message_info) {}

  const roq::MessageInfo &message_info;
};
void create_message_info(auto &context) {
  py::class_<MessageInfo>(context, "MessageInfo")
      .def_property_readonly("source", [](MessageInfo const *obj) { return (*obj).message_info.source; })
      .def_property_readonly("source_name", [](MessageInfo const *obj) { return (*obj).message_info.source_name; })
      .def_property_readonly(
          "source_session_id", [](MessageInfo const *obj) { return (*obj).message_info.source_session_id; })
      .def_property_readonly("source_seqno", [](MessageInfo const *obj) { return (*obj).message_info.source_seqno; })
      .def_property_readonly(
          "receive_time_utc", [](MessageInfo const *obj) { return (*obj).message_info.receive_time_utc; })
      .def_property_readonly("receive_time", [](MessageInfo const *obj) { return (*obj).message_info.receive_time; })
      .def_property_readonly(
          "source_send_time", [](MessageInfo const *obj) { return (*obj).message_info.source_send_time; })
      .def_property_readonly(
          "source_receive_time", [](MessageInfo const *obj) { return (*obj).message_info.source_receive_time; })
      .def_property_readonly(
          "origin_create_time", [](MessageInfo const *obj) { return (*obj).message_info.origin_create_time; })
      .def_property_readonly(
          "origin_create_time_utc", [](MessageInfo const *obj) { return (*obj).message_info.origin_create_time_utc; })
      .def_property_readonly("is_last", [](MessageInfo const *obj) { return (*obj).message_info.is_last; })
      .def_property_readonly("opaque", [](MessageInfo const *obj) { return (*obj).message_info.opaque; })
      .def("__repr__", [](const MessageInfo &obj) { return fmt::format("{}"sv, obj.message_info); });
}
// messages
struct ReferenceData final {
  ReferenceData() = delete;
  explicit ReferenceData(const roq::ReferenceData &reference_data) : reference_data(reference_data) {}

  const roq::ReferenceData &reference_data;
};
void create_reference_data(auto &context) {
  py::class_<ReferenceData>(context, "ReferenceData")
      .def_property_readonly("exchange", [](ReferenceData const *obj) { return (*obj).reference_data.exchange; })
      .def_property_readonly("symbol", [](ReferenceData const *obj) { return (*obj).reference_data.symbol; })
      .def_property_readonly("description", [](ReferenceData const *obj) { return (*obj).reference_data.description; })
      .def_property_readonly(
          "security_type", [](ReferenceData const *obj) { return (*obj).reference_data.security_type; })
      .def_property_readonly(
          "base_currency", [](ReferenceData const *obj) { return (*obj).reference_data.base_currency; })
      .def_property_readonly(
          "quote_currency", [](ReferenceData const *obj) { return (*obj).reference_data.quote_currency; })
      .def_property_readonly(
          "margin_currency", [](ReferenceData const *obj) { return (*obj).reference_data.margin_currency; })
      .def_property_readonly(
          "commission_currency", [](ReferenceData const *obj) { return (*obj).reference_data.commission_currency; })
      .def_property_readonly("tick_size", [](ReferenceData const *obj) { return (*obj).reference_data.tick_size; })
      .def_property_readonly("multiplier", [](ReferenceData const *obj) { return (*obj).reference_data.multiplier; })
      .def_property_readonly(
          "min_trade_vol", [](ReferenceData const *obj) { return (*obj).reference_data.min_trade_vol; })
      .def_property_readonly(
          "max_trade_vol", [](ReferenceData const *obj) { return (*obj).reference_data.max_trade_vol; })
      .def_property_readonly(
          "trade_vol_step_size", [](ReferenceData const *obj) { return (*obj).reference_data.trade_vol_step_size; })
      .def_property_readonly("option_type", [](ReferenceData const *obj) { return (*obj).reference_data.option_type; })
      .def_property_readonly(
          "strike_currency", [](ReferenceData const *obj) { return (*obj).reference_data.strike_currency; })
      .def_property_readonly(
          "strike_price", [](ReferenceData const *obj) { return (*obj).reference_data.strike_price; })
      .def_property_readonly("underlying", [](ReferenceData const *obj) { return (*obj).reference_data.underlying; })
      .def_property_readonly("time_zone", [](ReferenceData const *obj) { return (*obj).reference_data.time_zone; })
      .def_property_readonly("issue_date", [](ReferenceData const *obj) { return (*obj).reference_data.issue_date; })
      .def_property_readonly(
          "settlement_date", [](ReferenceData const *obj) { return (*obj).reference_data.settlement_date; })
      .def_property_readonly(
          "expiry_datetime", [](ReferenceData const *obj) { return (*obj).reference_data.expiry_datetime; })
      .def_property_readonly(
          "expiry_datetime_utc", [](ReferenceData const *obj) { return (*obj).reference_data.expiry_datetime_utc; })
      .def("__repr__", [](const ReferenceData &obj) { return fmt::format("{}"sv, obj.reference_data); });
}
struct TopOfBook final {
  TopOfBook() = delete;
  explicit TopOfBook(const roq::TopOfBook &top_of_book) : top_of_book(top_of_book) {}

  const roq::TopOfBook &top_of_book;
};
void create_top_of_book(auto &context) {
  py::class_<TopOfBook>(context, "TopOfBook")
      .def_property_readonly("exchange", [](TopOfBook const *obj) { return (*obj).top_of_book.exchange; })
      .def_property_readonly("symbol", [](TopOfBook const *obj) { return (*obj).top_of_book.symbol; })
      .def_property_readonly("bid_price", [](TopOfBook const *obj) { return (*obj).top_of_book.layer.bid_price; })
      .def_property_readonly("bid_quantity", [](TopOfBook const *obj) { return (*obj).top_of_book.layer.bid_quantity; })
      .def_property_readonly("ask_price", [](TopOfBook const *obj) { return (*obj).top_of_book.layer.ask_price; })
      .def_property_readonly("ask_quantity", [](TopOfBook const *obj) { return (*obj).top_of_book.layer.ask_quantity; })
      .def_property_readonly("update_type", [](TopOfBook const *obj) { return (*obj).top_of_book.update_type; })
      .def_property_readonly(
          "exchange_time_utc", [](TopOfBook const *obj) { return (*obj).top_of_book.exchange_time_utc; })
      .def("__repr__", [](const TopOfBook &obj) { return fmt::format("{}"sv, obj.top_of_book); });
}
struct MarketByPriceUpdate final {
  MarketByPriceUpdate() = delete;
  explicit MarketByPriceUpdate(const roq::MarketByPriceUpdate &market_by_price_update)
      : market_by_price_update(market_by_price_update) {}

  // XXX check if std::span works
  py::list get_bids() const {
    py::list result;
    for (auto &item : market_by_price_update.bids)
      result.append(MBPUpdate{item});
    return result;
  }

  py::list get_asks() const {
    py::list result;
    for (auto &item : market_by_price_update.asks)
      result.append(MBPUpdate{item});
    return result;
  }

  const roq::MarketByPriceUpdate &market_by_price_update;
};
void create_market_by_price_update(auto &context) {
  py::class_<MarketByPriceUpdate>(context, "MarketByPriceUpdate")
      .def_property_readonly(
          "exchange", [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.exchange; })
      .def_property_readonly(
          "symbol", [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.symbol; })
      .def_property_readonly("bids", [](MarketByPriceUpdate const *obj) { return (*obj).get_bids(); })
      .def_property_readonly("asks", [](MarketByPriceUpdate const *obj) { return (*obj).get_asks(); })
      .def_property_readonly(
          "update_type", [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.update_type; })
      .def_property_readonly(
          "exchange_time_utc",
          [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.exchange_time_utc; })
      .def_property_readonly(
          "exchange_sequence",
          [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.exchange_sequence; })
      .def_property_readonly(
          "price_decimals", [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.price_decimals; })
      .def_property_readonly(
          "quantity_decimals",
          [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.quantity_decimals; })
      .def_property_readonly(
          "max_depth", [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.max_depth; })
      .def_property_readonly(
          "checksum", [](MarketByPriceUpdate const *obj) { return (*obj).market_by_price_update.checksum; })
      .def("__repr__", [](const MarketByPriceUpdate &obj) { return fmt::format("{}"sv, obj.market_by_price_update); });
}
namespace client {
void create_settings(auto &context) {
  py::class_<roq::client::Settings>(context, "Settings")
      .def(
          py::init<OrderCancelPolicy, OrderManagement>(),
          py::arg("order_cancel_policy") = OrderCancelPolicy::UNDEFINED,
          py::arg("order_management") = OrderManagement::UNDEFINED)
      .def_readonly("order_cancel_policy", &roq::client::Settings::order_cancel_policy)
      .def_readonly("order_management", &roq::client::Settings::order_management)
      .def("__repr__", [](const roq::client::Settings &settings) { return fmt::format("{}", settings); });
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
void create_config(auto &context) {
  py::class_<Config>(context, "Config")
      .def(
          py::init<roq::client::Settings, std::set<std::string>, std::map<std::string, std::set<std::string>>>(),
          py::arg("settings"),
          py::arg("accounts"),
          py::arg("symbols"));
}
struct Manager final {
  Manager(const Config &, [[maybe_unused]] const std::vector<std::string> &connections) {}
  bool dispatch(const std::function<void(py::object, py::object)> &callback) {
    try {
      // XXX exception must break the dispatch loop
      {
        roq::MessageInfo message_info;
        roq::ReferenceData reference_data;
        callback(py::cast(MessageInfo{message_info}), py::cast(ReferenceData{reference_data}));
      }
      {
        roq::MessageInfo message_info;
        roq::TopOfBook top_of_book;
        callback(py::cast(MessageInfo{message_info}), py::cast(TopOfBook{top_of_book}));
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
void create_manager(auto &context) {
  py::class_<Manager>(context, "Manager")
      .def(py::init<const Config &, std::vector<std::string>>(), py::arg("config"), py::arg("connections"))
      .def(
          "dispatch",
          [](Manager *obj, std::function<void(py::object, py::object)> &callback) { return obj->dispatch(callback); },
          py::arg("value"));
}
struct EventLogReader final {
  struct Handler final : public roq::client::EventLogReader::Handler {
    explicit Handler(const std::function<void(py::object, py::object)> &callback) : callback_(callback) {}

   protected:
    template <typename T>
    void dispatch(const auto &message_info, const auto &value) {
      auto arg0 = py::cast(MessageInfo{message_info});
      auto arg1 = py::cast(T{value});
      callback_(arg0, arg1);
      if (arg0.ref_count() > 1 || arg1.ref_count() > 1)
        throw std::runtime_error("Objects must not be stored"s);
    }

   protected:
    void operator()(const Event<roq::GatewaySettings> &) override {}

    void operator()(const Event<roq::StreamStatus> &) override {}
    void operator()(const Event<roq::ExternalLatency> &) override {}
    void operator()(const Event<roq::RateLimitTrigger> &) override {}

    void operator()(const Event<roq::GatewayStatus> &) override {}

    void operator()(const Event<roq::ReferenceData> &event) override {
      auto &[message_info, reference_data] = event;
      dispatch<ReferenceData>(message_info, reference_data);
    }
    void operator()(const Event<roq::MarketStatus> &) override {}
    void operator()(const Event<roq::TopOfBook> &event) override {
      auto &[message_info, top_of_book] = event;
      dispatch<TopOfBook>(message_info, top_of_book);
    }
    void operator()(const Event<roq::MarketByPriceUpdate> &event) override {
      auto &[message_info, market_by_price_update] = event;
      dispatch<MarketByPriceUpdate>(message_info, market_by_price_update);
    }
    void operator()(const Event<roq::MarketByOrderUpdate> &) override {}
    void operator()(const Event<roq::TradeSummary> &) override {}
    void operator()(const Event<roq::StatisticsUpdate> &) override {}

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
    const std::function<void(py::object, py::object)> &callback_;
  };
  EventLogReader(const std::string_view &path, size_t buffer_size)
      : reader_(roq::client::EventLogReaderFactory::create(path, buffer_size)) {}

  bool dispatch(const std::function<void(py::object, py::object)> &callback) {
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
void create_event_log_reader(auto &context) {
  py::class_<EventLogReader>(context, "EventLogReader")
      .def(py::init<const std::string_view &, size_t>(), py::arg("path"), py::arg("buffer_size") = 0)
      .def(
          "dispatch",
          [](EventLogReader *obj, std::function<void(py::object, py::object)> &callback) {
            return obj->dispatch(callback);
          },
          py::arg("value"));
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

  py::object extract(size_t depth) const {
    std::vector<Layer> tmp(depth);
    (*market_by_price_).extract(tmp);
    py::list result;
    for (auto &item : tmp)
      result.append(Layer{item});
    return result;
  }

 private:
  std::unique_ptr<roq::cache::MarketByPrice> market_by_price_;
};
void create_market_by_price(auto &context) {
  py::class_<MarketByPrice>(context, "MarketByPrice")
      .def(py::init<const std::string_view &, const std::string_view &>(), py::arg("exchange") = "", py::arg("symbol"))
      .def(
          "apply",
          [](MarketByPrice *obj, const MarketByPriceUpdate &market_by_price_update) {
            (*obj)(market_by_price_update.market_by_price_update);
          })
      .def("extract", [](MarketByPrice const *obj, size_t depth) { return (*obj).extract(depth); });
}
}  // namespace cache
}  // namespace python
}  // namespace roq

PYBIND11_MODULE(roq, m) {
  m.doc() = "Roq Python Tools";

  // enums

  roq::python::create_enum<roq::ConnectionStatus>(m, "ConnectionStatus"s);
  roq::python::create_enum<roq::Decimals>(m, "Decimals"s);
  roq::python::create_enum<roq::Error>(m, "Error"s);
  roq::python::create_enum<roq::ExecutionInstruction>(m, "ExecutionInstruction"s);
  roq::python::create_enum<roq::Liquidity>(m, "Liquidity"s);
  roq::python::create_enum<roq::OptionType>(m, "OptionType"s);
  roq::python::create_enum<roq::OrderCancelPolicy>(m, "OrderCancelPolicy"s);
  roq::python::create_enum<roq::OrderManagement>(m, "OrderManagement"s);
  roq::python::create_enum<roq::OrderStatus>(m, "OrderStatus"s);
  roq::python::create_enum<roq::OrderType>(m, "OrderType"s);
  roq::python::create_enum<roq::OrderUpdateAction>(m, "OrderUpdateAction"s);
  roq::python::create_enum<roq::Origin>(m, "Origin"s);
  roq::python::create_enum<roq::PositionEffect>(m, "PositionEffect"s);
  roq::python::create_enum<roq::Priority>(m, "Priority"s);
  roq::python::create_enum<roq::RateLimitType>(m, "RateLimitType"s);
  roq::python::create_enum<roq::RequestIdType>(m, "RequestIdType"s);
  roq::python::create_enum<roq::RequestStatus>(m, "RequestStatus"s);
  roq::python::create_enum<roq::RequestType>(m, "RequestType"s);
  roq::python::create_enum<roq::SecurityType>(m, "SecurityType"s);
  roq::python::create_enum<roq::Side>(m, "Side"s);
  roq::python::create_enum<roq::StatisticsType>(m, "StatisticsType"s);
  roq::python::create_enum<roq::StreamType>(m, "StreamType"s);
  roq::python::create_enum<roq::SupportType>(m, "SupportType"s);
  roq::python::create_enum<roq::TimeInForce>(m, "TimeInForce"s);
  roq::python::create_enum<roq::TradingStatus>(m, "TradingStatus"s);
  roq::python::create_enum<roq::UpdateType>(m, "UpdateType"s);

  // helpers

  roq::python::create_fill(m);
  roq::python::create_layer(m);
  roq::python::create_mbp_update(m);
  roq::python::create_measurement(m);
  roq::python::create_statistics(m);
  roq::python::create_trade(m);
  roq::python::create_uuid(m);

  // transport

  roq::python::create_message_info(m);

  // struct

  roq::python::create_reference_data(m);
  roq::python::create_top_of_book(m);
  roq::python::create_market_by_price_update(m);

  auto client = m.def_submodule("client");

  roq::python::client::create_settings(client);
  roq::python::client::create_config(client);
  roq::python::client::create_manager(client);

  roq::python::client::create_event_log_reader(client);

  auto cache = m.def_submodule("cache");

  roq::python::cache::create_market_by_price(cache);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "head";
#endif
}
