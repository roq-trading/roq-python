/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/fix/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/fix/decoder.hpp"
#include "roq/python/fix/details.hpp"
#include "roq/python/fix/header.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace fix {

void Module::create(pybind11::module_ &module) {
  utils::create_enum<roq::fix::MsgType>(module);

  utils::create_enum<roq::fix::EncryptMethod>(module);
  utils::create_enum<roq::fix::SessionRejectReason>(module);
  utils::create_enum<roq::fix::BusinessRejectReason>(module);
  utils::create_enum<roq::fix::UserRequestType>(module);
  utils::create_enum<roq::fix::UserStatus>(module);
  utils::create_enum<roq::fix::SubscriptionRequestType>(module);
  utils::create_enum<roq::fix::TradSesStatus>(module);
  utils::create_enum<roq::fix::TradSesStatusRejReason>(module);
  utils::create_enum<roq::fix::SecurityListRequestType>(module);
  utils::create_enum<roq::fix::SecurityRequestResult>(module);

  utils::create_struct<roq::python::fix::SecListGrp>(module);

  utils::create_struct<roq::python::fix::Encodeable>(module);
  utils::create_struct<roq::python::fix::Encoder>(module);

  utils::create_ref_struct_2<roq::python::fix::Logon, roq::python::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::fix::Logout, roq::python::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::fix::TestRequest, roq::python::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::fix::Heartbeat, roq::python::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::fix::ResendRequest, roq::python::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::fix::Reject, roq::python::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::fix::BusinessMessageReject, roq::python::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::fix::UserRequest, roq::python::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::fix::UserResponse, roq::python::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::fix::TradingSessionStatusRequest, roq::python::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::fix::TradingSessionStatus, roq::python::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::fix::SecurityListRequest, roq::python::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::fix::SecurityList, roq::python::fix::Encodeable>(module);

  utils::create_struct<roq::python::fix::Header>(module);
  utils::create_struct<roq::python::fix::Decoder>(module);
}

}  // namespace fix
}  // namespace python
}  // namespace roq
