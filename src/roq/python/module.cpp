/* Copyright (c) 2017-2023, Hans Erik Thrane */

#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "roq/python/module.hpp"

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "roq/python/details.hpp"
#include "roq/python/utils.hpp"

#include "roq/python/client/module.hpp"
#include "roq/python/codec/module.hpp"
#include "roq/python/io/module.hpp"
#include "roq/python/utils/module.hpp"

using namespace std::literals;

namespace roq {
namespace python {

void Module::create(pybind11::module_ &module) {
  // enums

  roq::python::utils::create_enum<roq::ConnectionStatus>(module);
  roq::python::utils::create_enum<roq::Decimals>(module);
  roq::python::utils::create_enum<roq::Encoding>(module);
  roq::python::utils::create_enum<roq::Error>(module);
  roq::python::utils::create_enum<roq::ExecutionInstruction>(module);
  roq::python::utils::create_enum<roq::Liquidity>(module);
  roq::python::utils::create_enum<roq::OptionType>(module);
  roq::python::utils::create_enum<roq::OrderCancelPolicy>(module);
  roq::python::utils::create_enum<roq::OrderManagement>(module);
  roq::python::utils::create_enum<roq::OrderStatus>(module);
  roq::python::utils::create_enum<roq::OrderType>(module);
  roq::python::utils::create_enum<roq::Origin>(module);
  roq::python::utils::create_enum<roq::PositionEffect>(module);
  roq::python::utils::create_enum<roq::Priority>(module);
  roq::python::utils::create_enum<roq::Protocol>(module);
  roq::python::utils::create_enum<roq::RateLimitType>(module);
  roq::python::utils::create_enum<roq::RequestIdType>(module);
  roq::python::utils::create_enum<roq::RequestStatus>(module);
  roq::python::utils::create_enum<roq::RequestType>(module);
  roq::python::utils::create_enum<roq::SecurityType>(module);
  roq::python::utils::create_enum<roq::Side>(module);
  roq::python::utils::create_enum<roq::StatisticsType>(module);
  roq::python::utils::create_enum<roq::SupportType>(module);
  roq::python::utils::create_enum<roq::TimeInForce>(module);
  roq::python::utils::create_enum<roq::TradingStatus>(module);
  roq::python::utils::create_enum<roq::Transport>(module);
  roq::python::utils::create_enum<roq::UpdateAction>(module);
  roq::python::utils::create_enum<roq::UpdateType>(module);

  // helpers

  roq::python::utils::create_struct<roq::Fill>(module);
  roq::python::utils::create_struct<roq::Layer>(module);
  roq::python::utils::create_struct<roq::MBPUpdate>(module);
  roq::python::utils::create_struct<roq::MBOUpdate>(module);
  roq::python::utils::create_struct<roq::Measurement>(module);
  roq::python::utils::create_struct<roq::Statistics>(module);
  roq::python::utils::create_struct<roq::Trade>(module);
  roq::python::utils::create_struct<roq::UUID>(module);

  // transport

  roq::python::utils::create_ref_struct<roq::MessageInfo>(module);

  // struct

  roq::python::utils::create_ref_struct<roq::Start>(module);
  roq::python::utils::create_ref_struct<roq::Stop>(module);
  roq::python::utils::create_ref_struct<roq::Timer>(module);
  roq::python::utils::create_ref_struct<roq::Connected>(module);
  roq::python::utils::create_ref_struct<roq::Disconnected>(module);

  roq::python::utils::create_ref_struct<roq::DownloadBegin>(module);
  roq::python::utils::create_ref_struct<roq::DownloadEnd>(module);

  roq::python::utils::create_ref_struct<roq::GatewaySettings>(module);

  roq::python::utils::create_ref_struct<roq::StreamStatus>(module);
  roq::python::utils::create_ref_struct<roq::ExternalLatency>(module);
  roq::python::utils::create_ref_struct<roq::RateLimitTrigger>(module);

  roq::python::utils::create_ref_struct<roq::GatewayStatus>(module);

  roq::python::utils::create_ref_struct<roq::ReferenceData>(module);
  roq::python::utils::create_ref_struct<roq::MarketStatus>(module);
  roq::python::utils::create_ref_struct<roq::TopOfBook>(module);
  roq::python::utils::create_ref_struct<roq::MarketByPriceUpdate>(module);
  roq::python::utils::create_ref_struct<roq::MarketByOrderUpdate>(module);
  roq::python::utils::create_ref_struct<roq::TradeSummary>(module);
  roq::python::utils::create_ref_struct<roq::StatisticsUpdate>(module);

  roq::python::utils::create_ref_struct<roq::CreateOrder>(module);
  roq::python::utils::create_ref_struct<roq::ModifyOrder>(module);
  roq::python::utils::create_ref_struct<roq::CancelOrder>(module);
  roq::python::utils::create_ref_struct<roq::CancelAllOrders>(module);

  roq::python::utils::create_ref_struct<roq::CancelAllOrdersAck>(module);
  roq::python::utils::create_ref_struct<roq::OrderAck>(module);
  roq::python::utils::create_ref_struct<roq::OrderUpdate>(module);
  roq::python::utils::create_ref_struct<roq::TradeUpdate>(module);

  roq::python::utils::create_ref_struct<roq::PositionUpdate>(module);
  roq::python::utils::create_ref_struct<roq::FundsUpdate>(module);

  roq::python::utils::create_ref_struct<roq::CustomMetricsUpdate>(module);

  // sub-modules

  auto codec = module.def_submodule("codec");
  roq::python::codec::Module::create(codec);

  auto utils = module.def_submodule("utils");
  roq::python::utils::Module::create(utils);

  auto client = module.def_submodule("client");
  roq::python::client::Module::create(client);
}

}  // namespace python
}  // namespace roq
