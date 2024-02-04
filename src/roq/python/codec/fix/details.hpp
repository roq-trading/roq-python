/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/codec/fix/business_message_reject.hpp"
#include "roq/codec/fix/heartbeat.hpp"
#include "roq/codec/fix/logon.hpp"
#include "roq/codec/fix/logout.hpp"
#include "roq/codec/fix/reject.hpp"
#include "roq/codec/fix/resend_request.hpp"
#include "roq/codec/fix/security_list.hpp"
#include "roq/codec/fix/security_list_request.hpp"
#include "roq/codec/fix/test_request.hpp"
#include "roq/codec/fix/trading_session_status.hpp"
#include "roq/codec/fix/trading_session_status_request.hpp"
#include "roq/codec/fix/user_request.hpp"
#include "roq/codec/fix/user_response.hpp"

#include "roq/python/codec/fix/encodeable.hpp"
#include "roq/python/codec/fix/encoder.hpp"

namespace roq {
namespace python {
namespace codec {
namespace fix {

namespace {
struct DateTime final {
  explicit DateTime(std::chrono::system_clock::time_point value) : value_{value} {}

  operator std::chrono::milliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(value_.time_since_epoch());
  }
  operator std::chrono::year_month_day() const {
    return {};  // XXX FIXME TODO
  }

 private:
  std::chrono::system_clock::time_point const value_;
};
}  // namespace

struct Logon final : public Encodeable {
  using value_type = roq::codec::fix::Logon;

  explicit Logon(value_type const &value)
      : encrypt_method_{value.encrypt_method}, heart_bt_int_{value.heart_bt_int},
        raw_data_length_{value.raw_data_length}, raw_data_{value.raw_data},
        reset_seq_num_flag_{value.reset_seq_num_flag}, next_expected_msg_seq_num_{value.next_expected_msg_seq_num},
        username_{value.username}, password_{value.password} {}

  Logon(
      roq::fix::EncryptMethod encrypt_method,
      std::chrono::seconds heart_bt_int,
      uint32_t raw_data_length,
      std::string_view const &raw_data,
      bool reset_seq_num_flag,
      uint64_t next_expected_msg_seq_num,
      std::string_view const &username,
      std::string_view const &password)
      : encrypt_method_{encrypt_method}, heart_bt_int_{heart_bt_int}, raw_data_length_{raw_data_length},
        raw_data_{raw_data}, reset_seq_num_flag_{reset_seq_num_flag},
        next_expected_msg_seq_num_{next_expected_msg_seq_num}, username_{username}, password_{password} {}

