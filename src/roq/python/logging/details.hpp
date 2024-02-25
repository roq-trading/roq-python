/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <pybind11/pybind11.h>

#include "roq/compat.hpp"

#include "roq/logging.hpp"

#include "roq/python/utils.hpp"

namespace roq {
namespace python {
namespace logging {

void ROQ_PUBLIC set_handler(std::function<void(roq::logging::Level, std::string_view const &)> const &);

}  // namespace logging
}  // namespace python
}  // namespace roq
