/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

namespace roq {
namespace python {

struct Module final {
  static void create(pybind11::module_ &);
};

}  // namespace python
}  // namespace roq
