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
  explicit LogHandler(std::function<void(roq::logging::Level, std::string_view const &)> const &handler)
      : handler_{handler} {}
  void operator()(roq::logging::Level level, std::string_view const &message) override { handler_(level, message); }

 private:
  std::function<void(roq::logging::Level, std::string_view const &)> handler_;
};

static std::unique_ptr<roq::logging::Handler> HANDLER;
}  // namespace

void reset() {
  HANDLER.reset();
}

void set_handler(std::function<void(roq::logging::Level, std::string_view const &)> const &handler) {
  HANDLER = std::make_unique<LogHandler>(handler);
}

}  // namespace logging
}  // namespace python
}  // namespace roq
