/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <pybind11/pybind11.h>

#include <map>
#include <set>

// #include <absl/flags/parse.h>  // XXX shouldn't be here...

#include "roq/logging.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/client.hpp"

#include "roq/client/simple.hpp"

#include "roq/python/utils.hpp"

namespace roq {
namespace python {
namespace client {

struct Settings2 final : public roq::client::Settings2 {
  Settings2(
      [[maybe_unused]] pybind11::object app,
      [[maybe_unused]] pybind11::object loop,
      [[maybe_unused]] pybind11::object service,
      [[maybe_unused]] pybind11::object common) {}
};

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
  virtual void callback(pybind11::object const &, pybind11::object const &) = 0;
};

// trampoline
struct PyHandler : public Handler {
  using Handler::Handler;

  void callback(pybind11::object const &message_info, pybind11::object const &value) override {
    PYBIND11_OVERRIDE_PURE(void, Handler, callback, message_info, value);
  }
};

namespace {
struct Bridge final : public roq::client::Handler {
  explicit Bridge(roq::client::Dispatcher &, python::client::Handler &handler) : handler_{handler} {}

 protected:
  template <typename T>
  void dispatch(auto const &message_info, T const &value) {
    auto arg0 = pybind11::cast(utils::Ref<MessageInfo>{message_info});
    auto arg1 = pybind11::cast(utils::Ref<T>{value});
    handler_.callback(arg0, arg1);
    if (arg0.ref_count() > 1 || arg1.ref_count() > 1) {
      using namespace std::literals;
      throw std::runtime_error{"Objects must not be stored"s};
    }
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
  void operator()(Event<roq::RateLimitsUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::RateLimitTrigger> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::GatewayStatus> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::ReferenceData> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketStatus> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::TopOfBook> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketByPriceUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketByOrderUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::TradeSummary> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::StatisticsUpdate> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::CancelAllOrdersAck> const &event) override { dispatch(event.message_info, event.value); }
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

struct Bridge2 final : public roq::client::Simple::Handler {
  explicit Bridge2(python::client::Handler &handler) : handler_{handler} {}

 protected:
  template <typename T>
  void dispatch(auto const &message_info, T const &value) {
    auto arg0 = pybind11::cast(utils::Ref<MessageInfo>{message_info});
    auto arg1 = pybind11::cast(utils::Ref<T>{value});
    handler_.callback(arg0, arg1);
    if (arg0.ref_count() > 1 || arg1.ref_count() > 1) {
      using namespace std::literals;
      throw std::runtime_error{"Objects must not be stored"s};
    }
  }

 protected:
  void operator()(Event<roq::Start> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::Stop> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::Connected> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::Disconnected> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::DownloadBegin> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::DownloadEnd> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::GatewaySettings> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::StreamStatus> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::ExternalLatency> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::RateLimitsUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::RateLimitTrigger> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::GatewayStatus> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::ReferenceData> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketStatus> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::TopOfBook> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketByPriceUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::MarketByOrderUpdate> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::TradeSummary> const &event) override { dispatch(event.message_info, event.value); }
  void operator()(Event<roq::StatisticsUpdate> const &event) override { dispatch(event.message_info, event.value); }

  void operator()(Event<roq::CancelAllOrdersAck> const &event) override { dispatch(event.message_info, event.value); }
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

struct Dispatcher final {
  Dispatcher(
      python::client::Settings2 const &settings,
      python::client::Config const &config,
      std::vector<std::string> const &connections)
      : settings_{create_settings(settings)}, config_{config}, connections_{connections},
        context_{roq::io::engine::ContextFactory::create("libevent")},
        dispatcher_{create_dispatcher(settings_, config, *context_, connections)} {}

 protected:
  static roq::client::Settings2 create_settings(roq::client::Settings2 const &) {
    roq::client::Settings2 result;
    result.app.name = "trader";
    result.loop.timer_freq = std::chrono::milliseconds{100};
    return result;
  }

  static std::unique_ptr<roq::client::Simple> create_dispatcher(
      roq::client::Settings2 const &settings,
      roq::client::Config const &config,
      roq::io::Context &context,
      std::vector<std::string> const &connections) {
    std::vector<std::string_view> tmp;
    for (auto &item : connections)
      tmp.emplace_back(item);
    log::info("settings={}", settings);
    log::info("connections={}", connections);
    return roq::client::Simple::create(settings, config, context, tmp);
  }

  template <typename T>
  void dispatch(auto const &message_info, T const &value) {
    auto arg0 = pybind11::cast(utils::Ref<MessageInfo>{message_info});
    auto arg1 = pybind11::cast(utils::Ref<T>{value});
    handler_(arg0, arg1);
    if (arg0.ref_count() > 1 || arg1.ref_count() > 1) {
      using namespace std::literals;
      throw std::runtime_error{"Objects must not be stored"s};
    }
  }

 public:
  void start() { (*dispatcher_).start(); }

  void stop() { (*dispatcher_).stop(); }

  bool dispatch(pybind11::object handler) {
    try {
      Bridge2 bridge{pybind11::cast<python::client::Handler &>(handler)};
      return (*dispatcher_).dispatch(bridge);
    } catch (pybind11::error_already_set &) {
      throw;
    }
    return false;
  }
  void send(roq::CreateOrder const &create_order, uint8_t source) { (*dispatcher_).send(create_order, source); }
  void send(roq::ModifyOrder const &modify_order, uint8_t source) { (*dispatcher_).send(modify_order, source); }
  void send(roq::CancelOrder const &cancel_order, uint8_t source) { (*dispatcher_).send(cancel_order, source); }
  void send(roq::CancelAllOrders const &cancel_all_orders, uint8_t source) {
    (*dispatcher_).send(cancel_all_orders, source);
  }

 private:
  roq::client::Settings2 const settings_;  // XXX TODO proper
  python::client::Config const config_;
  std::vector<std::string> const connections_;
  std::unique_ptr<roq::io::Context> context_;
  std::unique_ptr<roq::client::Simple> dispatcher_;
};

struct EventLogReader final {
  template <typename Callback>
  struct Handler final : public roq::client::EventLogReader::Handler {
    explicit Handler(Callback const &callback) : callback_{callback} {}

   protected:
    template <typename T>
    void dispatch(auto const &message_info, T const &value) {
      auto arg0 = pybind11::cast(utils::Ref<MessageInfo>{message_info});
      auto arg1 = pybind11::cast(utils::Ref<T>{value});
      callback_(arg0, arg1);
      if (arg0.ref_count() > 1 || arg1.ref_count() > 1) {
        using namespace std::literals;
        throw std::runtime_error{"Objects must not be stored"s};
      }
    }

   protected:
    void operator()(Event<roq::GatewaySettings> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::StreamStatus> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::ExternalLatency> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::RateLimitsUpdate> const &event) override { dispatch(event.message_info, event.value); }
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

    void operator()(Event<roq::CancelAllOrdersAck> const &event) override { dispatch(event.message_info, event.value); }
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
    } catch (pybind11::error_already_set &) {
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
      auto arg0 = pybind11::cast(utils::Ref<MessageInfo>{message_info});
      auto arg1 = pybind11::cast(utils::Ref<T>{value});
      callback_(arg0, arg1);
      if (arg0.ref_count() > 1 || arg1.ref_count() > 1) {
        using namespace std::literals;
        throw std::runtime_error{"Objects must not be stored"s};
      }
    }

   protected:
    void operator()(Event<roq::GatewaySettings> const &event) override { dispatch(event.message_info, event.value); }

    void operator()(Event<roq::StreamStatus> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::ExternalLatency> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<roq::RateLimitsUpdate> const &event) override { dispatch(event.message_info, event.value); }
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

    void operator()(Event<roq::CancelAllOrdersAck> const &event) override { dispatch(event.message_info, event.value); }
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
    } catch (pybind11::error_already_set &) {
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
}  // namespace python
}  // namespace roq
