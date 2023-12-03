/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string>

#include "roq/fix/header.hpp"

namespace roq {
namespace python {
namespace codec {
namespace fix {

struct Header final {
  using value_type = roq::fix::Header;

  explicit Header(value_type const &header)
      : msg_type_{header.msg_type}, sender_comp_id_{header.sender_comp_id}, target_comp_id_{header.target_comp_id},
        msg_seq_num_{header.msg_seq_num}, sending_time_{header.sending_time} {}

  operator value_type() const {
    return {
        .version = roq::fix::Version::FIX_44,
        .msg_type = msg_type_,
        .sender_comp_id = sender_comp_id_,
        .target_comp_id = target_comp_id_,
        .msg_seq_num = msg_seq_num_,
        .sending_time = sending_time_,
    };
  }

 private:
  roq::fix::MsgType const msg_type_;
  std::string const sender_comp_id_;
  std::string const target_comp_id_;
  uint64_t const msg_seq_num_;
  std::chrono::nanoseconds const sending_time_;
};

}  // namespace fix
}  // namespace codec
}  // namespace python
}  // namespace roq
