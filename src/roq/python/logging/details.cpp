/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/logging/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

using namespace std::literals;

namespace roq {
namespace python {
namespace logging {

namespace {
struct LogHandler final : public roq::logging::Handler {
  explicit LogHandler(std::function<void(roq::logging::Level, std::string_view const &)> const &callback)
      : callback_{callback} {}
  void operator()(roq::logging::Level level, std::string_view const &message) override { callback_(level, message); }

 private:
  std::function<void(roq::logging::Level, std::string_view const &)> callback_;
};

static std::unique_ptr<LogHandler> HANDLER;
}  // namespace

void set_callback(std::function<void(roq::logging::Level, std::string_view const &)> const &callback) {
  HANDLER = std::make_unique<LogHandler>(callback);
}

}  // namespace logging
}  // namespace python
}  // namespace roq
