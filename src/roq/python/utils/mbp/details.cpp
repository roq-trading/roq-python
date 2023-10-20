/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/utils/mbp/details.hpp"

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
    // assert(!mbp_wait_for_snapshot_);
    auto mbp_update = create(bids, asks, UpdateType::INCREMENTAL);
    dispatch(mbp_update);
  };
  auto publish_snapshot = [&](auto &bids, auto &asks, [[maybe_unused]] auto sequence) {
    auto mbp_update = create(bids, asks, UpdateType::SNAPSHOT);
    dispatch(mbp_update);
    // mbp_wait_for_snapshot_ = false;  // note! now we could leave the snapshot channel
  };
  auto request_snapshot = [&](auto retries) {
    reset(retries);
    // mbp_wait_for_snapshot_ = true;  // note! now we must join the snapshot channel
  };
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
void utils::create_struct<utils::mbp::MarketByPrice>(pybind11::module_ &module) {
  using value_type = utils::mbp::MarketByPrice;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<std::string_view const &, std::string_view const &>(),
          pybind11::arg("exchange") = "",
          pybind11::arg("symbol"))
      .def(
          "apply",
          [](value_type &self, utils::Ref<MarketByPriceUpdate> const &market_by_price_update) {
            self(market_by_price_update);
          })
      .def("extract", [](value_type const &self, size_t depth) { return self.extract(depth); })
      .def("__repr__", [](value_type const &self) { return self.print(); });
}

template <>
void utils::create_struct<utils::mbp::Sequencer>(pybind11::module_ &module) {
  using value_type = utils::mbp::Sequencer;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<>())
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
