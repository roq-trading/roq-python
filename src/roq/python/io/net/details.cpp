/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/io/net/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/io/net/reorder_buffer.hpp"

using namespace std::literals;

namespace roq {
namespace python {

namespace io {
namespace net {

auto const DEFAULT_REORDER_BUFFER_OPTIONS = roq::io::net::ReorderBuffer::Options{
    .depth = 8,
    .maximum_packet_size = 4096,
    .maximum_sequence_number = std::numeric_limits<uint64_t>::max(),
    .minimum_sequence_number = {},
};

ReorderBuffer::ReorderBuffer() : reorder_buffer_{roq::io::net::ReorderBuffer::create(DEFAULT_REORDER_BUFFER_OPTIONS)} {
}

}  // namespace net
}  // namespace io

template <>
void utils::create_struct<roq::python::io::net::ReorderBuffer>(pybind11::module_ &module) {
  using value_type = roq::python::io::net::ReorderBuffer;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(pybind11::init<>())
      .def(
          "dispatch",
          [](value_type &self,
             pybind11::bytes data,
             uint64_t sequence_number,
             std::function<void(pybind11::bytes const &)> const &parse,
             std::function<void()> const &reset) {
            struct MyHandler final : public roq::io::net::ReorderBuffer::Handler {
              explicit MyHandler(
                  uint64_t sequence_number,
                  std::function<void(pybind11::bytes const &)> const &parse,
                  std::function<void()> const &reset)
                  : sequence_number_{sequence_number}, parse_{parse}, reset_{reset} {}

              uint64_t operator()(roq::io::net::ReorderBuffer::GetSequenceNumber const &) override {
                return sequence_number_;
              }
              void operator()(roq::io::net::ReorderBuffer::Parse const &parse) override {
                pybind11::bytes arg0{
                    reinterpret_cast<char const *>(std::data(parse.payload)), std::size(parse.payload)};
                parse_(arg0);
              }
              void operator()(roq::io::net::ReorderBuffer::Reset const &) override { reset_(); }

             private:
              uint64_t const sequence_number_;
              std::function<void(pybind11::bytes const &)> const &parse_;
              std::function<void()> const &reset_;
            } handler{sequence_number, parse, reset};
            auto data_1 = static_cast<std::string_view>(data);
            std::span data_2{reinterpret_cast<std::byte const *>(std::data(data_1)), std::size(data_1)};
            static_cast<value_type::value_type &>(self).dispatch(handler, data_2);
          },
          pybind11::arg("data"),
          pybind11::arg("sequence_number"),
          pybind11::arg("parse"),
          pybind11::arg("reset"));
}

}  // namespace python
}  // namespace roq
