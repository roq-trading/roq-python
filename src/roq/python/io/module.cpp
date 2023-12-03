/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/io/module.hpp"

#include "roq/python/io/net/module.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace io {

void Module::create(pybind11::module_ &module) {
  auto net = module.def_submodule("net");
  roq::python::io::net::Module::create(net);
}

}  // namespace io
}  // namespace python
}  // namespace roq
