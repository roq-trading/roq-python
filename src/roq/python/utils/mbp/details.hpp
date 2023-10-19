/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/cache/market_by_order.hpp"

#include "roq/utils/mbp/factory.hpp"

#include "roq/python/utils.hpp"

namespace roq {
namespace python {
namespace utils {
namespace mbp {

struct MarketByPrice final {
  MarketByPrice(std::string_view const &exchange, std::string_view const &symbol)
      : market_by_price_{roq::utils::mbp::Factory::create(exchange, symbol)} {}

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
      using namespace std::literals;
      result = fmt::format("{}"sv, market_by_price_update);
    });
    return result;
  }

 private:
  std::unique_ptr<roq::cache::MarketByPrice> market_by_price_;
};

}  // namespace mbp
}  // namespace utils
}  // namespace python
}  // namespace roq
