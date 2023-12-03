/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/utils/module.hpp"

#include "roq/python/utils/mbo/module.hpp"
#include "roq/python/utils/mbp/module.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace utils {

void Module::create(pybind11::module_ &module) {
  auto mbp = module.def_submodule("mbp");
  roq::python::utils::mbp::Module::create(mbp);

  auto mbo = module.def_submodule("mbo");
  roq::python::utils::mbo::Module::create(mbo);
}

}  // namespace utils
}  // namespace python
}  // namespace roq
