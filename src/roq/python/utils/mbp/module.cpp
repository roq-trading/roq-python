/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/utils/mbp/module.hpp"

#include "roq/python/utils.hpp"

#include "roq/python/utils/mbp/details.hpp"

namespace roq {
namespace python {
namespace utils {
namespace mbp {

void Module::create(pybind11::module_ &module) {
  roq::python::utils::create_struct<roq::python::utils::mbp::MarketByPrice>(module);
}

}  // namespace mbp
}  // namespace utils
}  // namespace python
}  // namespace roq
