/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

namespace roq {
namespace python {
namespace market {
namespace mbo {

struct Module final {
  static void create(pybind11::module_ &);
};

}  // namespace mbo
}  // namespace market
}  // namespace python
}  // namespace roq
