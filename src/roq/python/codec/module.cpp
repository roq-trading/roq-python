/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/module.hpp"

// #include "roq/python/codec/fbs/module.hpp"
#include "roq/python/codec/fix/module.hpp"
#include "roq/python/codec/sbe/module.hpp"
#include "roq/python/codec/udp/module.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace codec {

void Module::create(pybind11::module_ &module) {
  // auto fbs = module.def_submodule("fbs");
  // roq::python::codec::fbs::Module::create(fbs);

  auto fix = module.def_submodule("fix");
  roq::python::codec::fix::Module::create(fix);

  auto sbe = module.def_submodule("sbe");
  roq::python::codec::sbe::Module::create(sbe);

  auto udp = module.def_submodule("udp");
  roq::python::codec::udp::Module::create(udp);
}

}  // namespace codec
}  // namespace python
}  // namespace roq
