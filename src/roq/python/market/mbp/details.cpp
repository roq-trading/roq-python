/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/market/mbp/details.hpp"

#include <pybind11/pybind11.h>

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/codec/udp/header.hpp"

#include "roq/python/codec/udp/details.hpp"

using namespace std::literals;

namespace roq {
namespace python {

namespace {
auto use_snapshot(auto &header, auto &sequencer) {
  if (sequencer.ready()) {
    return roq::codec::udp::is_incremental(header);
  } else {
    return true;  // note! we're happy with a snapshot from either of incremental or snapshot
  }
}

void sequence_helper(auto &sequencer, auto &market_by_price_update, auto &header, auto &callback, auto &reset) {
  auto create = [&](auto &bids, auto &asks, auto update_type) {
    auto result = market_by_price_update;
    result.bids = bids;
    result.asks = asks;
    result.update_type = update_type;
    return result;
  };
  auto dispatch = [&](auto &mbp_update) {
    auto arg0 = pybind11::cast(utils::Ref{mbp_update});
    callback(arg0);
    if (arg0.ref_count() > 1) {
      throw std::runtime_error{"Objects must not be stored"s};
    }
  };
  auto publish_update = [&](auto &bids, auto &asks) {
    auto mbp_update = create(bids, asks, UpdateType::INCREMENTAL);
    dispatch(mbp_update);
  };
  auto publish_snapshot = [&](auto &bids,
                              auto &asks,
                              [[maybe_unused]] auto sequence,
                              [[maybe_unused]] auto retries,
                              [[maybe_unused]] auto delay) {
    auto mbp_update = create(bids, asks, UpdateType::SNAPSHOT);
    dispatch(mbp_update);
  };
  auto request_snapshot = [&](auto retries) { reset(retries); };
  switch (market_by_price_update.update_type) {
    using enum UpdateType;
    case UNDEFINED:
      assert(false);
      break;
    case SNAPSHOT:
      if (use_snapshot(header, sequencer)) {
        sequencer(
            market_by_price_update.bids,
            market_by_price_update.asks,
            header.last_sequence_number,
            true,  // note! use the snapshot even when we don't have any incremental history
            publish_snapshot,
            request_snapshot);
      }
      break;
    case INCREMENTAL:
      sequencer(
          market_by_price_update.bids,
          market_by_price_update.asks,
          header.sequence_number,
          header.sequence_number,
          header.last_sequence_number,
          publish_update,
          publish_snapshot,
          request_snapshot);
      break;
    case STALE:
      // (*market_by_price_).clear();
      break;
  }
}
}  // namespace

template <>
void utils::create_struct<market::mbp::MarketByPrice>(pybind11::module_ &module) {
  using value_type = market::mbp::MarketByPrice;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str(), "Maintains order book (by price) state")
      .def(
          pybind11::init<std::string_view const &, std::string_view const &>(),
          pybind11::arg("exchange") = "",
          pybind11::arg("symbol"))
      .def(
          "clear", [](value_type &self) { self.clear(); }, "Reset state")
      .def(
          "apply",
          [](value_type &self, utils::Ref<MarketByPriceUpdate> const &market_by_price_update) {
            self(market_by_price_update);
          },
          "Apply an update")
      .def(
          "extract",
          [](value_type const &self, size_t depth) { return self.extract(depth); },
          "Extract best levels from book")
      .def("__repr__", [](value_type const &self) { return self.print(); });
}

namespace {
auto create_sequencer(auto timeout) {
  auto options = roq::market::mbp::Sequencer::Options{
      .timeout = timeout,
      .max_updates = {},
  };
  return roq::market::mbp::Sequencer{options};
}
}  // namespace

market::mbp::Sequencer::Sequencer(std::chrono::milliseconds timeout) : sequencer{create_sequencer(timeout)} {
}

template <>
void utils::create_struct<market::mbp::Sequencer>(pybind11::module_ &module) {
  using value_type = market::mbp::Sequencer;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<std::chrono::milliseconds>(), pybind11::arg("timeout") = std::chrono::milliseconds{})
      .def(
          "apply",
          [](value_type &self,
             utils::Ref<MarketByPriceUpdate> const &market_by_price_update,
             roq::python::codec::udp::Header const &header,
             std::function<void(pybind11::object const &)> const &callback,
             std::function<void(size_t)> const &reset) {
            sequence_helper(
                self.sequencer,
                static_cast<roq::MarketByPriceUpdate const &>(market_by_price_update),
                static_cast<roq::codec::udp::Header const &>(header),
                callback,
                reset);
          },
          pybind11::arg("market_by_price_update"),
          pybind11::arg("header"),
          pybind11::arg("callback"),
          pybind11::arg("reset"));
}

}  // namespace python
}  // namespace roq
