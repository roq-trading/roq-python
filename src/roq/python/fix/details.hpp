/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <pybind11/pybind11.h>

#include "roq/fix/reader.hpp"

#include "roq/codec/fix/decoder.hpp"
#include "roq/codec/fix/encoder.hpp"

namespace roq {
namespace python {
namespace fix {

struct Encoder;

struct Encodeable {
  virtual ~Encodeable() = default;
  virtual std::span<std::byte const> encode(Encoder &, std::chrono::nanoseconds sending_time) const = 0;
};

struct Encoder final {
  Encoder(std::string_view const &sender_comp_id, std::string_view const &target_comp_id)
      : encoder_{roq::codec::fix::Encoder::create()}, sender_comp_id_{sender_comp_id}, target_comp_id_{target_comp_id} {
  }

  template <typename T>
  std::span<std::byte const> encode(T const &value, std::chrono::nanoseconds sending_time) {
    auto header = roq::fix::Header{
        .version = roq::fix::Version::FIX_44,
        .msg_type = T::MSG_TYPE,
        .sender_comp_id = sender_comp_id_,
        .target_comp_id = target_comp_id_,
        .msg_seq_num = ++msg_seq_num_,
        .sending_time = sending_time,
    };
    return (*encoder_).encode(header, value);
  }

  std::span<std::byte const> encode(Encodeable const &encodeable, std::chrono::system_clock::time_point sending_time) {
    return encodeable.encode(*this, sending_time.time_since_epoch());
  }

 private:
  std::unique_ptr<roq::codec::fix::Encoder> encoder_;
  std::string const sender_comp_id_;
  std::string const target_comp_id_;
  uint64_t msg_seq_num_ = {};
};

struct Logon final : public Encodeable {
  using value_type = roq::codec::fix::Logon;

  explicit Logon(value_type const &value)
      : encrypt_method_{value.encrypt_method}, heart_bt_int_{value.heart_bt_int},
        reset_seq_num_flag_{value.reset_seq_num_flag}, next_expected_msg_seq_num_{value.next_expected_msg_seq_num},
        username_{value.username}, password_{value.password} {}

  Logon(
      roq::fix::EncryptMethod encrypt_method,
      std::chrono::seconds heart_bt_int,
      bool reset_seq_num_flag,
      uint64_t next_expected_msg_seq_num,
      std::string_view const &username,
      std::string_view const &password)
      : encrypt_method_{encrypt_method}, heart_bt_int_{heart_bt_int}, reset_seq_num_flag_{reset_seq_num_flag},
        next_expected_msg_seq_num_{next_expected_msg_seq_num}, username_{username}, password_{password} {}

