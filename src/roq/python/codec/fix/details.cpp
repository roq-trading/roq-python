/* Copyright (c) 2017-2024, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/fix/details.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/codec/fix/decoder.hpp"

using namespace std::literals;

namespace roq {
namespace python {

// helpers (create)

template <>
void utils::create_struct<roq::python::codec::fix::SecListGrp>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::SecListGrp;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<std::string, double, std::string, double, std::string, double>(),
          pybind11::arg("symbol"),
          pybind11::arg("contract_multiplier") = NaN,
          pybind11::arg("security_exchange"),
          pybind11::arg("min_trade_vol") = NaN,
          pybind11::arg("trading_session_id") = std::string{},
          pybind11::arg("min_price_increment") = NaN)
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_struct<roq::python::codec::fix::InstrmtMDReq>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::InstrmtMDReq;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<std::string_view, std::string_view>(),
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

// helpers (update)

template <>
void utils::create_struct<roq::python::codec::fix::MDFull>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::MDFull;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly(
          "md_entry_type",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_type; })
      .def_property_readonly(
          "md_entry_px",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_px.value; })
      .def_property_readonly(
          "md_entry_size",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_size.value; })
      .def_property_readonly(
          "md_entry_date",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_date; })
      .def_property_readonly(
          "md_entry_time",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_time; })
      .def_property_readonly(
          "trading_session_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).trading_session_id; })
      .def_property_readonly(
          "expire_time", [](value_type const &self) { return static_cast<value_type::value_type>(self).expire_time; })
      .def_property_readonly(
          "order_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).order_id; })
      .def_property_readonly(
          "number_of_orders",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).number_of_orders; })
      .def_property_readonly(
          "md_entry_position_no",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_position_no; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_struct<roq::python::codec::fix::MDInc>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::MDInc;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def_property_readonly(
          "md_update_action",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_update_action; })
      .def_property_readonly(
          "md_entry_type",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_type; })
      .def_property_readonly(
          "symbol", [](value_type const &self) { return static_cast<value_type::value_type>(self).symbol; })
      .def_property_readonly(
          "security_exchange",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).security_exchange; })
      .def_property_readonly(
          "md_entry_px",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_px.value; })
      .def_property_readonly(
          "md_entry_size",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_size.value; })
      .def_property_readonly(
          "md_entry_date",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_date; })
      .def_property_readonly(
          "md_entry_time",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_time; })
      .def_property_readonly(
          "trading_session_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).trading_session_id; })
      .def_property_readonly(
          "expire_time", [](value_type const &self) { return static_cast<value_type::value_type>(self).expire_time; })
      .def_property_readonly(
          "order_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).order_id; })
      .def_property_readonly(
          "number_of_orders",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).number_of_orders; })
      .def_property_readonly(
          "md_entry_position_no",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_entry_position_no; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_struct<roq::python::codec::fix::Encoder>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Encoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
      .def(
          pybind11::init<std::string_view, std::string_view>(),
          pybind11::arg("sender_comp_id"),
          pybind11::arg("target_comp_id"))
      .def(
          "encode",
          [](value_type &self,
             roq::python::codec::fix::Encodeable &encodeable,
             std::chrono::system_clock::time_point sending_time) {
            auto message = self.encode(encodeable, sending_time);
            std::string_view result{reinterpret_cast<char const *>(std::data(message)), std::size(message)};
            return pybind11::bytes{result};
          },
          pybind11::arg("encodeable"),
          pybind11::arg("sending_time"));
}

template <>
void utils::create_struct<roq::python::codec::fix::Encodeable>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str());
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::Logon, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Logon;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              roq::fix::EncryptMethod,
              std::chrono::seconds,
              uint32_t,
              std::string_view,
              bool,
              uint64_t,
              std::string_view,
              std::string_view>(),
          pybind11::arg("encrypt_method") = roq::fix::EncryptMethod::NONE,
          pybind11::arg("heart_bt_int"),
          pybind11::arg("raw_data_length"),
          pybind11::arg("raw_data"),
          pybind11::arg("reset_seq_num_flag") = bool{},
          pybind11::arg("next_expected_msg_seq_num") = uint64_t{},
          pybind11::arg("username"),
          pybind11::arg("password") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::Logout, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Logout;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(pybind11::init<std::string_view>(), pybind11::arg("text") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::TestRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::TestRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(pybind11::init<std::string_view>(), pybind11::arg("test_req_id") = std::string_view{})
      .def_property_readonly(
          "test_req_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).test_req_id; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::Heartbeat, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Heartbeat;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(pybind11::init<std::string_view>(), pybind11::arg("test_req_id") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::ResendRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::ResendRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<uint64_t, uint64_t>(),
          pybind11::arg("begin_seq_no") = uint64_t{},
          pybind11::arg("end_seq_no") = uint64_t{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::Reject, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Reject;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<uint64_t, std::string_view, uint32_t, roq::fix::MsgType, roq::fix::SessionRejectReason>(),
          pybind11::arg("ref_seq_num") = uint64_t{},
          pybind11::arg("text") = std::string_view{},
          pybind11::arg("ref_tag_id") = uint32_t{},
          pybind11::arg("ref_msg_type") = roq::fix::MsgType{},
          pybind11::arg("session_reject_reason") = roq::fix::SessionRejectReason{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::BusinessMessageReject, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::BusinessMessageReject;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::
              init<uint64_t, roq::fix::MsgType, std::string_view, roq::fix::BusinessRejectReason, std::string_view>(),
          pybind11::arg("ref_seq_num") = uint64_t{},
          pybind11::arg("ref_msg_type") = roq::fix::MsgType{},
          pybind11::arg("business_reject_ref_id") = std::string_view{},
          pybind11::arg("business_reject_reason") = roq::fix::BusinessRejectReason{},
          pybind11::arg("text") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::UserRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::UserRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::
              init<std::string_view, roq::fix::UserRequestType, std::string_view, std::string_view, std::string_view>(),
          pybind11::arg("user_request_id") = std::string_view{},
          pybind11::arg("user_request_type") = roq::fix::UserRequestType{},
          pybind11::arg("username") = std::string_view{},
          pybind11::arg("password") = std::string_view{},
          pybind11::arg("new_password") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::UserResponse, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::UserResponse;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<std::string_view, std::string_view, roq::fix::UserStatus, std::string_view>(),
          pybind11::arg("user_request_id") = std::string_view{},
          pybind11::arg("username") = std::string_view{},
          pybind11::arg("user_status") = roq::fix::UserStatus{},
          pybind11::arg("user_status_text") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<
    roq::python::codec::fix::TradingSessionStatusRequest,
    roq::python::codec::fix::Encodeable>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::TradingSessionStatusRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<std::string_view, std::string_view, roq::fix::SubscriptionRequestType>(),
          pybind11::arg("trad_ses_req_id") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("subscription_request_type") = roq::fix::SubscriptionRequestType{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::TradingSessionStatus, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::TradingSessionStatus;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
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
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::SecurityListRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::SecurityListRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
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
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::SecurityList, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::SecurityList;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              roq::fix::SecurityRequestResult,
              std::vector<roq::python::codec::fix::SecListGrp>>(),
          pybind11::arg("security_req_id") = std::string_view{},
          pybind11::arg("security_response_id") = std::string_view{},
          pybind11::arg("security_request_result") = roq::fix::SecurityRequestResult{},
          pybind11::arg("no_related_sym") = std::vector<roq::python::codec::fix::SecListGrp>{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<
    roq::python::codec::fix::SecurityDefinitionRequest,
    roq::python::codec::fix::Encodeable>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::SecurityDefinitionRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              roq::fix::SecurityRequestType,
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::SubscriptionRequestType>(),
          pybind11::arg("security_req_id") = std::string_view{},
          pybind11::arg("security_request_type") = roq::fix::SecurityRequestType{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("subscription_request_type") = roq::fix::SubscriptionRequestType{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::SecurityDefinition, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::SecurityDefinition;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              roq::fix::SecurityResponseType,
              std::string_view,
              double,
              std::string_view,
              std::string_view,
              double>(),
          pybind11::arg("security_req_id") = std::string_view{},
          pybind11::arg("security_response_id") = std::string_view{},
          pybind11::arg("security_response_type") = roq::fix::SecurityResponseType{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("contract_multiplier") = std::numeric_limits<double>::quiet_NaN(),
          pybind11::arg("security_exechange") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("min_trade_vol") = std::numeric_limits<double>::quiet_NaN())
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::SecurityStatusRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::SecurityStatusRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::SubscriptionRequestType,
              std::string_view>(),
          pybind11::arg("security_status_req_id") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("subscription_request_type") = roq::fix::SubscriptionRequestType{},
          pybind11::arg("trading_session_id") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::SecurityStatus, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::SecurityStatus;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              bool,
              roq::fix::SecurityTradingStatus,
              std::chrono::system_clock::time_point>(),
          pybind11::arg("security_status_req_id") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("unsolicited_indicator,") = bool{},
          pybind11::arg("security_trading_status") = roq::fix::SecurityTradingStatus{},
          pybind11::arg("transact_time") = std::chrono::system_clock::time_point{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::MarketDataRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::MarketDataRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              roq::fix::SubscriptionRequestType,
              uint32_t,
              roq::fix::MDUpdateType,
              bool,
              std::vector<roq::fix::MDEntryType>,
              std::vector<roq::python::codec::fix::InstrmtMDReq>>(),
          pybind11::arg("md_req_id"),                  // required
          pybind11::arg("subscription_request_type"),  // required
          pybind11::arg("market_depth") = uint32_t{},  // required
          pybind11::arg("md_update_type") = roq::fix::MDUpdateType{},
          pybind11::arg("aggregated_book") = false,
          pybind11::arg("no_md_entry_types"),  // required
          pybind11::arg("no_related_sym"))     // required
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::MarketDataRequestReject, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::MarketDataRequestReject;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "md_req_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).md_req_id; })
      .def_property_readonly(
          "md_req_rej_reason",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).md_req_rej_reason; })
      .def_property_readonly(
          "text", [](value_type const &self) { return static_cast<value_type::value_type>(self).text; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<
    roq::python::codec::fix::MarketDataSnapshotFullRefresh,
    roq::python::codec::fix::Encodeable>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::MarketDataSnapshotFullRefresh;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "md_req_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).md_req_id; })
      .def_property_readonly(
          "symbol", [](value_type const &self) { return static_cast<value_type::value_type>(self).symbol; })
      .def_property_readonly(
          "security_exchange",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).security_exchange; })
      .def_property_readonly(
          "no_md_entries",
          [](value_type const &self) {
            return self.no_md_entries_;  // XXX FIXME
          })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<
    roq::python::codec::fix::MarketDataIncrementalRefresh,
    roq::python::codec::fix::Encodeable>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::MarketDataIncrementalRefresh;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "md_req_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).md_req_id; })
      .def_property_readonly(
          "no_md_entries",
          [](value_type const &self) {
            return self.no_md_entries_;  // XXX FIXME
          })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::OrderStatusRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::OrderStatusRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::Side>(),
          pybind11::arg("order_id") = std::string_view{},
          pybind11::arg("cl_ord_id") = std::string_view{},
          pybind11::arg("ord_status_req_id") = std::string_view{},
          pybind11::arg("account") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("side") = roq::fix::Side{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::OrderMassStatusRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::OrderMassStatusRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              roq::fix::MassStatusReqType,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::Side>(),
          pybind11::arg("mass_status_req_id") = std::string_view{},
          pybind11::arg("mass_status_req_type") = roq::fix::MassStatusReqType{},
          pybind11::arg("account") = std::string_view{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("side") = roq::fix::Side{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::NewOrderSingle, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::NewOrderSingle;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::HandlInst,
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::Side,
              std::chrono::system_clock::time_point,
              double,
              roq::fix::OrdType,
              double,
              double,
              roq::fix::TimeInForce,
              std::string_view,
              roq::fix::PositionEffect,
              double>(),
          pybind11::arg("cl_ord_id") = std::string_view{},
          pybind11::arg("secondary_cl_ord_id") = std::string_view{},
          pybind11::arg("account") = std::string_view{},
          pybind11::arg("handl_inst") = roq::fix::HandlInst{},
          pybind11::arg("exec_inst") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("side") = roq::fix::Side{},
          pybind11::arg("transact_time") = std::chrono::system_clock::time_point{},
          pybind11::arg("order_qty") = std::numeric_limits<double>::quiet_NaN(),
          pybind11::arg("ord_type") = roq::fix::OrdType{},
          pybind11::arg("price") = std::numeric_limits<double>::quiet_NaN(),
          pybind11::arg("stop_px") = std::numeric_limits<double>::quiet_NaN(),
          pybind11::arg("time_in_force") = roq::fix::TimeInForce{},
          pybind11::arg("text") = std::string_view{},
          pybind11::arg("position_effect") = roq::fix::PositionEffect{},
          pybind11::arg("max_show") = std::numeric_limits<double>::quiet_NaN())
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::OrderCancelRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::OrderCancelRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::Side,
              std::chrono::system_clock::time_point,
              double,
              std::string_view>(),
          pybind11::arg("orig_cl_ord_id") = std::string_view{},
          pybind11::arg("order_id") = std::string_view{},
          pybind11::arg("cl_ord_id") = std::string_view{},
          pybind11::arg("secondary_cl_ord_id") = std::string_view{},
          pybind11::arg("account") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("side") = roq::fix::Side{},
          pybind11::arg("transact_time") = std::chrono::system_clock::time_point{},
          pybind11::arg("order_qty") = std::numeric_limits<double>::quiet_NaN(),
          pybind11::arg("text") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<
    roq::python::codec::fix::OrderCancelReplaceRequest,
    roq::python::codec::fix::Encodeable>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::OrderCancelReplaceRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view,
              double,
              double,
              roq::fix::Side,
              std::chrono::system_clock::time_point,
              roq::fix::OrdType>(),
          pybind11::arg("order_id") = std::string_view{},
          pybind11::arg("orig_cl_ord_id") = std::string_view{},
          pybind11::arg("cl_ord_id") = std::string_view{},
          pybind11::arg("secondary_cl_ord_id") = std::string_view{},
          pybind11::arg("account") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("order_qty") = std::numeric_limits<double>::quiet_NaN(),
          pybind11::arg("price") = std::numeric_limits<double>::quiet_NaN(),
          pybind11::arg("side") = roq::fix::Side{},
          pybind11::arg("transact_time") = std::chrono::system_clock::time_point{},
          pybind11::arg("ord_type") = roq::fix::OrdType{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::OrderMassCancelRequest, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::OrderMassCancelRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              roq::fix::MassCancelRequestType,
              std::string_view,
              std::string_view,
              std::string_view,
              roq::fix::Side,
              std::chrono::system_clock::time_point>(),
          pybind11::arg("cl_ord_id") = std::string_view{},
          pybind11::arg("mass_cancel_request_type") = roq::fix::MassCancelRequestType{},
          pybind11::arg("trading_session_id") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("side") = roq::fix::Side{},
          pybind11::arg("transact_time") = std::chrono::system_clock::time_point{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::OrderCancelReject, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::OrderCancelReject;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "order_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).order_id; })
      .def_property_readonly(
          "secondary_cl_ord_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).secondary_cl_ord_id; })
      .def_property_readonly(
          "cl_ord_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).cl_ord_id; })
      .def_property_readonly(
          "orig_cl_ord_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).orig_cl_ord_id; })
      .def_property_readonly(
          "ord_status", [](value_type const &self) { return static_cast<value_type::value_type>(self).ord_status; })
      .def_property_readonly(
          "working_indicator",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).working_indicator; })
      .def_property_readonly(
          "account", [](value_type const &self) { return static_cast<value_type::value_type>(self).account; })
      .def_property_readonly(
          "cxl_rej_response_to",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).cxl_rej_response_to; })
      .def_property_readonly(
          "cxl_rej_reason",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).cxl_rej_reason; })
      .def_property_readonly(
          "text", [](value_type const &self) { return static_cast<value_type::value_type>(self).text; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::OrderMassCancelReport, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::OrderMassCancelReport;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "cl_ord_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).cl_ord_id; })
      .def_property_readonly(
          "order_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).order_id; })
      .def_property_readonly(
          "mass_cancel_request_type",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).mass_cancel_request_type; })
      .def_property_readonly(
          "mass_cancel_response",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).mass_cancel_response; })
      .def_property_readonly(
          "mass_cancel_reject_reason",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).mass_cancel_reject_reason; })
      .def_property_readonly(
          "total_affected_orders",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).total_affected_orders; })
      .def_property_readonly(
          "symbol", [](value_type const &self) { return static_cast<value_type::value_type>(self).symbol; })
      .def_property_readonly(
          "security_exchange",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).security_exchange; })
      .def_property_readonly(
          "side", [](value_type const &self) { return static_cast<value_type::value_type>(self).side; })
      .def_property_readonly(
          "text", [](value_type const &self) { return static_cast<value_type::value_type>(self).text; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::ExecutionReport, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::ExecutionReport;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "order_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).order_id; })
      .def_property_readonly(
          "secondary_cl_ord_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).secondary_cl_ord_id; })
      .def_property_readonly(
          "cl_ord_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).cl_ord_id; })
      .def_property_readonly(
          "orig_cl_ord_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).orig_cl_ord_id; })
      .def_property_readonly(
          "ord_status_req_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).ord_status_req_id; })
      .def_property_readonly(
          "mass_status_req_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).mass_status_req_id; })
      .def_property_readonly(
          "tot_num_reports",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).tot_num_reports; })
      .def_property_readonly(
          "last_rpt_requested",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).last_rpt_requested; })
      .def_property_readonly(
          "exec_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).exec_id; })
      .def_property_readonly(
          "exec_type", [](value_type const &self) { return static_cast<value_type::value_type>(self).exec_type; })
      .def_property_readonly(
          "ord_status", [](value_type const &self) { return static_cast<value_type::value_type>(self).ord_status; })
      .def_property_readonly(
          "working_indicator",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).working_indicator; })
      .def_property_readonly(
          "ord_rej_reason",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).ord_rej_reason; })
      .def_property_readonly(
          "account", [](value_type const &self) { return static_cast<value_type::value_type>(self).account; })
      .def_property_readonly(
          "account_type", [](value_type const &self) { return static_cast<value_type::value_type>(self).account_type; })
      .def_property_readonly(
          "symbol", [](value_type const &self) { return static_cast<value_type::value_type>(self).symbol; })
      .def_property_readonly(
          "security_exchange",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).security_exchange; })
      .def_property_readonly(
          "side", [](value_type const &self) { return static_cast<value_type::value_type>(self).side; })
      .def_property_readonly(
          "ord_type", [](value_type const &self) { return static_cast<value_type::value_type>(self).ord_type; })
      .def_property_readonly(
          "order_qty", [](value_type const &self) { return static_cast<value_type::value_type>(self).order_qty.value; })
      .def_property_readonly(
          "price", [](value_type const &self) { return static_cast<value_type::value_type>(self).price.value; })
      .def_property_readonly(
          "stop_px", [](value_type const &self) { return static_cast<value_type::value_type>(self).stop_px.value; })
      .def_property_readonly(
          "currency", [](value_type const &self) { return static_cast<value_type::value_type>(self).currency; })
      .def_property_readonly(
          "time_in_force",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).time_in_force; })
      .def_property_readonly(
          "exec_inst", [](value_type const &self) { return static_cast<value_type::value_type>(self).exec_inst; })
      .def_property_readonly(
          "last_qty", [](value_type const &self) { return static_cast<value_type::value_type>(self).last_qty.value; })
      .def_property_readonly(
          "last_px", [](value_type const &self) { return static_cast<value_type::value_type>(self).last_px.value; })
      .def_property_readonly(
          "trading_session_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).trading_session_id; })
      .def_property_readonly(
          "leaves_qty",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).leaves_qty.value; })
      .def_property_readonly(
          "cum_qty", [](value_type const &self) { return static_cast<value_type::value_type>(self).cum_qty.value; })
      .def_property_readonly(
          "avg_px", [](value_type const &self) { return static_cast<value_type::value_type>(self).avg_px.value; })
      .def_property_readonly(
          "transact_time",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).transact_time; })
      .def_property_readonly(
          "position_effect",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).position_effect; })
      .def_property_readonly(
          "max_show", [](value_type const &self) { return static_cast<value_type::value_type>(self).max_show; })
      .def_property_readonly(
          "text", [](value_type const &self) { return static_cast<value_type::value_type>(self).text; })
      .def_property_readonly(
          "last_liquidity_ind",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).last_liquidity_ind; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<
    roq::python::codec::fix::TradeCaptureReportRequest,
    roq::python::codec::fix::Encodeable>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::TradeCaptureReportRequest;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              roq::fix::TradeRequestType,
              roq::fix::SubscriptionRequestType,
              std::string_view,
              std::string_view,
              std::string_view,
              std::string_view>(),
          pybind11::arg("trade_request_id") = std::string_view{},
          pybind11::arg("trade_request_type,") = roq::fix::TradeRequestType{},
          pybind11::arg("subscription_request_type,") = roq::fix::SubscriptionRequestType{},
          pybind11::arg("order_id") = std::string_view{},
          pybind11::arg("cl_ord_id") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::TradeCaptureReport, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::TradeCaptureReport;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "trade_report_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).trade_report_id; })
      .def_property_readonly(
          "trade_request_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).trade_request_id; })
      .def_property_readonly(
          "exec_type", [](value_type const &self) { return static_cast<value_type::value_type>(self).exec_type; })
      .def_property_readonly(
          "tot_num_trade_reports",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).tot_num_trade_reports; })
      .def_property_readonly(
          "last_rpt_requested",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).last_rpt_requested; })
      .def_property_readonly(
          "unsolicited_indicator",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).unsolicited_indicator; })
      .def_property_readonly(
          "trd_match_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).trd_match_id; })
      .def_property_readonly(
          "exec_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).exec_id; })
      .def_property_readonly(
          "previously_reported",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).previously_reported; })
      .def_property_readonly(
          "symbol", [](value_type const &self) { return static_cast<value_type::value_type>(self).symbol; })
      .def_property_readonly(
          "security_exchange",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).security_exchange; })
      .def_property_readonly(
          "last_qty", [](value_type const &self) { return static_cast<value_type::value_type>(self).last_qty.value; })
      .def_property_readonly(
          "last_px", [](value_type const &self) { return static_cast<value_type::value_type>(self).last_px.value; })
      .def_property_readonly(
          "trade_date", [](value_type const &self) { return static_cast<value_type::value_type>(self).trade_date; })
      .def_property_readonly(
          "transact_time",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).transact_time; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::RequestForPositions, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::RequestForPositions;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def(
          pybind11::init<
              std::string_view,
              roq::fix::PosReqType,
              roq::fix::SubscriptionRequestType,
              std::string_view,
              roq::fix::AccountType,
              std::string_view,
              std::string_view,
              std::string_view,
              std::chrono::system_clock::time_point,
              std::chrono::system_clock::time_point>(),
          pybind11::arg("pos_req_id") = std::string_view{},
          pybind11::arg("pos_req_type,") = roq::fix::PosReqType{},
          pybind11::arg("subscription_request_type,") = roq::fix::SubscriptionRequestType{},
          pybind11::arg("account") = std::string_view{},
          pybind11::arg("account_type,") = roq::fix::AccountType{},
          pybind11::arg("currency") = std::string_view{},
          pybind11::arg("symbol") = std::string_view{},
          pybind11::arg("security_exchange") = std::string_view{},
          pybind11::arg("clearing_business_date") = std::chrono::system_clock::time_point{},
          pybind11::arg("transact_time") = std::chrono::system_clock::time_point{})
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::RequestForPositionsAck, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::RequestForPositionsAck;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "pos_maint_rpt_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_maint_rpt_id; })
      .def_property_readonly(
          "pos_req_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_req_id; })
      .def_property_readonly(
          "total_num_pos_reports",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).total_num_pos_reports; })
      .def_property_readonly(
          "unsolicited_indicator",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).unsolicited_indicator; })
      .def_property_readonly(
          "pos_req_result",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_req_result; })
      .def_property_readonly(
          "pos_req_status",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_req_status; })
      .def_property_readonly(
          "account", [](value_type const &self) { return static_cast<value_type::value_type>(self).account; })
      .def_property_readonly(
          "account_type", [](value_type const &self) { return static_cast<value_type::value_type>(self).account_type; })
      .def_property_readonly(
          "text", [](value_type const &self) { return static_cast<value_type::value_type>(self).text; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_ref_struct_2<roq::python::codec::fix::PositionReport, roq::python::codec::fix::Encodeable>(
    pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::PositionReport;
  using base_type = roq::python::codec::fix::Encodeable;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type, base_type>(module, name.c_str())
      .def_property_readonly(
          "pos_maint_rpt_id",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_maint_rpt_id; })
      .def_property_readonly(
          "pos_req_id", [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_req_id; })
      .def_property_readonly(
          "pos_req_type", [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_req_type; })
      .def_property_readonly(
          "subscription_request_type",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).subscription_request_type; })
      .def_property_readonly(
          "total_num_pos_reports",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).total_num_pos_reports; })
      .def_property_readonly(
          "unsolicited_indicator",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).unsolicited_indicator; })
      .def_property_readonly(
          "pos_req_result",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).pos_req_result; })
      .def_property_readonly(
          "clearing_business_date",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).clearing_business_date; })
      .def_property_readonly(
          "account", [](value_type const &self) { return static_cast<value_type::value_type>(self).account; })
      .def_property_readonly(
          "account_type", [](value_type const &self) { return static_cast<value_type::value_type>(self).account_type; })
      .def_property_readonly(
          "symbol", [](value_type const &self) { return static_cast<value_type::value_type>(self).symbol; })
      .def_property_readonly(
          "security_exchange",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).security_exchange; })
      .def_property_readonly(
          "currency", [](value_type const &self) { return static_cast<value_type::value_type>(self).currency; })
      .def_property_readonly(
          "settl_price",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).settl_price.value; })
      .def_property_readonly(
          "settl_price_type",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).settl_price_type; })
      .def_property_readonly(
          "prior_settl_price",
          [](value_type const &self) { return static_cast<value_type::value_type>(self).prior_settl_price.value; })
      .def_property_readonly(
          "text", [](value_type const &self) { return static_cast<value_type::value_type>(self).text; })
      .def("__repr__", [](value_type const &self) {
        return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
      });
}

template <>
void utils::create_struct<roq::python::codec::fix::Header>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Header;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str()).def("__repr__", [](value_type const &self) {
    return fmt::format("{}"sv, static_cast<value_type::value_type>(self));
  });
}

template <>
void utils::create_struct<roq::python::codec::fix::Decoder>(pybind11::module_ &module) {
  using value_type = roq::python::codec::fix::Decoder;
  std::string name{nameof::nameof_short_type<value_type>()};
  pybind11::class_<value_type>(module, name.c_str())
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