  operator value_type() const {
    return {
        .encrypt_method = encrypt_method_,
        .heart_bt_int = static_cast<uint16_t>(heart_bt_int_.count()),
        .raw_data_length = raw_data_length_,
        .raw_data = raw_data_,
        .reset_seq_num_flag = reset_seq_num_flag_,
        .next_expected_msg_seq_num = next_expected_msg_seq_num_,
        .username = username_,
        .password = password_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  roq::fix::EncryptMethod const encrypt_method_;
  std::chrono::seconds const heart_bt_int_;
  uint32_t const raw_data_length_;
  std::string const raw_data_;
  bool const reset_seq_num_flag_;
  uint64_t const next_expected_msg_seq_num_;
  std::string const username_;
  std::string const password_;
};

struct Logout final : public Encodeable {
  using value_type = roq::codec::fix::Logout;

  explicit Logout(value_type const &value) : text_{value.text} {}

  explicit Logout(std::string_view const &text) : text_{text} {}

  operator value_type() const {
    return {
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const text_;
};

struct TestRequest final : public Encodeable {
  using value_type = roq::codec::fix::TestRequest;

  explicit TestRequest(value_type const &value) : test_req_id_{value.test_req_id} {}

  explicit TestRequest(std::string_view const &test_req_id) : test_req_id_{test_req_id} {}

  operator value_type() const {
    return {
        .test_req_id = test_req_id_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const test_req_id_;
};

struct Heartbeat final : public Encodeable {
  using value_type = roq::codec::fix::Heartbeat;

  explicit Heartbeat(value_type const &value) : test_req_id_{value.test_req_id} {}

  explicit Heartbeat(std::string_view const &test_req_id) : test_req_id_{test_req_id} {}

  operator value_type() const {
    return {
        .test_req_id = test_req_id_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const test_req_id_;
};

struct ResendRequest final : public Encodeable {
  using value_type = roq::codec::fix::ResendRequest;

  explicit ResendRequest(value_type const &value) : begin_seq_no_{value.begin_seq_no}, end_seq_no_{value.end_seq_no} {}

  ResendRequest(uint64_t begin_seq_no, uint64_t end_seq_no) : begin_seq_no_{begin_seq_no}, end_seq_no_{end_seq_no} {}

  operator value_type() const {
    return {
        .begin_seq_no = begin_seq_no_,
        .end_seq_no = end_seq_no_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  uint64_t const begin_seq_no_;
  uint64_t const end_seq_no_;
};

struct Reject final : public Encodeable {
  using value_type = roq::codec::fix::Reject;

  explicit Reject(value_type const &value)
      : ref_seq_num_{value.ref_seq_num}, text_{value.text}, ref_tag_id_{value.ref_tag_id},
        ref_msg_type_{value.ref_msg_type}, session_reject_reason_{value.session_reject_reason} {}

  Reject(
      uint64_t ref_seq_num,
      std::string_view const &text,
      uint32_t ref_tag_id,
      roq::fix::MsgType ref_msg_type,
      roq::fix::SessionRejectReason session_reject_reason)
      : ref_seq_num_{ref_seq_num}, text_{text}, ref_tag_id_{ref_tag_id}, ref_msg_type_{ref_msg_type},
        session_reject_reason_{session_reject_reason} {}

  operator value_type() const {
    return {
        .ref_seq_num = ref_seq_num_,
        .text = text_,
        .ref_tag_id = ref_tag_id_,
        .ref_msg_type = ref_msg_type_,
        .session_reject_reason = session_reject_reason_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  uint64_t const ref_seq_num_;
  std::string const text_;
  uint32_t const ref_tag_id_;
  roq::fix::MsgType const ref_msg_type_;
  roq::fix::SessionRejectReason const session_reject_reason_;
};

struct BusinessMessageReject final : public Encodeable {
  using value_type = roq::codec::fix::BusinessMessageReject;

  explicit BusinessMessageReject(value_type const &value)
      : ref_seq_num_{value.ref_seq_num}, ref_msg_type_{value.ref_msg_type},
        business_reject_ref_id_{value.business_reject_ref_id}, business_reject_reason_{value.business_reject_reason},
        text_{value.text} {}

  BusinessMessageReject(
      uint64_t ref_seq_num,
      roq::fix::MsgType ref_msg_type,
      std::string_view const &business_reject_ref_id,
      roq::fix::BusinessRejectReason business_reject_reason,
      std::string_view const &text)
      : ref_seq_num_{ref_seq_num}, ref_msg_type_{ref_msg_type}, business_reject_ref_id_{business_reject_ref_id},
        business_reject_reason_{business_reject_reason}, text_{text} {}

  operator value_type() const {
    return {
        .ref_seq_num = ref_seq_num_,
        .ref_msg_type = ref_msg_type_,
        .business_reject_ref_id = business_reject_ref_id_,
        .business_reject_reason = business_reject_reason_,
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  uint64_t const ref_seq_num_;
  roq::fix::MsgType const ref_msg_type_;
  std::string const business_reject_ref_id_;
  roq::fix::BusinessRejectReason const business_reject_reason_;
  std::string_view const text_;
};

struct UserRequest final : public Encodeable {
  using value_type = roq::codec::fix::UserRequest;

  explicit UserRequest(value_type const &value)
      : user_request_id_{value.user_request_id}, user_request_type_{value.user_request_type}, username_{value.username},
        password_{value.password}, new_password_{value.new_password} {}

  UserRequest(
      std::string_view const &user_request_id,
      roq::fix::UserRequestType user_request_type,
      std::string_view const &username,
      std::string_view const &password,
      std::string_view const &new_password)
      : user_request_id_{user_request_id}, user_request_type_{user_request_type}, username_{username},
        password_{password}, new_password_{new_password} {}

  operator value_type() const {
    return {
        .user_request_id = user_request_id_,
        .user_request_type = user_request_type_,
        .username = username_,
        .password = password_,
        .new_password = new_password_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const user_request_id_;
  roq::fix::UserRequestType const user_request_type_;
  std::string const username_;
  std::string const password_;
  std::string const new_password_;
};

struct UserResponse final : public Encodeable {
  using value_type = roq::codec::fix::UserResponse;

  explicit UserResponse(value_type const &value)
      : user_request_id_{value.user_request_id}, username_{value.username}, user_status_{value.user_status},
        user_status_text_{value.user_status_text} {}

  UserResponse(
      std::string_view const &user_request_id,
      std::string_view const &username,
      roq::fix::UserStatus user_status,
      std::string_view const &user_status_text)
      : user_request_id_{user_request_id}, username_{username}, user_status_{user_status},
        user_status_text_{user_status_text} {}

  operator value_type() const {
    return {
        .user_request_id = user_request_id_,
        .username = username_,
        .user_status = user_status_,
        .user_status_text = user_status_text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const user_request_id_;
  std::string const username_;
  roq::fix::UserStatus const user_status_;
  std::string const user_status_text_;
};

struct TradingSessionStatusRequest final : public Encodeable {
  using value_type = roq::codec::fix::TradingSessionStatusRequest;

  explicit TradingSessionStatusRequest(value_type const &value)
      : trad_ses_req_id_{value.trad_ses_req_id}, trading_session_id_{value.trading_session_id},
        subscription_request_type_{value.subscription_request_type} {}

  TradingSessionStatusRequest(
      std::string_view const &trad_ses_req_id,
      std::string_view const &trading_session_id,
      roq::fix::SubscriptionRequestType subscription_request_type)
      : trad_ses_req_id_{trad_ses_req_id}, trading_session_id_{trading_session_id},
        subscription_request_type_{subscription_request_type} {}

  operator value_type() const {
    return {
        .trad_ses_req_id = trad_ses_req_id_,
        .trading_session_id = trading_session_id_,
        .subscription_request_type = subscription_request_type_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const trad_ses_req_id_;
  std::string const trading_session_id_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
};

struct TradingSessionStatus final : public Encodeable {
  using value_type = roq::codec::fix::TradingSessionStatus;

  explicit TradingSessionStatus(value_type const &value)
      : trad_ses_req_id_{value.trad_ses_req_id}, trading_session_id_{value.trading_session_id},
        unsolicited_indicator_{value.unsolicited_indicator}, trad_ses_status_{value.trad_ses_status},
        trad_ses_status_rej_reason_{value.trad_ses_status_rej_reason}, text_{value.text} {}

  TradingSessionStatus(
      std::string_view const &trad_ses_req_id,
      std::string_view const &trading_session_id,
      bool unsolicited_indicator,
      roq::fix::TradSesStatus trad_ses_status,
      roq::fix::TradSesStatusRejReason trad_ses_status_rej_reason,
      std::string_view const &text)
      : trad_ses_req_id_{trad_ses_req_id}, trading_session_id_{trading_session_id},
        unsolicited_indicator_{unsolicited_indicator}, trad_ses_status_{trad_ses_status},
        trad_ses_status_rej_reason_{trad_ses_status_rej_reason}, text_{text} {}

  operator value_type() const {
    return {
        .trad_ses_req_id = trad_ses_req_id_,
        .trading_session_id = trading_session_id_,
        .unsolicited_indicator = unsolicited_indicator_,
        .trad_ses_status = trad_ses_status_,
        .trad_ses_status_rej_reason = trad_ses_status_rej_reason_,
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const trad_ses_req_id_;
  std::string const trading_session_id_;
  bool const unsolicited_indicator_;
  roq::fix::TradSesStatus const trad_ses_status_;
  roq::fix::TradSesStatusRejReason const trad_ses_status_rej_reason_;
  std::string const text_;
};

struct SecurityListRequest final : public Encodeable {
  using value_type = roq::codec::fix::SecurityListRequest;

  explicit SecurityListRequest(value_type const &value)
      : security_req_id_{value.security_req_id}, security_list_request_type_{value.security_list_request_type},
        symbol_{value.symbol}, security_exchange_{value.security_exchange},
        trading_session_id_{value.trading_session_id}, subscription_request_type_{value.subscription_request_type} {}

  SecurityListRequest(
      std::string_view const &security_req_id,
      roq::fix::SecurityListRequestType security_list_request_type,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      std::string_view const &trading_session_id,
      roq::fix::SubscriptionRequestType subscription_request_type)
      : security_req_id_{security_req_id}, security_list_request_type_{security_list_request_type}, symbol_{symbol},
        security_exchange_{security_exchange}, trading_session_id_{trading_session_id},
        subscription_request_type_{subscription_request_type} {}

  operator value_type() const {
    return {
        .security_req_id = security_req_id_,
        .security_list_request_type = security_list_request_type_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .trading_session_id = trading_session_id_,
        .subscription_request_type = subscription_request_type_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const security_req_id_;
  roq::fix::SecurityListRequestType const security_list_request_type_;
  std::string const symbol_;
  std::string const security_exchange_;
  std::string const trading_session_id_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
};

struct SecListGrp final {
  using value_type = roq::codec::fix::SecListGrp;

  explicit SecListGrp(value_type const &value)
      : symbol{value.symbol}, contract_multiplier{value.contract_multiplier.value},
        security_exchange{value.security_exchange}, min_trade_vol{value.min_trade_vol.value},
        trading_session_id{value.trading_session_id}, min_price_increment{value.min_price_increment.value} {}

  SecListGrp(
      std::string_view const &symbol,
      double contract_multiplier,
      std::string_view const &security_exchange,
      double min_trade_vol,
      std::string_view const &trading_session_id,
      double min_price_increment)
      : symbol{symbol}, contract_multiplier{contract_multiplier}, security_exchange{security_exchange},
        min_trade_vol{min_trade_vol}, trading_session_id{trading_session_id}, min_price_increment{min_price_increment} {
  }

  operator value_type() const {
    return {
        .symbol = symbol,
        .contract_multiplier = {contract_multiplier, Precision{}},
        .security_exchange = security_exchange,
        .min_trade_vol = {min_trade_vol, Precision{}},
        .trading_session_id = trading_session_id,
        .min_price_increment = {min_price_increment, Precision{}},
    };
  };

  std::string symbol;
  double contract_multiplier = NaN;
  std::string security_exchange;
  double min_trade_vol = NaN;
  std::string trading_session_id;
  double min_price_increment = NaN;
};

struct SecurityList final : public Encodeable {
  using value_type = roq::codec::fix::SecurityList;

  explicit SecurityList(value_type const &value)
      : security_req_id_{value.security_req_id}, security_response_id_{value.security_response_id},
        security_request_result_{value.security_request_result},
        no_related_sym_{create<decltype(no_related_sym_)>(value.no_related_sym)},
        no_related_sym_2_{create_2<decltype(no_related_sym_2_)>(no_related_sym_)} {}

  SecurityList(
      std::string_view const &security_req_id,
      std::string_view const &security_response_id,
      roq::fix::SecurityRequestResult security_request_result,
      std::vector<SecListGrp> const &no_related_sym)
      : security_req_id_{security_req_id}, security_response_id_{security_response_id},
        security_request_result_{security_request_result},
        no_related_sym_{create<decltype(no_related_sym_)>(no_related_sym)},
        no_related_sym_2_{create_2<decltype(no_related_sym_2_)>(no_related_sym_)} {}

  operator value_type() const {
    return {
        .security_req_id = security_req_id_,
        .security_response_id = security_response_id_,
        .security_request_result = security_request_result_,
        .no_related_sym = no_related_sym_2_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

  template <typename R>
  static R create(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      value_type item_2{item};
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

  template <typename R>
  static R create_2(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      auto item_2 = static_cast<value_type>(item);
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

 private:
  std::string const security_req_id_;
  std::string const security_response_id_;
  roq::fix::SecurityRequestResult const security_request_result_;
  std::vector<SecListGrp> const no_related_sym_;
  std::vector<roq::codec::fix::SecListGrp> const no_related_sym_2_;
};

struct SecurityDefinitionRequest final : public Encodeable {
  using value_type = roq::codec::fix::SecurityDefinitionRequest;

  explicit SecurityDefinitionRequest(value_type const &value)
      : security_req_id_{value.security_req_id}, security_request_type_{value.security_request_type},
        symbol_{value.symbol}, security_exchange_{value.security_exchange},
        trading_session_id_{value.trading_session_id}, subscription_request_type_{value.subscription_request_type} {}

  SecurityDefinitionRequest(
      std::string_view const &security_req_id,
      roq::fix::SecurityRequestType security_request_type,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      std::string_view const &trading_session_id,
      roq::fix::SubscriptionRequestType subscription_request_type)
      : security_req_id_{security_req_id}, security_request_type_{security_request_type}, symbol_{symbol},
        security_exchange_{security_exchange}, trading_session_id_{trading_session_id},
        subscription_request_type_{subscription_request_type} {}

  operator value_type() const {
    return {
        .security_req_id = security_req_id_,
        .security_request_type = security_request_type_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .trading_session_id = trading_session_id_,
        .subscription_request_type = subscription_request_type_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const security_req_id_;
  roq::fix::SecurityRequestType const security_request_type_;
  std::string const symbol_;
  std::string const security_exchange_;
  std::string const trading_session_id_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
};

struct SecurityDefinition final : public Encodeable {
  using value_type = roq::codec::fix::SecurityDefinition;

  explicit SecurityDefinition(value_type const &value)
      : security_req_id_{value.security_req_id}, security_response_id_{value.security_response_id},
        security_response_type_{value.security_response_type}, symbol_{value.symbol},
        contract_multiplier_{value.contract_multiplier}, security_exchange_{value.security_exchange},
        trading_session_id_{value.trading_session_id}, min_trade_vol_{value.min_trade_vol} {}

  SecurityDefinition(
      std::string_view const &security_req_id,
      std::string_view const &security_response_id,
      roq::fix::SecurityResponseType security_response_type,
      std::string_view const &symbol,
      Decimal const &contract_multiplier,
      std::string_view const &security_exchange,
      std::string_view const &trading_session_id,
      Decimal const &min_trade_vol)
      : security_req_id_{security_req_id}, security_response_id_{security_response_id},
        security_response_type_{security_response_type}, symbol_{symbol}, contract_multiplier_{contract_multiplier},
        security_exchange_{security_exchange}, trading_session_id_{trading_session_id}, min_trade_vol_{min_trade_vol} {}

  // XXX Decimal
  SecurityDefinition(
      std::string_view const &security_req_id,
      std::string_view const &security_response_id,
      roq::fix::SecurityResponseType security_response_type,
      std::string_view const &symbol,
      double contract_multiplier,
      std::string_view const &security_exchange,
      std::string_view const &trading_session_id,
      double min_trade_vol)
      : security_req_id_{security_req_id}, security_response_id_{security_response_id},
        security_response_type_{security_response_type}, symbol_{symbol}, contract_multiplier_{contract_multiplier, {}},
        security_exchange_{security_exchange}, trading_session_id_{trading_session_id},
        min_trade_vol_{min_trade_vol, {}} {}

  operator value_type() const {
    return {
        .security_req_id = security_req_id_,
        .security_response_id = security_response_id_,
        .security_response_type = security_response_type_,
        .symbol = symbol_,
        .contract_multiplier = contract_multiplier_,
        .security_exchange = security_exchange_,
        .trading_session_id = trading_session_id_,
        .min_trade_vol = min_trade_vol_,
        .min_price_increment = {},  // XXX TODO
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const security_req_id_;
  std::string const security_response_id_;
  roq::fix::SecurityResponseType const security_response_type_;
  std::string const symbol_;
  Decimal const contract_multiplier_;
  std::string const security_exchange_;
  std::string const trading_session_id_;
  Decimal const min_trade_vol_;
};

struct SecurityStatusRequest final : public Encodeable {
  using value_type = roq::codec::fix::SecurityStatusRequest;

  explicit SecurityStatusRequest(value_type const &value)
      : security_status_req_id_{value.security_status_req_id}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, subscription_request_type_{value.subscription_request_type},
        trading_session_id_{value.trading_session_id} {}

  SecurityStatusRequest(
      std::string_view const &security_status_req_id,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::SubscriptionRequestType const &subscription_request_type,
      std::string_view const &trading_session_id)
      : security_status_req_id_{security_status_req_id}, symbol_{symbol}, security_exchange_{security_exchange},
        subscription_request_type_{subscription_request_type}, trading_session_id_{trading_session_id} {}

  operator value_type() const {
    return {
        .security_status_req_id = security_status_req_id_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .subscription_request_type = subscription_request_type_,
        .trading_session_id = trading_session_id_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const security_status_req_id_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
  std::string const trading_session_id_;
};

struct SecurityStatus final : public Encodeable {
  using value_type = roq::codec::fix::SecurityStatus;

  explicit SecurityStatus(value_type const &value)
      : security_status_req_id_{value.security_status_req_id}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, trading_session_id_{value.trading_session_id},
        unsolicited_indicator_{value.unsolicited_indicator}, security_trading_status_{value.security_trading_status},
        transact_time_{value.transact_time} {}

  SecurityStatus(
      std::string_view const &security_status_req_id,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      std::string_view const &trading_session_id,
      bool unsolicited_indicator,
      roq::fix::SecurityTradingStatus security_trading_status,
      std::chrono::milliseconds transact_time)
      : security_status_req_id_{security_status_req_id}, symbol_{symbol}, security_exchange_{security_exchange},
        trading_session_id_{trading_session_id}, unsolicited_indicator_{unsolicited_indicator},
        security_trading_status_{security_trading_status}, transact_time_{transact_time} {}

  SecurityStatus(
      std::string_view const &security_status_req_id,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      std::string_view const &trading_session_id,
      bool unsolicited_indicator,
      roq::fix::SecurityTradingStatus security_trading_status,
      std::chrono::system_clock::time_point transact_time)  // XXX
      : security_status_req_id_{security_status_req_id}, symbol_{symbol}, security_exchange_{security_exchange},
        trading_session_id_{trading_session_id}, unsolicited_indicator_{unsolicited_indicator},
        security_trading_status_{security_trading_status}, transact_time_{DateTime{transact_time}} {}

  operator value_type() const {
    return {
        .security_status_req_id = security_status_req_id_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .trading_session_id = trading_session_id_,
        .unsolicited_indicator = unsolicited_indicator_,
        .security_trading_status = security_trading_status_,
        .transact_time = transact_time_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const security_status_req_id_;
  std::string const symbol_;
  std::string const security_exchange_;
  std::string const trading_session_id_;
  bool const unsolicited_indicator_;
  roq::fix::SecurityTradingStatus const security_trading_status_;
  std::chrono::milliseconds const transact_time_;
};

struct InstrmtMDReq final {
  using value_type = roq::codec::fix::InstrmtMDReq;

  explicit InstrmtMDReq(value_type const &value) : symbol_{value.symbol}, security_exchange_{value.security_exchange} {}

  InstrmtMDReq(std::string_view const &symbol, std::string_view const &security_exchange)
      : symbol_{symbol}, security_exchange_{security_exchange} {}

  operator value_type() const {
    return {
        .symbol = symbol_,
        .security_exchange = security_exchange_,
    };
  }

 private:
  std::string const symbol_;
  std::string const security_exchange_;
};

struct MarketDataRequest final : public Encodeable {
  using value_type = roq::codec::fix::MarketDataRequest;

  explicit MarketDataRequest(value_type const &value)
      : md_req_id_{value.md_req_id}, subscription_request_type_{value.subscription_request_type},
        market_depth_{value.market_depth}, md_update_type_{value.md_update_type},
        aggregated_book_{value.aggregated_book},
        no_md_entry_types_{std::begin(value.no_md_entry_types), std::end(value.no_md_entry_types)},
        no_related_sym_{create<decltype(no_related_sym_)>(value.no_related_sym)},
        no_related_sym_2_{create_2<decltype(no_related_sym_2_)>(no_related_sym_)}
  // no_trading_sessions_{value.no_trading_sessions}
  {}

  MarketDataRequest(
      std::string_view const &md_req_id,
      roq::fix::SubscriptionRequestType const &subscription_request_type,
      uint32_t const &market_depth,
      roq::fix::MDUpdateType const &md_update_type,
      bool const &aggregated_book,
      std::vector<roq::fix::MDEntryType> const &no_md_entry_types,
      std::vector<roq::python::codec::fix::InstrmtMDReq> const &no_related_sym
      // std::span<TradingSession const> const &no_trading_sessions,
      )
      : md_req_id_{md_req_id}, subscription_request_type_{subscription_request_type}, market_depth_{market_depth},
        md_update_type_{md_update_type}, aggregated_book_{aggregated_book},
        no_md_entry_types_{std::begin(no_md_entry_types), std::end(no_md_entry_types)},
        no_related_sym_{create<decltype(no_related_sym_)>(no_related_sym)},
        no_related_sym_2_{create_2<decltype(no_related_sym_2_)>(no_related_sym_)}
  // no_trading_sessions_{no_trading_sessions}
  {}

  operator value_type() const {
    return {
        .md_req_id = md_req_id_,
        .subscription_request_type = subscription_request_type_,
        .market_depth = market_depth_,
        .md_update_type = md_update_type_,
        .aggregated_book = aggregated_book_,
        .no_md_entry_types = no_md_entry_types_,
        .no_related_sym = no_related_sym_2_,
        .no_trading_sessions = {},  // no_trading_sessions_,
        .custom_type = {},
        .custom_value = {},
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

  template <typename R>
  static R create(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      value_type item_2{item};
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

  template <typename R>
  static R create_2(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      auto item_2 = static_cast<value_type>(item);
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

 private:
  std::string const md_req_id_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
  uint32_t const market_depth_;
  roq::fix::MDUpdateType const md_update_type_;
  bool const aggregated_book_;
  std::vector<roq::codec::fix::MDReq> const no_md_entry_types_;
  std::vector<InstrmtMDReq> const no_related_sym_;
  std::vector<roq::codec::fix::InstrmtMDReq> const no_related_sym_2_;
  // std::span<TradingSession const> const no_trading_sessions_;
};

struct MarketDataRequestReject final : public Encodeable {
  using value_type = roq::codec::fix::MarketDataRequestReject;

  explicit MarketDataRequestReject(value_type const &value)
      : md_req_id_{value.md_req_id}, md_req_rej_reason_{value.md_req_rej_reason}, text_{value.text} {}

  operator value_type() const {
    return {
        .md_req_id = md_req_id_,
        .md_req_rej_reason = md_req_rej_reason_,
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string md_req_id_;
  roq::fix::MDReqRejReason md_req_rej_reason_;
  std::string text_;
};

struct MDFull final {
  using value_type = roq::codec::fix::MDFull;

  explicit MDFull(value_type const &value)
      : md_entry_type_{value.md_entry_type}, md_entry_px_{value.md_entry_px}, md_entry_size_{value.md_entry_size},
        md_entry_date_{value.md_entry_date}, md_entry_time_{value.md_entry_time},
        trading_session_id_{value.trading_session_id}, expire_time_{value.expire_time}, order_id_{value.order_id},
        number_of_orders_{value.number_of_orders}, md_entry_position_no_{value.md_entry_position_no} {}

  operator value_type() const {
    return {
        .md_entry_type = md_entry_type_,
        .md_entry_px = md_entry_px_,
        .md_entry_size = md_entry_size_,
        .md_entry_date = md_entry_date_,
        .md_entry_time = md_entry_time_,
        .trading_session_id = trading_session_id_,
        .expire_time = expire_time_,
        .order_id = order_id_,
        .number_of_orders = number_of_orders_,
        .md_entry_position_no = md_entry_position_no_,
    };
  };

 private:
  roq::fix::MDEntryType const md_entry_type_;
  Decimal const md_entry_px_;
  Decimal const md_entry_size_;
  std::chrono::year_month_day const md_entry_date_;
  std::chrono::hh_mm_ss<std::chrono::milliseconds> const md_entry_time_;
  std::string const trading_session_id_;
  std::string const expire_time_;
  std::string const order_id_;
  uint32_t const number_of_orders_;
  uint32_t const md_entry_position_no_;
};

struct MDInc final {
  using value_type = roq::codec::fix::MDInc;

  explicit MDInc(value_type const &value)
      : md_update_action_{value.md_update_action}, md_entry_type_{value.md_entry_type}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, md_entry_px_{value.md_entry_px},
        md_entry_size_{value.md_entry_size}, md_entry_date_{value.md_entry_date}, md_entry_time_{value.md_entry_time},
        trading_session_id_{value.trading_session_id}, expire_time_{value.expire_time}, order_id_{value.order_id},
        number_of_orders_{value.number_of_orders}, md_entry_position_no_{value.md_entry_position_no},
        text_{value.text} {}

  operator value_type() const {
    return {
        .md_update_action = md_update_action_,
        .md_entry_type = md_entry_type_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .md_entry_px = md_entry_px_,
        .md_entry_size = md_entry_size_,
        .md_entry_date = md_entry_date_,
        .md_entry_time = md_entry_time_,
        .trading_session_id = trading_session_id_,
        .expire_time = expire_time_,
        .order_id = order_id_,
        .number_of_orders = number_of_orders_,
        .md_entry_position_no = md_entry_position_no_,
        .text = text_,
    };
  };

 private:
  roq::fix::MDUpdateAction const md_update_action_;
  roq::fix::MDEntryType const md_entry_type_;
  std::string const symbol_;
  std::string const security_exchange_;
  Decimal const md_entry_px_;
  Decimal const md_entry_size_;
  std::chrono::year_month_day const md_entry_date_;
  std::chrono::hh_mm_ss<std::chrono::milliseconds> const md_entry_time_;
  std::string const trading_session_id_;
  std::string const expire_time_;
  std::string const order_id_;
  uint32_t const number_of_orders_;
  uint32_t const md_entry_position_no_;
  std::string const text_;
};

struct MarketDataSnapshotFullRefresh final : public Encodeable {
  using value_type = roq::codec::fix::MarketDataSnapshotFullRefresh;

  explicit MarketDataSnapshotFullRefresh(value_type const &value)
      : md_req_id_{value.md_req_id}, symbol_{value.symbol}, security_exchange_{value.security_exchange},
        no_md_entries_{create<decltype(no_md_entries_)>(value.no_md_entries)},
        no_md_entries_2_{create_2<decltype(no_md_entries_2_)>(no_md_entries_)} {}

  operator value_type() const {
    return {
        .md_req_id = md_req_id_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .no_md_entries = no_md_entries_2_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

  template <typename R>
  static R create(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      value_type item_2{item};
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

  template <typename R>
  static R create_2(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      auto item_2 = static_cast<value_type>(item);
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

  // private:
 public:  // XXX FIXME
  std::string const md_req_id_;
  std::string const symbol_;
  std::string const security_exchange_;
  std::vector<MDFull> const no_md_entries_;
  std::vector<roq::codec::fix::MDFull> const no_md_entries_2_;
};

struct MarketDataIncrementalRefresh final : public Encodeable {
  using value_type = roq::codec::fix::MarketDataIncrementalRefresh;

  explicit MarketDataIncrementalRefresh(value_type const &value)
      : md_req_id_{value.md_req_id}, no_md_entries_{create<decltype(no_md_entries_)>(value.no_md_entries)},
        no_md_entries_2_{create_2<decltype(no_md_entries_2_)>(no_md_entries_)} {}

  operator value_type() const {
    return {
        .md_req_id = md_req_id_,
        .no_md_entries = no_md_entries_2_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

  template <typename R>
  static R create(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      value_type item_2{item};
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

  template <typename R>
  static R create_2(auto const &value) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    using value_type = result_type::value_type;
    for (auto &item : value) {
      auto item_2 = static_cast<value_type>(item);
      result.emplace_back(std::move(item_2));
    }
    return result;
  }

  // private:
 public:  // XXX FIXME
  std::string const md_req_id_;
  std::vector<MDInc> const no_md_entries_;
  std::vector<roq::codec::fix::MDInc> const no_md_entries_2_;
};

struct OrderStatusRequest final : public Encodeable {
  using value_type = roq::codec::fix::OrderStatusRequest;

  explicit OrderStatusRequest(value_type const &value)
      : order_id_{value.order_id}, cl_ord_id_{value.cl_ord_id},
        // no_party_ids_{value.no_party_ids},
        ord_status_req_id_{value.ord_status_req_id}, account_{value.account}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, side_{value.side} {}

  OrderStatusRequest(
      std::string_view const &order_id,
      std::string_view const &cl_ord_id,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &ord_status_req_id,
      std::string_view const &account,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side)
      : order_id_{order_id}, cl_ord_id_{cl_ord_id},
        // no_party_ids_{no_party_ids},
        ord_status_req_id_{ord_status_req_id}, account_{account}, symbol_{symbol},
        security_exchange_{security_exchange}, side_{side} {}

  operator value_type() const {
    return {
        .order_id = order_id_,
        .cl_ord_id = cl_ord_id_,
        .no_party_ids = {},  // no_party_ids_,
        .ord_status_req_id = ord_status_req_id_,
        .account = account_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .side = side_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const order_id_;
  std::string const cl_ord_id_;
  // std::span<Party const> no_party_ids_;
  std::string const ord_status_req_id_;
  std::string const account_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::Side const side_;
};

struct OrderMassStatusRequest final : public Encodeable {
  using value_type = roq::codec::fix::OrderMassStatusRequest;

  explicit OrderMassStatusRequest(value_type const &value)
      : mass_status_req_id_{value.mass_status_req_id}, mass_status_req_type_{value.mass_status_req_type},
        // no_party_ids_{value.no_party_ids},
        account_{value.account}, trading_session_id_{value.trading_session_id}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, side_{value.side} {}

  OrderMassStatusRequest(
      std::string_view const &mass_status_req_id,
      roq::fix::MassStatusReqType const &mass_status_req_type,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &account,
      std::string_view const &trading_session_id,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side)
      : mass_status_req_id_{mass_status_req_id}, mass_status_req_type_{mass_status_req_type},
        // no_party_ids_{no_party_ids},
        account_{account}, trading_session_id_{trading_session_id}, symbol_{symbol},
        security_exchange_{security_exchange}, side_{side} {}

  operator value_type() const {
    return {
        .mass_status_req_id = mass_status_req_id_,
        .mass_status_req_type = mass_status_req_type_,
        .no_party_ids = {},  // no_party_ids_,
        .account = account_,
        .trading_session_id = trading_session_id_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .side = side_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const mass_status_req_id_;
  roq::fix::MassStatusReqType const mass_status_req_type_;
  // std::span<Party const> const no_party_ids_;
  std::string const account_;
  std::string const trading_session_id_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::Side const side_;
};

struct NewOrderSingle final : public Encodeable {
  using value_type = roq::codec::fix::NewOrderSingle;

  explicit NewOrderSingle(value_type const &value)
      : cl_ord_id_{value.cl_ord_id}, secondary_cl_ord_id_{value.secondary_cl_ord_id},
        // no_party_ids_{value.no_party_ids},
        account_{value.account}, handl_inst_{value.handl_inst}, exec_inst_{value.exec_inst},
        // no_trading_sessions_{value.no_trading_sessions},
        symbol_{value.symbol}, security_exchange_{value.security_exchange}, side_{value.side},
        transact_time_{value.transact_time}, order_qty_{value.order_qty}, ord_type_{value.ord_type},
        price_{value.price}, stop_px_{value.stop_px}, time_in_force_{value.time_in_force}, text_{value.text},
        position_effect_{value.position_effect}, max_show_{value.max_show} {}

  NewOrderSingle(
      std::string_view const &cl_ord_id,
      std::string_view const &secondary_cl_ord_id,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &account,
      roq::fix::HandlInst const &handl_inst,
      std::string_view const &exec_inst,
      // std::span<TradingSession const> const &no_trading_sessions,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side,
      std::chrono::milliseconds const &transact_time,
      Decimal const &order_qty,
      roq::fix::OrdType const &ord_type,
      Decimal const &price,
      Decimal const &stop_px,
      roq::fix::TimeInForce const &time_in_force,
      std::string_view const &text,
      roq::fix::PositionEffect const &position_effect,
      Decimal const &max_show)
      : cl_ord_id_{cl_ord_id}, secondary_cl_ord_id_{secondary_cl_ord_id},
        // no_party_ids_{no_party_ids},
        account_{account}, handl_inst_{handl_inst}, exec_inst_{exec_inst},
        // no_trading_sessions_{no_trading_sessions},
        symbol_{symbol}, security_exchange_{security_exchange}, side_{side}, transact_time_{transact_time},
        order_qty_{order_qty}, ord_type_{ord_type}, price_{price}, stop_px_{stop_px}, time_in_force_{time_in_force},
        text_{text}, position_effect_{position_effect}, max_show_{max_show} {}

  // XXX
  NewOrderSingle(
      std::string_view const &cl_ord_id,
      std::string_view const &secondary_cl_ord_id,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &account,
      roq::fix::HandlInst const &handl_inst,
      std::string_view const &exec_inst,
      // std::span<TradingSession const> const &no_trading_sessions,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side,
      std::chrono::system_clock::time_point transact_time,  // XXX
      double order_qty,
      roq::fix::OrdType const &ord_type,
      double price,
      double stop_px,
      roq::fix::TimeInForce const &time_in_force,
      std::string_view const &text,
      roq::fix::PositionEffect const &position_effect,
      double max_show)
      : cl_ord_id_{cl_ord_id}, secondary_cl_ord_id_{secondary_cl_ord_id},
        // no_party_ids_{no_party_ids},
        account_{account}, handl_inst_{handl_inst}, exec_inst_{exec_inst},
        // no_trading_sessions_{no_trading_sessions},
        symbol_{symbol}, security_exchange_{security_exchange}, side_{side}, transact_time_{DateTime{transact_time}},
        order_qty_{order_qty, {}}, ord_type_{ord_type}, price_{price, {}}, stop_px_{stop_px, {}},
        time_in_force_{time_in_force}, text_{text}, position_effect_{position_effect}, max_show_{max_show, {}} {}

  operator value_type() const {
    return {
        .cl_ord_id = cl_ord_id_,
        .secondary_cl_ord_id = secondary_cl_ord_id_,
        .no_party_ids = {},  // no_party_ids_,
        .account = account_,
        .handl_inst = handl_inst_,
        .exec_inst = exec_inst_,
        .no_trading_sessions = {},  // no_trading_sessions_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .side = side_,
        .transact_time = transact_time_,
        .order_qty = order_qty_,
        .ord_type = ord_type_,
        .price = price_,
        .stop_px = stop_px_,
        .time_in_force = time_in_force_,
        .text = text_,
        .position_effect = position_effect_,
        .max_show = max_show_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const cl_ord_id_;
  std::string const secondary_cl_ord_id_;
  // std::span<Party const> const no_party_ids_;
  std::string const account_;
  roq::fix::HandlInst const handl_inst_;
  std::string const exec_inst_;
  // std::span<TradingSession const> const no_trading_sessions_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::Side const side_;
  std::chrono::milliseconds const transact_time_;
  Decimal const order_qty_;
  roq::fix::OrdType const ord_type_;
  Decimal const price_;
  Decimal const stop_px_;
  roq::fix::TimeInForce const time_in_force_;
  std::string const text_;
  roq::fix::PositionEffect const position_effect_;
  Decimal const max_show_;
};

struct OrderCancelRequest final : public Encodeable {
  using value_type = roq::codec::fix::OrderCancelRequest;

  explicit OrderCancelRequest(value_type const &value)
      : orig_cl_ord_id_{value.orig_cl_ord_id}, order_id_{value.order_id}, cl_ord_id_{value.cl_ord_id},
        secondary_cl_ord_id_{value.secondary_cl_ord_id}, account_{value.account},
        // no_party_ids_{value.no_party_ids},
        symbol_{value.symbol}, security_exchange_{value.security_exchange}, side_{value.side},
        transact_time_{value.transact_time}, order_qty_{value.order_qty}, text_{value.text} {}

  OrderCancelRequest(
      std::string_view const &orig_cl_ord_id,
      std::string_view const &order_id,
      std::string_view const &cl_ord_id,
      std::string_view const &secondary_cl_ord_id,
      std::string_view const &account,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side,
      std::chrono::milliseconds const &transact_time,
      Decimal const &order_qty,
      std::string_view const &text)
      : orig_cl_ord_id_{orig_cl_ord_id}, order_id_{order_id}, cl_ord_id_{cl_ord_id},
        secondary_cl_ord_id_{secondary_cl_ord_id}, account_{account},
        // no_party_ids_{no_party_ids},
        symbol_{symbol}, security_exchange_{security_exchange}, side_{side}, transact_time_{transact_time},
        order_qty_{order_qty}, text_{text} {}

  // XXX
  OrderCancelRequest(
      std::string_view const &orig_cl_ord_id,
      std::string_view const &order_id,
      std::string_view const &cl_ord_id,
      std::string_view const &secondary_cl_ord_id,
      std::string_view const &account,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side,
      std::chrono::system_clock::time_point transact_time,  // XXX
      double order_qty,
      std::string_view const &text)
      : orig_cl_ord_id_{orig_cl_ord_id}, order_id_{order_id}, cl_ord_id_{cl_ord_id},
        secondary_cl_ord_id_{secondary_cl_ord_id}, account_{account},
        // no_party_ids_{no_party_ids},
        symbol_{symbol}, security_exchange_{security_exchange}, side_{side}, transact_time_{DateTime{transact_time}},
        order_qty_{order_qty, {}}, text_{text} {}

  operator value_type() const {
    return {
        .orig_cl_ord_id = orig_cl_ord_id_,
        .order_id = order_id_,
        .cl_ord_id = cl_ord_id_,
        .secondary_cl_ord_id = secondary_cl_ord_id_,
        .account = account_,
        .no_party_ids = {},  // no_party_ids_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .side = side_,
        .transact_time = transact_time_,
        .order_qty = order_qty_,
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const orig_cl_ord_id_;
  std::string const order_id_;
  std::string const cl_ord_id_;
  std::string const secondary_cl_ord_id_;
  std::string const account_;
  // std::span<Party const> const no_party_ids_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::Side const side_;
  std::chrono::milliseconds const transact_time_;
  Decimal const order_qty_;
  std::string const text_;
};

struct OrderCancelReplaceRequest final : public Encodeable {
  using value_type = roq::codec::fix::OrderCancelReplaceRequest;

  explicit OrderCancelReplaceRequest(value_type const &value)
      : order_id_{value.order_id},
        // no_party_ids_{value.no_party_ids},
        orig_cl_ord_id_{value.orig_cl_ord_id}, cl_ord_id_{value.cl_ord_id},
        secondary_cl_ord_id_{value.secondary_cl_ord_id}, account_{value.account}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, order_qty_{value.order_qty}, price_{value.price},
        side_{value.side}, transact_time_{value.transact_time}, ord_type_{value.ord_type} {}

  OrderCancelReplaceRequest(
      std::string_view const &order_id,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &orig_cl_ord_id,
      std::string_view const &cl_ord_id,
      std::string_view const &secondary_cl_ord_id,
      std::string_view const &account,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      Decimal const &order_qty,
      Decimal const &price,
      roq::fix::Side const &side,
      std::chrono::milliseconds const &transact_time,
      roq::fix::OrdType const &ord_type)
      : order_id_{order_id},
        // no_party_ids_{no_party_ids},
        orig_cl_ord_id_{orig_cl_ord_id}, cl_ord_id_{cl_ord_id}, secondary_cl_ord_id_{secondary_cl_ord_id},
        account_{account}, symbol_{symbol}, security_exchange_{security_exchange}, order_qty_{order_qty}, price_{price},
        side_{side}, transact_time_{transact_time}, ord_type_{ord_type} {}

  // XXX
  OrderCancelReplaceRequest(
      std::string_view const &order_id,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &orig_cl_ord_id,
      std::string_view const &cl_ord_id,
      std::string_view const &secondary_cl_ord_id,
      std::string_view const &account,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      double order_qty,
      double price,
      roq::fix::Side const &side,
      std::chrono::system_clock::time_point transact_time,  // XXX
      roq::fix::OrdType const &ord_type)
      : order_id_{order_id},
        // no_party_ids_{no_party_ids},
        orig_cl_ord_id_{orig_cl_ord_id}, cl_ord_id_{cl_ord_id}, secondary_cl_ord_id_{secondary_cl_ord_id},
        account_{account}, symbol_{symbol}, security_exchange_{security_exchange}, order_qty_{order_qty, {}},
        price_{price, {}}, side_{side}, transact_time_{DateTime{transact_time}}, ord_type_{ord_type} {}

  operator value_type() const {
    return {
        .order_id = order_id_,
        .no_party_ids = {},  // no_party_ids_,
        .orig_cl_ord_id = orig_cl_ord_id_,
        .cl_ord_id = cl_ord_id_,
        .secondary_cl_ord_id = secondary_cl_ord_id_,
        .account = account_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .order_qty = order_qty_,
        .price = price_,
        .side = side_,
        .transact_time = transact_time_,
        .ord_type = ord_type_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const order_id_;
  // std::span<Party const> const no_party_ids_;
  std::string const orig_cl_ord_id_;
  std::string const cl_ord_id_;
  std::string const secondary_cl_ord_id_;
  std::string const account_;
  std::string const symbol_;
  std::string const security_exchange_;
  Decimal const order_qty_;
  Decimal const price_;
  roq::fix::Side const side_;
  std::chrono::milliseconds const transact_time_;
  roq::fix::OrdType const ord_type_;
};

struct OrderMassCancelRequest final : public Encodeable {
  using value_type = roq::codec::fix::OrderMassCancelRequest;

  explicit OrderMassCancelRequest(value_type const &value)
      : cl_ord_id_{value.cl_ord_id}, mass_cancel_request_type_{value.mass_cancel_request_type},
        trading_session_id_{value.trading_session_id}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, side_{value.side},
        transact_time_{value.transact_time}  // no_party_ids_{value.no_party_ids}
  {}

  OrderMassCancelRequest(
      std::string_view const &cl_ord_id,
      roq::fix::MassCancelRequestType const &mass_cancel_request_type,
      std::string_view const &trading_session_id,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side,
      std::chrono::milliseconds const &transact_time
      // std::span<Party const> const &no_party_ids
      )
      : cl_ord_id_{cl_ord_id}, mass_cancel_request_type_{mass_cancel_request_type},
        trading_session_id_{trading_session_id}, symbol_{symbol}, security_exchange_{security_exchange}, side_{side},
        transact_time_{transact_time}  // no_party_ids_{no_party_ids}
  {}

  // XXX
  OrderMassCancelRequest(
      std::string_view const &cl_ord_id,
      roq::fix::MassCancelRequestType const &mass_cancel_request_type,
      std::string_view const &trading_session_id,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      roq::fix::Side const &side,
      std::chrono::system_clock::time_point transact_time  // XXX std::span<Party const> const &no_party_ids
      )
      : cl_ord_id_{cl_ord_id}, mass_cancel_request_type_{mass_cancel_request_type},
        trading_session_id_{trading_session_id}, symbol_{symbol}, security_exchange_{security_exchange}, side_{side},
        transact_time_{DateTime{transact_time}}  // no_party_ids_{no_party_ids}
  {}

  operator value_type() const {
    return {
        .cl_ord_id = cl_ord_id_,
        .mass_cancel_request_type = mass_cancel_request_type_,
        .trading_session_id = trading_session_id_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .side = side_,
        .transact_time = transact_time_,
        .no_party_ids = {},  // no_party_ids_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const cl_ord_id_;
  roq::fix::MassCancelRequestType const mass_cancel_request_type_;
  std::string const trading_session_id_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::Side const side_;
  std::chrono::milliseconds const transact_time_;
  // std::span<Party const> const no_party_ids_;
};

struct OrderCancelReject final : public Encodeable {
  using value_type = roq::codec::fix::OrderCancelReject;

  explicit OrderCancelReject(value_type const &value)
      : order_id_{value.order_id}, secondary_cl_ord_id_{value.secondary_cl_ord_id}, cl_ord_id_{value.cl_ord_id},
        orig_cl_ord_id_{value.orig_cl_ord_id}, ord_status_{value.ord_status},
        working_indicator_{value.working_indicator}, account_{value.account},
        cxl_rej_response_to_{value.cxl_rej_response_to}, cxl_rej_reason_{value.cxl_rej_reason}, text_{value.text} {}

  operator value_type() const {
    return {
        .order_id = order_id_,
        .secondary_cl_ord_id = secondary_cl_ord_id_,
        .cl_ord_id = cl_ord_id_,
        .orig_cl_ord_id = orig_cl_ord_id_,
        .ord_status = ord_status_,
        .working_indicator = working_indicator_,
        .account = account_,
        .cxl_rej_response_to = cxl_rej_response_to_,
        .cxl_rej_reason = cxl_rej_reason_,
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const order_id_;
  std::string const secondary_cl_ord_id_;
  std::string const cl_ord_id_;
  std::string const orig_cl_ord_id_;
  roq::fix::OrdStatus const ord_status_;
  bool const working_indicator_;
  std::string const account_;
  roq::fix::CxlRejResponseTo const cxl_rej_response_to_;
  roq::fix::CxlRejReason const cxl_rej_reason_;
  std::string const text_;
};

struct OrderMassCancelReport final : public Encodeable {
  using value_type = roq::codec::fix::OrderMassCancelReport;

  explicit OrderMassCancelReport(value_type const &value)
      : cl_ord_id_{value.cl_ord_id}, order_id_{value.order_id},
        mass_cancel_request_type_{value.mass_cancel_request_type}, mass_cancel_response_{value.mass_cancel_response},
        mass_cancel_reject_reason_{value.mass_cancel_reject_reason},
        total_affected_orders_{value.total_affected_orders}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, side_{value.side},
        text_{value.text}  // no_party_ids_{value.no_party_ids}
  {}

  operator value_type() const {
    return {
        .cl_ord_id = cl_ord_id_,
        .order_id = order_id_,
        .mass_cancel_request_type = mass_cancel_request_type_,
        .mass_cancel_response = mass_cancel_response_,
        .mass_cancel_reject_reason = mass_cancel_reject_reason_,
        .total_affected_orders = total_affected_orders_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .side = side_,
        .text = text_,
        .no_party_ids = {},  // no_party_ids_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const cl_ord_id_;
  std::string const order_id_;
  roq::fix::MassCancelRequestType const mass_cancel_request_type_;
  roq::fix::MassCancelResponse const mass_cancel_response_;
  roq::fix::MassCancelRejectReason const mass_cancel_reject_reason_;
  uint32_t const total_affected_orders_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::Side const side_;
  std::string const text_;
  // std::span<Party const> const no_party_ids_;
};

struct ExecutionReport final : public Encodeable {
  using value_type = roq::codec::fix::ExecutionReport;

  explicit ExecutionReport(value_type const &value)
      : order_id_{value.order_id}, secondary_cl_ord_id_{value.secondary_cl_ord_id}, cl_ord_id_{value.cl_ord_id},
        orig_cl_ord_id_{value.orig_cl_ord_id}, ord_status_req_id_{value.ord_status_req_id},
        mass_status_req_id_{value.mass_status_req_id}, tot_num_reports_{value.tot_num_reports},
        last_rpt_requested_{value.last_rpt_requested},
        // no_party_ids_{value.no_party_ids},
        exec_id_{value.exec_id}, exec_type_{value.exec_type}, ord_status_{value.ord_status},
        working_indicator_{value.working_indicator}, ord_rej_reason_{value.ord_rej_reason}, account_{value.account},
        account_type_{value.account_type}, symbol_{value.symbol}, security_exchange_{value.security_exchange},
        side_{value.side}, ord_type_{value.ord_type}, order_qty_{value.order_qty}, price_{value.price},
        stop_px_{value.stop_px}, currency_{value.currency}, time_in_force_{value.time_in_force},
        exec_inst_{value.exec_inst}, last_qty_{value.last_qty}, last_px_{value.last_px},
        trading_session_id_{value.trading_session_id}, leaves_qty_{value.leaves_qty}, cum_qty_{value.cum_qty},
        avg_px_{value.avg_px}, transact_time_{value.transact_time}, position_effect_{value.position_effect},
        max_show_{value.max_show}, text_{value.text}, last_liquidity_ind_{value.last_liquidity_ind} {}

  operator value_type() const {
    return {
        .order_id = order_id_,
        .secondary_cl_ord_id = secondary_cl_ord_id_,
        .cl_ord_id = cl_ord_id_,
        .orig_cl_ord_id = orig_cl_ord_id_,
        .ord_status_req_id = ord_status_req_id_,
        .mass_status_req_id = mass_status_req_id_,
        .tot_num_reports = tot_num_reports_,
        .last_rpt_requested = last_rpt_requested_,
        .no_party_ids = {},
        .exec_id = exec_id_,
        .exec_type = exec_type_,
        .ord_status = ord_status_,
        .working_indicator = working_indicator_,
        .ord_rej_reason = ord_rej_reason_,
        .account = account_,
        .account_type = account_type_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .side = side_,
        .ord_type = ord_type_,
        .order_qty = order_qty_,
        .price = price_,
        .stop_px = stop_px_,
        .currency = currency_,
        .time_in_force = time_in_force_,
        .exec_inst = exec_inst_,
        .last_qty = last_qty_,
        .last_px = last_px_,
        .trading_session_id = trading_session_id_,
        .leaves_qty = leaves_qty_,
        .cum_qty = cum_qty_,
        .avg_px = avg_px_,
        .transact_time = transact_time_,
        .position_effect = position_effect_,
        .max_show = max_show_,
        .text = text_,
        .last_liquidity_ind = last_liquidity_ind_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const order_id_;
  std::string const secondary_cl_ord_id_;
  std::string const cl_ord_id_;
  std::string const orig_cl_ord_id_;
  std::string const ord_status_req_id_;
  std::string const mass_status_req_id_;
  uint32_t const tot_num_reports_;
  bool const last_rpt_requested_;
  // std::vector<Party const> no_party_ids_;
  std::string const exec_id_;
  roq::fix::ExecType const exec_type_;
  roq::fix::OrdStatus const ord_status_;
  bool const working_indicator_;
  roq::fix::OrdRejReason const ord_rej_reason_;
  std::string const account_;
  roq::fix::AccountType const account_type_;
  std::string const symbol_;
  std::string const security_exchange_;
  roq::fix::Side const side_;
  roq::fix::OrdType const ord_type_;
  Decimal const order_qty_;
  Decimal const price_;
  Decimal const stop_px_;
  std::string const currency_;
  roq::fix::TimeInForce const time_in_force_;
  std::string const exec_inst_;
  Decimal const last_qty_;
  Decimal const last_px_;
  std::string const trading_session_id_;
  Decimal const leaves_qty_;
  Decimal const cum_qty_;
  Decimal const avg_px_;
  std::chrono::milliseconds const transact_time_;
  roq::fix::PositionEffect const position_effect_;
  Decimal const max_show_;
  std::string const text_;
  roq::fix::LastLiquidityInd const last_liquidity_ind_;
};

struct TradeCaptureReportRequest final : public Encodeable {
  using value_type = roq::codec::fix::TradeCaptureReportRequest;

  explicit TradeCaptureReportRequest(value_type const &value)
      : trade_request_id_{value.trade_request_id}, trade_request_type_{value.trade_request_type},
        subscription_request_type_{value.subscription_request_type}, order_id_{value.order_id},
        cl_ord_id_{value.cl_ord_id},
        // no_party_ids_{value.no_party_ids},
        symbol_{value.symbol}, security_exchange_{value.security_exchange} {}

  TradeCaptureReportRequest(
      std::string_view const &trade_request_id,
      roq::fix::TradeRequestType const &trade_request_type,
      roq::fix::SubscriptionRequestType const &subscription_request_type,
      std::string_view const &order_id,
      std::string_view const &cl_ord_id,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &symbol,
      std::string_view const &security_exchange)
      : trade_request_id_{trade_request_id}, trade_request_type_{trade_request_type},
        subscription_request_type_{subscription_request_type}, order_id_{order_id}, cl_ord_id_{cl_ord_id},
        // no_party_ids_{no_party_ids},
        symbol_{symbol}, security_exchange_{security_exchange} {}

  operator value_type() const {
    return {
        .trade_request_id = trade_request_id_,
        .trade_request_type = trade_request_type_,
        .subscription_request_type = subscription_request_type_,
        .order_id = order_id_,
        .cl_ord_id = cl_ord_id_,
        .no_party_ids = {},  // no_party_ids_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const trade_request_id_;
  roq::fix::TradeRequestType const trade_request_type_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
  std::string const order_id_;
  std::string const cl_ord_id_;
  // std::span<Party const> const no_party_ids_;
  std::string const symbol_;
  std::string const security_exchange_;
};

struct TradeCaptureReport final : public Encodeable {
  using value_type = roq::codec::fix::TradeCaptureReport;

  explicit TradeCaptureReport(value_type const &value)
      : trade_report_id_{value.trade_report_id}, trade_request_id_{value.trade_request_id}, exec_type_{value.exec_type},
        tot_num_trade_reports_{value.tot_num_trade_reports}, last_rpt_requested_{value.last_rpt_requested},
        unsolicited_indicator_{value.unsolicited_indicator}, trd_match_id_{value.trd_match_id}, exec_id_{value.exec_id},
        previously_reported_{value.previously_reported}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, last_qty_{value.last_qty}, last_px_{value.last_px},
        trade_date_{value.trade_date}, transact_time_{value.transact_time}  // no_sides_{value.no_sides}
  {}

  operator value_type() const {
    return {
        .trade_report_id = trade_report_id_,
        .trade_request_id = trade_request_id_,
        .exec_type = exec_type_,
        .tot_num_trade_reports = tot_num_trade_reports_,
        .last_rpt_requested = last_rpt_requested_,
        .unsolicited_indicator = unsolicited_indicator_,
        .trd_match_id = trd_match_id_,
        .exec_id = exec_id_,
        .previously_reported = previously_reported_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .last_qty = last_qty_,
        .last_px = last_px_,
        .trade_date = trade_date_,
        .transact_time = transact_time_,
        .no_sides = {},  // no_sides_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const trade_report_id_;
  std::string const trade_request_id_;
  roq::fix::ExecType const exec_type_;
  uint32_t const tot_num_trade_reports_;
  bool const last_rpt_requested_;
  bool const unsolicited_indicator_;
  std::string const trd_match_id_;
  std::string const exec_id_;
  bool const previously_reported_;
  std::string const symbol_;
  std::string const security_exchange_;
  Decimal const last_qty_;
  Decimal const last_px_;
  std::chrono::year_month_day const trade_date_;
  std::chrono::milliseconds const transact_time_;
  // std::span<TrdCapRptSide const> const no_sides_;
};

struct RequestForPositions final : public Encodeable {
  using value_type = roq::codec::fix::RequestForPositions;

  explicit RequestForPositions(value_type const &value)
      : pos_req_id_{value.pos_req_id}, pos_req_type_{value.pos_req_type},
        subscription_request_type_{value.subscription_request_type},
        // no_party_ids_{value.no_party_ids},
        account_{value.account}, account_type_{value.account_type}, currency_{value.currency}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, clearing_business_date_{value.clearing_business_date},
        // no_trading_sessions_{value.no_trading_sessions},
        transact_time_{value.transact_time} {}

  RequestForPositions(
      std::string_view const &pos_req_id,
      roq::fix::PosReqType const &pos_req_type,
      roq::fix::SubscriptionRequestType const &subscription_request_type,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &account,
      roq::fix::AccountType const &account_type,
      std::string_view const &currency,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      std::chrono::year_month_day const &clearing_business_date,
      // std::span<TradingSession const> const &no_trading_sessions,
      std::chrono::milliseconds const &transact_time)
      : pos_req_id_{pos_req_id}, pos_req_type_{pos_req_type}, subscription_request_type_{subscription_request_type},
        // no_party_ids_{no_party_ids},
        account_{account}, account_type_{account_type}, currency_{currency}, symbol_{symbol},
        security_exchange_{security_exchange}, clearing_business_date_{clearing_business_date},
        // no_trading_sessions_{no_trading_sessions},
        transact_time_{transact_time} {}

  // XXX
  RequestForPositions(
      std::string_view const &pos_req_id,
      roq::fix::PosReqType const &pos_req_type,
      roq::fix::SubscriptionRequestType const &subscription_request_type,
      // std::span<Party const> const &no_party_ids,
      std::string_view const &account,
      roq::fix::AccountType const &account_type,
      std::string_view const &currency,
      std::string_view const &symbol,
      std::string_view const &security_exchange,
      std::chrono::system_clock::time_point clearing_business_date,
      // std::span<TradingSession const> const &no_trading_sessions,
      std::chrono::system_clock::time_point transact_time)
      : pos_req_id_{pos_req_id}, pos_req_type_{pos_req_type}, subscription_request_type_{subscription_request_type},
        // no_party_ids_{no_party_ids},
        account_{account}, account_type_{account_type}, currency_{currency}, symbol_{symbol},
        security_exchange_{security_exchange}, clearing_business_date_{DateTime{clearing_business_date}},
        // no_trading_sessions_{no_trading_sessions},
        transact_time_{DateTime{transact_time}} {}

  operator value_type() const {
    return {
        .pos_req_id = pos_req_id_,
        .pos_req_type = pos_req_type_,
        .subscription_request_type = subscription_request_type_,
        .no_party_ids = {},  // no_party_ids_,
        .account = account_,
        .account_type = account_type_,
        .currency = currency_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .clearing_business_date = clearing_business_date_,
        .no_trading_sessions = {},  // no_trading_sessions_,
        .transact_time = transact_time_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const pos_req_id_;
  roq::fix::PosReqType const pos_req_type_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
  // std::span<Party const> const no_party_ids_;
  std::string const account_;
  roq::fix::AccountType const account_type_;
  std::string const currency_;
  std::string const symbol_;
  std::string const security_exchange_;
  std::chrono::year_month_day const clearing_business_date_;
  // std::span<TradingSession const> const no_trading_sessions_;
  std::chrono::milliseconds const transact_time_;
};

struct RequestForPositionsAck final : public Encodeable {
  using value_type = roq::codec::fix::RequestForPositionsAck;

  explicit RequestForPositionsAck(value_type const &value)
      : pos_maint_rpt_id_{value.pos_maint_rpt_id}, pos_req_id_{value.pos_req_id},
        total_num_pos_reports_{value.total_num_pos_reports}, unsolicited_indicator_{value.unsolicited_indicator},
        pos_req_result_{value.pos_req_result}, pos_req_status_{value.pos_req_status},
        // no_party_ids_{value.no_party_ids},
        account_{value.account}, account_type_{value.account_type}, text_{value.text} {}

  operator value_type() const {
    return {
        .pos_maint_rpt_id = pos_maint_rpt_id_,
        .pos_req_id = pos_req_id_,
        .total_num_pos_reports = total_num_pos_reports_,
        .unsolicited_indicator = unsolicited_indicator_,
        .pos_req_result = pos_req_result_,
        .pos_req_status = pos_req_status_,
        .no_party_ids = {},  // no_party_ids_,
        .account = account_,
        .account_type = account_type_,
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const pos_maint_rpt_id_;
  std::string const pos_req_id_;
  uint32_t const total_num_pos_reports_;
  bool const unsolicited_indicator_;
  roq::fix::PosReqResult const pos_req_result_;
  roq::fix::PosReqStatus const pos_req_status_;
  // std::span<Party const> const no_party_ids_;
  std::string const account_;
  roq::fix::AccountType const account_type_;
  std::string const text_;
};

struct PositionReport final : public Encodeable {
  using value_type = roq::codec::fix::PositionReport;

  explicit PositionReport(value_type const &value)
      : pos_maint_rpt_id_{value.pos_maint_rpt_id}, pos_req_id_{value.pos_req_id}, pos_req_type_{value.pos_req_type},
        subscription_request_type_{value.subscription_request_type},
        total_num_pos_reports_{value.total_num_pos_reports}, unsolicited_indicator_{value.unsolicited_indicator},
        pos_req_result_{value.pos_req_result}, clearing_business_date_{value.clearing_business_date},
        // no_party_ids_{value.no_party_ids},
        account_{value.account}, account_type_{value.account_type}, symbol_{value.symbol},
        security_exchange_{value.security_exchange}, currency_{value.currency}, settl_price_{value.settl_price},
        settl_price_type_{value.settl_price_type}, prior_settl_price_{value.prior_settl_price},
        // no_positions_{value.no_positions},
        // no_pos_amt_{value.no_pos_amt},
        text_{value.text} {}

  operator value_type() const {
    return {
        .pos_maint_rpt_id = pos_maint_rpt_id_,
        .pos_req_id = pos_req_id_,
        .pos_req_type = pos_req_type_,
        .subscription_request_type = subscription_request_type_,
        .total_num_pos_reports = total_num_pos_reports_,
        .unsolicited_indicator = unsolicited_indicator_,
        .pos_req_result = pos_req_result_,
        .clearing_business_date = clearing_business_date_,
        .no_party_ids = {},  // no_party_ids_,
        .account = account_,
        .account_type = account_type_,
        .symbol = symbol_,
        .security_exchange = security_exchange_,
        .currency = currency_,
        .settl_price = settl_price_,
        .settl_price_type = settl_price_type_,
        .prior_settl_price = prior_settl_price_,
        .no_positions = {},  // no_positions_,
        .no_pos_amt = {},    // no_pos_amt_,
        .text = text_,
    };
  }

 protected:
  std::span<std::byte const> encode(Encoder &encoder, std::chrono::nanoseconds sending_time) const override {
    return encoder.encode(static_cast<value_type>(*this), sending_time);
  }

 private:
  std::string const pos_maint_rpt_id_;
  std::string const pos_req_id_;
  roq::fix::PosReqType const pos_req_type_;
  roq::fix::SubscriptionRequestType const subscription_request_type_;
  uint32_t const total_num_pos_reports_;
  bool const unsolicited_indicator_;
  roq::fix::PosReqResult const pos_req_result_;
  std::chrono::year_month_day const clearing_business_date_;
  // std::span<Party const> const no_party_ids_;
  std::string const account_;
  roq::fix::AccountType const account_type_;
  std::string const symbol_;
  std::string const security_exchange_;
  std::string const currency_;
  Decimal const settl_price_;
  roq::fix::SettlPriceType const settl_price_type_;
  Decimal const prior_settl_price_;
  // std::span<PositionQty const> const no_positions_;
  // std::span<PositionAmountData const> const no_pos_amt_;
  std::string const text_;
};

}  // namespace fix
}  // namespace codec
}  // namespace python
}  // namespace roq
