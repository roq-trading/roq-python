/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

#include "roq/codec/udp/header.hpp"

namespace roq {
namespace python {
namespace codec {
namespace udp {

struct Header final {
  using value_type = roq::codec::udp::Header;

  explicit Header(value_type const &value) : value_{value} {}
  explicit Header(pybind11::bytes message);

  operator value_type() const { return value_; }

 private:
  value_type value_;
};

}  // namespace udp
}  // namespace codec
}  // namespace python
}  // namespace roq
