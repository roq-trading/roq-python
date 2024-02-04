/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/market/mbp/module.hpp"

#include "roq/python/utils.hpp"

#include "roq/python/market/mbp/details.hpp"

namespace roq {
namespace python {
namespace market {
namespace mbp {

void Module::create(pybind11::module_ &module) {
  utils::create_struct<roq::python::market::mbp::MarketByPrice>(module);
  utils::create_struct<roq::python::market::mbp::Sequencer>(module);
}

}  // namespace mbp
}  // namespace market
}  // namespace python
}  // namespace roq
