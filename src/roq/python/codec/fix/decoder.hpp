/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

#include "roq/codec/fix/decoder.hpp"

#include "roq/python/codec/fix/details.hpp"
#include "roq/python/codec/fix/header.hpp"

namespace roq {
namespace python {
namespace codec {
namespace fix {

struct Decoder final {
  template <typename Callback>
  struct Handler final : public roq::codec::fix::Decoder::Handler {
    explicit Handler(Callback const &callback) : callback_{callback} {}

    template <typename T>
    void dispatch(auto &header, auto &value) {
      Header header_2{header};
      T value_2{value};
      auto arg0 = pybind11::cast(header_2);
      auto arg1 = pybind11::cast(value_2);
      callback_(arg0, arg1);
    }

    void operator()(roq::fix::Header const &header, roq::codec::fix::Logon const &value) override {
      dispatch<Logon>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::Logout const &value) override {
      dispatch<Logout>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::TestRequest const &value) override {
      dispatch<TestRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::Heartbeat const &value) override {
      dispatch<Heartbeat>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::ResendRequest const &value) override {
      dispatch<ResendRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::Reject const &value) override {
      dispatch<Reject>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::BusinessMessageReject const &value) override {
      dispatch<BusinessMessageReject>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::UserRequest const &value) override {
      dispatch<UserRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::UserResponse const &value) override {
      dispatch<UserResponse>(header, value);
    }
    void operator()(
        roq::fix::Header const &header, roq::codec::fix::TradingSessionStatusRequest const &value) override {
      dispatch<TradingSessionStatusRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::TradingSessionStatus const &value) override {
      dispatch<TradingSessionStatus>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityListRequest const &value) override {
      dispatch<SecurityListRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityList const &value) override {
      dispatch<SecurityList>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityDefinitionRequest const &value) override {
      dispatch<SecurityDefinitionRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityDefinition const &value) override {
      dispatch<SecurityDefinition>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityStatusRequest const &value) override {
      dispatch<SecurityStatusRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityStatus const &value) override {
      dispatch<SecurityStatus>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::MarketDataRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::MarketDataRequestReject const &value) override {}
    void operator()(
        roq::fix::Header const &header, roq::codec::fix::MarketDataSnapshotFullRefresh const &value) override {}
    void operator()(
        roq::fix::Header const &header, roq::codec::fix::MarketDataIncrementalRefresh const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderStatusRequest const &value) override {
      dispatch<OrderStatusRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderMassStatusRequest const &value) override {
      dispatch<OrderMassStatusRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::NewOrderSingle const &value) override {
      dispatch<NewOrderSingle>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderCancelRequest const &value) override {
      dispatch<OrderCancelRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderCancelReplaceRequest const &value) override {
      dispatch<OrderCancelReplaceRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderMassCancelRequest const &value) override {
      dispatch<OrderMassCancelRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderCancelReject const &value) override {
      dispatch<OrderCancelReject>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderMassCancelReport const &value) override {
      dispatch<OrderMassCancelReport>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::ExecutionReport const &value) override {
      dispatch<ExecutionReport>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::TradeCaptureReportRequest const &value) override {
      dispatch<TradeCaptureReportRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::TradeCaptureReport const &value) override {
      dispatch<TradeCaptureReport>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::RequestForPositions const &value) override {
      dispatch<RequestForPositions>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::RequestForPositionsAck const &value) override {
      dispatch<RequestForPositionsAck>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::PositionReport const &value) override {}

   private:
    Callback const &callback_;
  };

  Decoder() : decoder_{roq::codec::fix::Decoder::create()} {}

  template <typename Callback>
  size_t dispatch(Callback const &callback, std::string_view const &message) {
    size_t result = {};
    try {
      Handler handler{callback};
      std::span buffer{reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)};
      result = (*decoder_)(handler, buffer);
    } catch (pybind11::error_already_set &) {
      /*
      log::warn("caught exception!"sv);
      return false;  // break
      */
      throw;
    } catch (...) {
      using namespace std::literals;
      fmt::print(stderr, "HERE\n"sv);
      throw;
    }
    return result;
  }

  // XXX HANS tuple

 private:
  std::unique_ptr<roq::codec::fix::Decoder> decoder_;
};

}  // namespace fix
}  // namespace codec
}  // namespace python
}  // namespace roq
