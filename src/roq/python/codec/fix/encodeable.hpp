/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>

namespace roq {
namespace python {
namespace codec {
namespace fix {

struct Encoder;

struct Encodeable {
  virtual ~Encodeable() = default;
  virtual std::span<std::byte const> encode(Encoder &, std::chrono::nanoseconds sending_time) const = 0;
};

}  // namespace fix
}  // namespace codec
}  // namespace python
}  // namespace roq
