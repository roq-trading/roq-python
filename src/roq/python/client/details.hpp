/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <pybind11/pybind11.h>

#include <absl/flags/parse.h>  // XXX shouldn't be here...

#include "roq/logging.hpp"

#include "roq/client.hpp"

#include "roq/python/utils.hpp"

namespace roq {
namespace python {
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
  Manager(pybind11::object handler, python::client::Config const &config, std::vector<std::string> const &connections)
      : config_{config}, connections_{connections}, handler_{handler(123)} {
    pybind11::cast<python::client::Handler &>(handler_);  // will throw exception if not inherited from
  }

 protected:
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
  bool dispatch() {
    // XXX we need an external loop here
    // XXX for now, only exceptions can break the dispatch loop
    try {
      std::vector<std::string_view> connections;
      for (auto &connection : connections_)
        connections.emplace_back(connection);
      auto &handler = pybind11::cast<python::client::Handler &>(handler_);
      roq::client::Settings2 settings;  // XXX TODO proper
      roq::client::Trader(settings, config_, std::span{connections}).dispatch<Bridge>(handler);
    } catch (pybind11::error_already_set &) {
      /*
      log::warn("caught exception!"sv);
      return true;  // break
      */
      throw;
    }
    return false;
  }
  void send(roq::CreateOrder const &, [[maybe_unused]] uint8_t source) {
    using namespace std::literals;
    log::info("HERE"sv);
    // XXX send
  }
  void send(roq::ModifyOrder const &, [[maybe_unused]] uint8_t source) {
    using namespace std::literals;
    log::info("HERE"sv);
    // XXX send
  }
  void send(roq::CancelOrder const &, [[maybe_unused]] uint8_t source) {
    using namespace std::literals;
    log::info("HERE"sv);
    // XXX send
  }
  void send(roq::CancelAllOrders const &, [[maybe_unused]] uint8_t source) {
    using namespace std::literals;
    log::info("HERE"sv);
    // XXX send
  }

 private:
  python::client::Config const config_;
  std::vector<std::string> const connections_;
  pybind11::object handler_;
};

inline void set_flags(pybind11::dict const &key_value_pairs) {
  // buffer (represents the command-line)
  std::vector<char> buffer;
  // - program
  buffer.emplace_back('\0');
  for (auto &[key, value] : key_value_pairs) {
    // - key
    auto k = pybind11::cast<std::string>(key);
    buffer.emplace_back('-');
    buffer.emplace_back('-');
    std::for_each(std::begin(k), std::end(k), [&](auto c) { buffer.emplace_back(c); });
    buffer.emplace_back('\0');
    // - value
    auto v = pybind11::cast<std::string>(value);
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
