/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/io/net/module.hpp"

#include "roq/python/utils.hpp"

#include "roq/python/io/net/details.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace io {
namespace net {

void Module::create(pybind11::module_ &module) {
  roq::python::utils::create_struct<roq::python::io::net::ReorderBuffer>(module);
}

}  // namespace net
}  // namespace io
}  // namespace python
}  // namespace roq
