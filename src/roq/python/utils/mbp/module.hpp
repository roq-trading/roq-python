/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

namespace roq {
namespace python {
namespace utils {
namespace mbp {

struct Module final {
  static void create(pybind11::module_ &);
};

}  // namespace mbp
}  // namespace utils
}  // namespace python
}  // namespace roq
