/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/market/mbo/module.hpp"

#include "roq/python/utils.hpp"

#include "roq/python/market/mbo/details.hpp"

namespace roq {
namespace python {
namespace market {
namespace mbo {

void Module::create(pybind11::module_ &module) {
  utils::create_struct<roq::python::market::mbo::MarketByOrder>(module);
}

}  // namespace mbo
}  // namespace market
}  // namespace python
}  // namespace roq
