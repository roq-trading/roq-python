/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/codec/sbe/encoder.hpp"

namespace roq {
namespace python {
namespace codec {
namespace sbe {

struct Encoder final {
  Encoder() : encoder_{roq::codec::sbe::Encoder::create()} {}

  std::span<std::byte const> operator()(MessageInfo const &message_info, auto const &value) {
    Event event{message_info, value};
    return (*encoder_)(event);
  }

 private:
  std::unique_ptr<roq::codec::sbe::Encoder> encoder_;
};

}  // namespace sbe
}  // namespace codec
}  // namespace python
}  // namespace roq
