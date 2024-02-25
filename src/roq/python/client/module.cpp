/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/client/module.hpp"

#include "roq/python/client/details.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace client {

void Module::create(pybind11::module_ &module) {
  pybind11::class_<roq::python::client::Handler, roq::python::client::PyHandler>(module, "Handler")
      .def(pybind11::init<>())
      .def("callback", &roq::python::client::Handler::callback);

  utils::create_struct<roq::python::client::Settings2>(module);

  utils::create_struct<roq::client::Settings>(module);  // XXX
  utils::create_struct<roq::python::client::Config>(module);

  utils::create_struct<roq::python::client::Dispatcher>(module);

  utils::create_struct<roq::python::client::EventLogReader>(module);
  utils::create_struct<roq::python::client::EventLogMultiplexer>(module);
}

}  // namespace client
}  // namespace python
}  // namespace roq
