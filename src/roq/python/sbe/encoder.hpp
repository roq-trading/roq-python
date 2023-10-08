/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/codec/sbe/encoder.hpp"

namespace roq {
namespace python {
namespace sbe {

struct Encoder final {
  Encoder() : encoder_{roq::codec::sbe::Encoder::create()} {}

  template <typename T>
  std::span<std::byte const> encode(T const &value) {
    return (*encoder_).encode(value);
  }

 private:
  std::unique_ptr<roq::codec::sbe::Encoder> encoder_;
};

}  // namespace sbe
}  // namespace python
}  // namespace roq
