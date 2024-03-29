/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/fix/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/fix/msg_type.hpp"

#include "roq/fix/account_type.hpp"
#include "roq/fix/business_reject_reason.hpp"
#include "roq/fix/cxl_rej_reason.hpp"
#include "roq/fix/cxl_rej_response_to.hpp"
#include "roq/fix/encrypt_method.hpp"
#include "roq/fix/exec_type.hpp"
#include "roq/fix/handl_inst.hpp"
#include "roq/fix/last_liquidity_ind.hpp"
#include "roq/fix/mass_cancel_reject_reason.hpp"
#include "roq/fix/mass_cancel_request_type.hpp"
#include "roq/fix/mass_cancel_response.hpp"
#include "roq/fix/mass_status_req_type.hpp"
#include "roq/fix/md_entry_type.hpp"
#include "roq/fix/md_req_rej_reason.hpp"
#include "roq/fix/md_update_action.hpp"
#include "roq/fix/md_update_type.hpp"
#include "roq/fix/ord_rej_reason.hpp"
#include "roq/fix/ord_status.hpp"
#include "roq/fix/ord_type.hpp"
#include "roq/fix/pos_req_result.hpp"
#include "roq/fix/pos_req_status.hpp"
#include "roq/fix/pos_req_type.hpp"
#include "roq/fix/position_effect.hpp"
#include "roq/fix/security_list_request_type.hpp"
#include "roq/fix/security_request_result.hpp"
#include "roq/fix/security_request_type.hpp"
#include "roq/fix/security_response_type.hpp"
#include "roq/fix/security_trading_status.hpp"
#include "roq/fix/session_reject_reason.hpp"
#include "roq/fix/settl_price_type.hpp"
#include "roq/fix/side.hpp"
#include "roq/fix/subscription_request_type.hpp"
#include "roq/fix/time_in_force.hpp"
#include "roq/fix/trad_ses_status.hpp"
#include "roq/fix/trad_ses_status_rej_reason.hpp"
#include "roq/fix/trade_request_type.hpp"
#include "roq/fix/user_request_type.hpp"
#include "roq/fix/user_status.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace fix {

void Module::create(pybind11::module_ &module) {
  utils::create_enum<roq::fix::MsgType>(module);

  utils::create_enum<roq::fix::AccountType>(module);
  utils::create_enum<roq::fix::BusinessRejectReason>(module);
  utils::create_enum<roq::fix::CxlRejReason>(module);
  utils::create_enum<roq::fix::CxlRejResponseTo>(module);
  utils::create_enum<roq::fix::EncryptMethod>(module);
  utils::create_enum<roq::fix::ExecType>(module);
  utils::create_enum<roq::fix::HandlInst>(module);
  utils::create_enum<roq::fix::LastLiquidityInd>(module);
  utils::create_enum<roq::fix::MassCancelRejectReason>(module);
  utils::create_enum<roq::fix::MassCancelRequestType>(module);
  utils::create_enum<roq::fix::MassCancelResponse>(module);
  utils::create_enum<roq::fix::MassStatusReqType>(module);
  utils::create_enum<roq::fix::MDEntryType>(module);
  utils::create_enum<roq::fix::MDReqRejReason>(module);
  utils::create_enum<roq::fix::MDUpdateAction>(module);
  utils::create_enum<roq::fix::MDUpdateType>(module);
  utils::create_enum<roq::fix::OrdRejReason>(module);
  utils::create_enum<roq::fix::OrdStatus>(module);
  utils::create_enum<roq::fix::OrdType>(module);
  utils::create_enum<roq::fix::PositionEffect>(module);
  utils::create_enum<roq::fix::PosReqResult>(module);
  utils::create_enum<roq::fix::PosReqStatus>(module);
  utils::create_enum<roq::fix::PosReqType>(module);
  utils::create_enum<roq::fix::SecurityListRequestType>(module);
  utils::create_enum<roq::fix::SecurityRequestResult>(module);
  utils::create_enum<roq::fix::SecurityRequestType>(module);
  utils::create_enum<roq::fix::SecurityResponseType>(module);
  utils::create_enum<roq::fix::SecurityTradingStatus>(module);
  utils::create_enum<roq::fix::SessionRejectReason>(module);
  utils::create_enum<roq::fix::SettlPriceType>(module);
  utils::create_enum<roq::fix::Side>(module);
  utils::create_enum<roq::fix::SubscriptionRequestType>(module);
  utils::create_enum<roq::fix::TimeInForce>(module);
  utils::create_enum<roq::fix::TradeRequestType>(module);
  utils::create_enum<roq::fix::TradSesStatus>(module);
  utils::create_enum<roq::fix::TradSesStatusRejReason>(module);
  utils::create_enum<roq::fix::UserRequestType>(module);
  utils::create_enum<roq::fix::UserStatus>(module);
}

}  // namespace fix
}  // namespace python
}  // namespace roq
