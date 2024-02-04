/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/market/module.hpp"

#include "roq/python/market/mbo/module.hpp"
#include "roq/python/market/mbp/module.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace market {

void Module::create(pybind11::module_ &module) {
  auto mbp = module.def_submodule("mbp");
  roq::python::market::mbp::Module::create(mbp);

  auto mbo = module.def_submodule("mbo");
  roq::python::market::mbo::Module::create(mbo);
}

}  // namespace market
}  // namespace python
}  // namespace roq
