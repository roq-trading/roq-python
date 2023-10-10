/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

namespace roq {
namespace python {
namespace codec {
namespace udp {

struct Module final {
  static void create(pybind11::module_ &);
};

}  // namespace udp
}  // namespace codec
}  // namespace python
}  // namespace roq
