/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/cache/module.hpp"

#include "roq/python/utils.hpp"

#include "roq/python/cache/details.hpp"

namespace roq {
namespace python {
namespace cache {

void Module::create(pybind11::module_ &module) {
  roq::python::utils::create_struct<roq::python::cache::MarketByPrice>(module);
  roq::python::utils::create_struct<roq::python::cache::MarketByOrder>(module);
}

}  // namespace cache
}  // namespace python
}  // namespace roq
