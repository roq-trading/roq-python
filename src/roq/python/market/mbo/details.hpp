/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/cache/market_by_order.hpp"

#include "roq/market/mbo/factory.hpp"

#include "roq/python/utils.hpp"

namespace roq {
namespace python {
namespace market {
namespace mbo {

struct MarketByOrder final {
  MarketByOrder(std::string_view const &exchange, std::string_view const &symbol)
      : market_by_order_{roq::market::mbo::Factory::create(exchange, symbol)} {}

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
      using namespace std::literals;
      result = fmt::format("{}"sv, market_by_order_update);
    });
    return result;
  }

 private:
  std::unique_ptr<roq::cache::MarketByOrder> market_by_order_;
};

}  // namespace mbo
}  // namespace market
}  // namespace python
}  // namespace roq
