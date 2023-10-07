/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/fix/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/fix/decoder.hpp"

using namespace std::literals;

namespace roq {
namespace python {

template <>
void utils::create_struct<roq::python::fix::SecListGrp>(pybind11::module_ &context) {
  using value_type = roq::python::fix::SecListGrp;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(context, name.c_str())
      .def(
          pybind11::init<std::string, double, std::string, double, std::string>(),
          pybind11::arg("symbol"),
          pybind11::arg("contract_multiplier") = NaN,
          pybind11::arg("security_exchange"),
          pybind11::arg("min_trade_vol") = NaN,
          pybind11::arg("trading_session_id") = std::string{})
      .def("__repr__", [](value_type const &value) {
        return fmt::print("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_struct<roq::python::fix::Encoder>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Encoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(context, name.c_str())
      .def(
          pybind11::init<std::string_view, std::string_view>(),
          pybind11::arg("sender_comp_id"),
          pybind11::arg("target_comp_id"))
      .def(
          "encode",
          [](value_type &self,
             roq::python::fix::Encodeable &encodeable,
             std::chrono::system_clock::time_point sending_time) {
            auto message = self.encode(encodeable, sending_time);
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("encodeable"),
          pybind11::arg("sending_time"));
}

template <>
void utils::create_struct<roq::python::fix::Encodeable>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(context, name.c_str());
}

template <>
void utils::create_ref_struct_2<roq::python::fix::Logon, roq::python::fix::Encodeable>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Logon;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::
              init<roq::fix::EncryptMethod, std::chrono::seconds, bool, uint64_t, std::string_view, std::string_view>(),
          pybind11::arg("encrypt_method") = roq::fix::EncryptMethod::NONE,
          pybind11::arg("heart_bt_int"),
          pybind11::arg("reset_seq_num_flag") = bool{},
          pybind11::arg("next_expected_msg_seq_num") = uint64_t{},
          pybind11::arg("username"),
          pybind11::arg("password") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::Logout, roq::python::fix::Encodeable>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Logout;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(pybind11::init<std::string_view>(), pybind11::arg("text") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::TestRequest, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::TestRequest;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(pybind11::init<std::string_view>(), pybind11::arg("test_req_id") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::Heartbeat, roq::python::fix::Encodeable>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Heartbeat;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(pybind11::init<std::string_view>(), pybind11::arg("test_req_id") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::ResendRequest, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::ResendRequest;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::init<uint64_t, uint64_t>(),
          pybind11::arg("begin_seq_no") = uint64_t{},
          pybind11::arg("end_seq_no") = uint64_t{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::Reject, roq::python::fix::Encodeable>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Reject;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::init<uint64_t, std::string_view, uint32_t, roq::fix::MsgType, roq::fix::SessionRejectReason>(),
          pybind11::arg("ref_seq_num") = uint64_t{},
          pybind11::arg("text") = std::string_view{},
          pybind11::arg("ref_tag_id") = uint32_t{},
          pybind11::arg("ref_msg_type") = roq::fix::MsgType{},
          pybind11::arg("session_reject_reason") = roq::fix::SessionRejectReason{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::BusinessMessageReject, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::BusinessMessageReject;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::
              init<uint64_t, roq::fix::MsgType, std::string_view, roq::fix::BusinessRejectReason, std::string_view>(),
          pybind11::arg("ref_seq_num") = uint64_t{},
          pybind11::arg("ref_msg_type") = roq::fix::MsgType{},
          pybind11::arg("business_reject_ref_id") = std::string_view{},
          pybind11::arg("business_reject_reason") = roq::fix::BusinessRejectReason{},
          pybind11::arg("text") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::UserRequest, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::UserRequest;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::
              init<std::string_view, roq::fix::UserRequestType, std::string_view, std::string_view, std::string_view>(),
          pybind11::arg("user_request_id") = std::string_view{},
          pybind11::arg("user_request_type") = roq::fix::UserRequestType{},
          pybind11::arg("username") = std::string_view{},
          pybind11::arg("password") = std::string_view{},
          pybind11::arg("new_password") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::UserResponse, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::UserResponse;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::init<std::string_view, std::string_view, roq::fix::UserStatus, std::string_view>(),
          pybind11::arg("user_request_id") = std::string_view{},
          pybind11::arg("username") = std::string_view{},
          pybind11::arg("user_status") = roq::fix::UserStatus{},
          pybind11::arg("user_status_text") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::TradingSessionStatusRequest, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::TradingSessionStatusRequest;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::init<std::string_view, std::string_view, roq::fix::SubscriptionRequestType>(),
          pybind11::arg("trad_ses_req_id") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("subscription_request_type") = roq::fix::SubscriptionRequestType{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::TradingSessionStatus, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::TradingSessionStatus;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              bool,
              roq::fix::TradSesStatus,
              roq::fix::TradSesStatusRejReason,
              std::string_view>(),
          pybind11::arg("trad_ses_req_id") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("unsolicited_indicator") = bool{},
          pybind11::arg("trad_ses_status") = roq::fix::TradSesStatus{},
          pybind11::arg("trad_ses_status_rej_reason") = roq::fix::TradSesStatusRejReason{},
          pybind11::arg("text") = std::string_view{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::SecurityListRequest, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::SecurityListRequest;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              roq::fix::SecurityListRequestType,
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::SubscriptionRequestType>(),
          pybind11::arg("security_req_id") = std::string_view{},
          pybind11::arg("security_list_request_type") = roq::fix::SecurityListRequestType{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("subscription_request_type") = roq::fix::SubscriptionRequestType{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::fix::SecurityList, roq::python::fix::Encodeable>(
    pybind11::module_ &context) {
  using value_type = roq::python::fix::SecurityList;
  using base_type = roq::python::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(context, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              roq::fix::SecurityRequestResult,
              std::vector<roq::python::fix::SecListGrp>>(),
          pybind11::arg("security_req_id") = std::string_view{},
          pybind11::arg("security_response_id") = std::string_view{},
          pybind11::arg("security_request_result") = roq::fix::SecurityRequestResult{},
          pybind11::arg("no_related_sym") = std::vector<roq::python::fix::SecListGrp>{})
      .def("__repr__", [](value_type const &value) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
      });
}

template <>
void utils::create_struct<roq::python::fix::Header>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Header;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(context, name.c_str()).def("__repr__", [](value_type const &value) {
    return fmt::format("{}"sv, static_cast<value_type::value_type>(value));
  });
}

template <>
void utils::create_struct<roq::python::fix::Decoder>(pybind11::module_ &context) {
  using value_type = roq::python::fix::Decoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(context, name.c_str())
      .def(pybind11::init<>())
      // note! the callback signature **MUST** be pybind11::object so we can verify the reference count hasn't increased
      .def(
          "dispatch",
          [](value_type &self,
             std::function<void(pybind11::object, pybind11::object)> &callback,
             pybind11::bytes message) { return self.dispatch(callback, message); },
          pybind11::arg("callback"),
          pybind11::arg("message"));
}

}  // namespace python
}  // namespace roq
