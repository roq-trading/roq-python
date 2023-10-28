/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/fix/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/fix/msg_type.hpp"

#include "roq/fix/business_reject_reason.hpp"
#include "roq/fix/encrypt_method.hpp"
#include "roq/fix/security_list_request_type.hpp"
#include "roq/fix/security_request_result.hpp"
#include "roq/fix/session_reject_reason.hpp"
#include "roq/fix/subscription_request_type.hpp"
#include "roq/fix/trad_ses_status.hpp"
#include "roq/fix/trad_ses_status_rej_reason.hpp"
#include "roq/fix/user_request_type.hpp"
#include "roq/fix/user_status.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace fix {

void Module::create(pybind11::module_ &module) {
  utils::create_enum<roq::fix::MsgType>(module);

  utils::create_enum<roq::fix::BusinessRejectReason>(module);
  utils::create_enum<roq::fix::EncryptMethod>(module);
  utils::create_enum<roq::fix::SecurityListRequestType>(module);
  utils::create_enum<roq::fix::SecurityRequestResult>(module);
  utils::create_enum<roq::fix::SessionRejectReason>(module);
  utils::create_enum<roq::fix::SubscriptionRequestType>(module);
  utils::create_enum<roq::fix::TradSesStatus>(module);
  utils::create_enum<roq::fix::TradSesStatusRejReason>(module);
  utils::create_enum<roq::fix::UserRequestType>(module);
  utils::create_enum<roq::fix::UserStatus>(module);
}

}  // namespace fix
}  // namespace python
}  // namespace roq
