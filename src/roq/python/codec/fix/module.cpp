/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/codec/fix/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/utils.hpp"

#include "roq/python/codec/fix/decoder.hpp"
#include "roq/python/codec/fix/details.hpp"
#include "roq/python/codec/fix/header.hpp"

using namespace std::literals;

namespace roq {
namespace python {
namespace codec {
namespace fix {

void Module::create(pybind11::module_ &module) {
  // helpers
  utils::create_struct<roq::python::codec::fix::InstrmtMDReq>(module);
  utils::create_struct<roq::python::codec::fix::SecListGrp>(module);

  utils::create_struct<roq::python::codec::fix::MDFull>(module);
  utils::create_struct<roq::python::codec::fix::MDInc>(module);

  utils::create_struct<roq::python::codec::fix::Encodeable>(module);
  utils::create_struct<roq::python::codec::fix::Encoder>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::Logon, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::Logout, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::TestRequest, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::Heartbeat, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::ResendRequest, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::Reject, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::BusinessMessageReject, roq::python::codec::fix::Encodeable>(
      module);

  utils::create_ref_struct_2<roq::python::codec::fix::UserRequest, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::UserResponse, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::TradingSessionStatusRequest, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::TradingSessionStatus, roq::python::codec::fix::Encodeable>(
      module);

  utils::create_ref_struct_2<roq::python::codec::fix::SecurityListRequest, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::SecurityList, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::SecurityDefinitionRequest, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::SecurityDefinition, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::SecurityStatusRequest, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::SecurityStatus, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::MarketDataRequest, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::MarketDataRequestReject, roq::python::codec::fix::Encodeable>(
      module);
  utils::
      create_ref_struct_2<roq::python::codec::fix::MarketDataSnapshotFullRefresh, roq::python::codec::fix::Encodeable>(
          module);
  utils::
      create_ref_struct_2<roq::python::codec::fix::MarketDataIncrementalRefresh, roq::python::codec::fix::Encodeable>(
          module);

  utils::create_ref_struct_2<roq::python::codec::fix::OrderStatusRequest, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::OrderMassStatusRequest, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::NewOrderSingle, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::OrderCancelRequest, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::OrderCancelReplaceRequest, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::OrderMassCancelRequest, roq::python::codec::fix::Encodeable>(
      module);

  utils::create_ref_struct_2<roq::python::codec::fix::OrderCancelReject, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::OrderMassCancelReport, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::ExecutionReport, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::TradeCaptureReportRequest, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::TradeCaptureReport, roq::python::codec::fix::Encodeable>(module);

  utils::create_ref_struct_2<roq::python::codec::fix::RequestForPositions, roq::python::codec::fix::Encodeable>(module);
  utils::create_ref_struct_2<roq::python::codec::fix::RequestForPositionsAck, roq::python::codec::fix::Encodeable>(
      module);
  utils::create_ref_struct_2<roq::python::codec::fix::PositionReport, roq::python::codec::fix::Encodeable>(module);

  utils::create_struct<roq::python::codec::fix::Header>(module);
  utils::create_struct<roq::python::codec::fix::Decoder>(module);
}

}  // namespace fix
}  // namespace codec
}  // namespace python
}  // namespace roq
