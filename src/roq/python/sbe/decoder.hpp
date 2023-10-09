/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

#include "roq/codec/sbe/decoder.hpp"

#include "roq/python/sbe/details.hpp"

namespace roq {
namespace python {
namespace sbe {

struct Decoder final {
  template <typename Callback>
  struct Handler final : public roq::codec::sbe::Decoder::Handler {
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

    void operator()(Event<ReferenceData> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<MarketStatus> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<TopOfBook> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<MarketByPriceUpdate> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<MarketByOrderUpdate> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<TradeSummary> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<StatisticsUpdate> const &event) override { dispatch(event.message_info, event.value); }
    // ...
    void operator()(Event<OrderAck> const &event) override { dispatch(event.message_info, event.value); }
    void operator()(Event<OrderUpdate> const &event) override { dispatch(event.message_info, event.value); }

   private:
    Callback const &callback_;
  };

  Decoder() : decoder_{roq::codec::sbe::Decoder::create()} {}

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
    }
    return result;
  }

  // XXX HANS tuple

 private:
  std::unique_ptr<roq::codec::sbe::Decoder> decoder_;
};

}  // namespace sbe
}  // namespace python
}  // namespace roq
