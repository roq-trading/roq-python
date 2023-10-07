/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>

namespace roq {
namespace python {
namespace fix {

struct Encoder;

struct Encodeable {
  virtual ~Encodeable() = default;
  virtual std::span<std::byte const> encode(Encoder &, std::chrono::nanoseconds sending_time) const = 0;
};

}  // namespace fix
}  // namespace python
}  // namespace roq