  operator value_type() const {
    return {
        .encrypt_method = encrypt_method_,
        .heart_bt_int = static_cast<uint16_t>(heart_bt_int_.count()),
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

  operator value_type() const {
    return {
        .symbol = symbol,
        .contract_multiplier = {contract_multiplier, Decimals{}},
        .security_exchange = security_exchange,
        .min_trade_vol = {min_trade_vol, Decimals{}},
        .trading_session_id = trading_session_id,
    };
  };

  std::string symbol;
  double contract_multiplier = NaN;
  std::string security_exchange;
  double min_trade_vol = NaN;
  std::string trading_session_id;
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
  static R create(auto const &no_related_sym) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    auto convert = []<typename U>(U const &value) -> double {
      if constexpr (std::is_same<U, roq::utils::Number>::value) {
        return value.value;
      } else {
        return value;
      }
    };
    for (auto &item : no_related_sym) {
      auto sec_list_grp = SecListGrp{
          .symbol = std::string{item.symbol},
          .contract_multiplier = convert(item.contract_multiplier),
          .security_exchange = std::string{item.security_exchange},
          .min_trade_vol = convert(item.min_trade_vol),
          .trading_session_id = std::string{item.trading_session_id},
      };
      result.emplace_back(std::move(sec_list_grp));
    }
    return result;
  }

  template <typename R>
  static R create_2(auto const &no_related_sym) {
    using result_type = std::remove_cvref<R>::type;
    result_type result;
    for (auto &item : no_related_sym) {
      auto sec_list_grp = roq::codec::fix::SecListGrp{
          .symbol = item.symbol,
          .contract_multiplier = {item.contract_multiplier, Decimals{}},
          .security_exchange = item.security_exchange,
          .min_trade_vol = {item.min_trade_vol, Decimals{}},
          .trading_session_id = item.trading_session_id,
      };
      result.emplace_back(std::move(sec_list_grp));
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

struct Header final {
  using value_type = roq::fix::Header;

  explicit Header(value_type const &header)
      : msg_type_{header.msg_type}, sender_comp_id_{header.sender_comp_id}, target_comp_id_{header.target_comp_id},
        msg_seq_num_{header.msg_seq_num}, sending_time_{header.sending_time} {}

  operator value_type() const {
    return {
        .version = roq::fix::Version::FIX_44,
        .msg_type = msg_type_,
        .sender_comp_id = sender_comp_id_,
        .target_comp_id = target_comp_id_,
        .msg_seq_num = msg_seq_num_,
        .sending_time = sending_time_,
    };
  }

 private:
  roq::fix::MsgType const msg_type_;
  std::string const sender_comp_id_;
  std::string const target_comp_id_;
  uint64_t const msg_seq_num_;
  std::chrono::nanoseconds const sending_time_;
};

struct Decoder final {
  template <typename Callback>
  struct Handler final : public roq::codec::fix::Decoder::Handler {
    explicit Handler(Callback const &callback) : callback_{callback} {}

    template <typename T>
    void dispatch(auto &header, auto &value) {
      Header header_2{header};
      T value_2{value};
      auto arg0 = pybind11::cast(header_2);
      auto arg1 = pybind11::cast(value_2);
      callback_(arg0, arg1);
    }

    void operator()(roq::fix::Header const &header, roq::codec::fix::Logon const &value) override {
      dispatch<Logon>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::Logout const &value) override {
      dispatch<Logout>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::TestRequest const &value) override {
      dispatch<TestRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::Heartbeat const &value) override {
      dispatch<Heartbeat>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::ResendRequest const &value) override {
      dispatch<ResendRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::Reject const &value) override {
      dispatch<Reject>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::BusinessMessageReject const &value) override {
      dispatch<BusinessMessageReject>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::UserRequest const &value) override {
      dispatch<UserRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::UserResponse const &value) override {
      dispatch<UserResponse>(header, value);
    }
    void operator()(
        roq::fix::Header const &header, roq::codec::fix::TradingSessionStatusRequest const &value) override {
      dispatch<TradingSessionStatusRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::TradingSessionStatus const &value) override {
      dispatch<TradingSessionStatus>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityListRequest const &value) override {
      dispatch<SecurityListRequest>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityList const &value) override {
      dispatch<SecurityList>(header, value);
    }
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityDefinitionRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityDefinition const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityStatusRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::SecurityStatus const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::MarketDataRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::MarketDataRequestReject const &value) override {}
    void operator()(
        roq::fix::Header const &header, roq::codec::fix::MarketDataSnapshotFullRefresh const &value) override {}
    void operator()(
        roq::fix::Header const &header, roq::codec::fix::MarketDataIncrementalRefresh const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderStatusRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderMassStatusRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::NewOrderSingle const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderCancelRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderCancelReplaceRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderMassCancelRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::OrderCancelReject const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::ExecutionReport const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::TradeCaptureReportRequest const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::TradeCaptureReport const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::RequestForPositions const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::RequestForPositionsAck const &value) override {}
    void operator()(roq::fix::Header const &header, roq::codec::fix::PositionReport const &value) override {}

   private:
    Callback const &callback_;
  };

  Decoder() : decoder_{roq::codec::fix::Decoder::create()} {}

  template <typename Callback>
  size_t dispatch(Callback const &callback, std::string_view const &message) {
    size_t result = {};
    try {
      Handler handler{callback};
      std::span buffer{reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)};
      result = (*decoder_)(handler, buffer);
    } catch (pybind11::error_already_set &) {
      /*
      log::warn("caught exception!"sv);
      return false;  // break
      */
      throw;
    } catch (...) {
      using namespace std::literals;
      fmt::print(stderr, "HERE\n"sv);
    }
    return result;
  }

  // XXX HANS tuple

 private:
  std::unique_ptr<roq::codec::fix::Decoder> decoder_;
};

}  // namespace fix
}  // namespace python
}  // namespace roq
