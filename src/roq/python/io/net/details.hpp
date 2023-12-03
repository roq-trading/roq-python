/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/io/net/reorder_buffer.hpp"

namespace roq {
namespace python {
namespace io {
namespace net {

struct ReorderBuffer final {
  using value_type = roq::io::net::ReorderBuffer;

  ReorderBuffer();

  operator value_type &() { return *reorder_buffer_; }

 private:
  std::unique_ptr<value_type> reorder_buffer_;
};

}  // namespace net
}  // namespace io
}  // namespace python
}  // namespace roq
