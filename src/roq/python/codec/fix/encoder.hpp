/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>
#include <string>
#include <string_view>

#include "roq/codec/fix/encoder.hpp"

#include "roq/python/codec/fix/encodeable.hpp"

namespace roq {
namespace python {
namespace codec {
namespace fix {

struct Encoder final {
  Encoder(std::string_view const &sender_comp_id, std::string_view const &target_comp_id)
      : encoder_{roq::codec::fix::Encoder::create()}, sender_comp_id_{sender_comp_id}, target_comp_id_{target_comp_id} {
  }

  template <typename T>
  std::span<std::byte const> encode(T const &value, std::chrono::nanoseconds sending_time) {
    auto header = roq::fix::Header{
        .version = roq::fix::Version::FIX_44,
        .msg_type = T::MSG_TYPE,
        .sender_comp_id = sender_comp_id_,
        .target_comp_id = target_comp_id_,
        .msg_seq_num = ++msg_seq_num_,
        .sending_time = sending_time,
    };
    return (*encoder_).encode(header, value);
  }

  std::span<std::byte const> encode(Encodeable const &encodeable, std::chrono::system_clock::time_point sending_time) {
    return encodeable.encode(*this, sending_time.time_since_epoch());
  }

 private:
  std::unique_ptr<roq::codec::fix::Encoder> encoder_;
  std::string const sender_comp_id_;
  std::string const target_comp_id_;
  uint64_t msg_seq_num_ = {};
};

}  // namespace fix
}  // namespace codec
}  // namespace python
}  // namespace roq
