/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

#include <nameof.hpp>

#include "roq/api.hpp"

namespace roq {
namespace python {
namespace utils {

template <typename T>
auto to_list(T const &values) {
  using value_type = std::remove_cvref<T>::type::value_type;
  pybind11::list result;
  for (auto &item : values)
    result.append(value_type{item});
  return result;
}

// note! this is a work-around until IntFlag (?) can be supported properly
template <typename T>
auto to_int_flag(Mask<T> const &value) {
  return static_cast<T>(value.get());
}

template <typename T>
void create_enum(auto &module) {
  std::string name{nameof::nameof_short_type<T>()};
  pybind11::enum_<T> enum_{module, name.c_str()};
  for (auto item : magic_enum::enum_values<T>()) {
    std::string name{magic_enum::enum_name(item)};
    enum_.value(name.c_str(), item);
  }
}

template <typename T>
struct Ref final {
  Ref() = delete;  // not allowed
  explicit Ref(T const &value) : value{value} {}

  operator T const &() const { return value; }

 private:
  T const &value;
};

// note! copy values
template <typename T>
void create_struct(pybind11::module_ &);

// note! reference to an underlying object (user is therefore not allowed to keep handles)
template <typename T>
void create_ref_struct(pybind11::module_ &);

template <typename T, typename U>
void create_ref_struct_2(pybind11::module_ &);

}  // namespace utils
}  // namespace python
}  // namespace roq
