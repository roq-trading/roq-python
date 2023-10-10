#!/usr/bin/env python

"""
Copyright (c) 2017-2023, Hans Erik Thrane

Demonstrates FIX message encoding/decoding
"""

from datetime import datetime, timedelta

from fastcore.all import typedispatch

import roq

encoder = roq.codec.fix.Encoder(sender_comp_id="me", target_comp_id="you")

logon = roq.codec.fix.Logon(
    heart_bt_int=timedelta(seconds=30),
    username="my_name",
    password="xyz",
    encrypt_method=roq.codec.fix.EncryptMethod.NONE,
)

message = encoder.encode(logon, datetime.now())

print('message="{}", length={}'.format(message.decode().replace(chr(1), "|"), len(message)))


@typedispatch
def callback(header: roq.codec.fix.Header, logon: roq.codec.fix.Logon):
    print("logon={}, header={}".format(logon, header))


@typedispatch
def callback(header: roq.codec.fix.Header, logout: roq.codec.fix.Logout):
    print("logout={}, header={}".format(logout, header))


@typedispatch
def callback(header: roq.codec.fix.Header, test_request: roq.codec.fix.TestRequest):
    print("test_request={}, header={}".format(test_request, header))


@typedispatch
def callback(header: roq.codec.fix.Header, heartbeat: roq.codec.fix.Heartbeat):
    print("heartbeat={}, header={}".format(heartbeat, header))


@typedispatch
def callback(header: roq.codec.fix.Header, resend_request: roq.codec.fix.ResendRequest):
    print("resend_request={}, header={}".format(resend_request, header))


@typedispatch
def callback(header: roq.codec.fix.Header, reject: roq.codec.fix.Reject):
    print("reject={}, header={}".format(reject, header))


@typedispatch
def callback(
    header: roq.codec.fix.Header, business_message_reject: roq.codec.fix.BusinessMessageReject
):
    print("business_message_reject={}, header={}".format(business_message_reject, header))


@typedispatch
def callback(header: roq.codec.fix.Header, user_request: roq.codec.fix.UserRequest):
    print("user_request={}, header={}".format(user_request, header))


@typedispatch
def callback(header: roq.codec.fix.Header, user_response: roq.codec.fix.UserResponse):
    print("user_response={}, header={}".format(user_response, header))


@typedispatch
def callback(
    header: roq.codec.fix.Header,
    trading_session_status_request: roq.codec.fix.TradingSessionStatusRequest,
):
    print(
        "trading_session_status_request={}, header={}".format(
            trading_session_status_request, header
        )
    )


@typedispatch
def callback(
    header: roq.codec.fix.Header, trading_session_status: roq.codec.fix.TradingSessionStatus
):
    print("trading_session_status={}, header={}".format(trading_session_status, header))


@typedispatch
def callback(
    header: roq.codec.fix.Header, security_list_request: roq.codec.fix.SecurityListRequest
):
    print("security_list_request={}, header={}".format(security_list_request, header))


@typedispatch
def callback(header: roq.codec.fix.Header, security_list: roq.codec.fix.SecurityList):
    print("security_list={}, header={}".format(security_list, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header,
#     security_definition_request: roq.codec.fix.SecurityDefinitionRequest,
# ):
#     print("security_definition_request={}, header={}".format(security_definition_request, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, security_definition: roq.codec.fix.SecurityDefinition):
#     print("security_definition={}, header={}".format(security_definition, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header, security_status_request: roq.codec.fix.SecurityStatusRequest
# ):
#     print("security_status_request={}, header={}".format(security_status_request, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, security_status: roq.codec.fix.SecurityStatus):
#     print("security_status={}, header={}".format(security_status, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, market_data_request: roq.codec.fix.MarketDataRequest):
#     print("market_data_request={}, header={}".format(market_data_request, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header, market_data_request_reject: roq.codec.fix.MarketDataRequestReject
# ):
#     print("market_data_request_reject={}, header={}".format(market_data_request_reject, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header,
#     market_data_snapshot_full_refresh: roq.codec.fix.MarketDataSnapshotFullRefresh,
# ):
#     print(
#         "market_data_snapshot_full_refresh={}, header={}".format(
#             market_data_snapshot_full_refresh, header
#         )
#     )


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header,
#     market_data_incremental_refresh: roq.codec.fix.MarketDataIncrementalRefresh,
# ):
#     print(
#         "market_data_incremental_refresh={}, header={}".format(
#             market_data_incremental_refresh, header
#         )
#     )


# @typedispatch
# def callback(header: roq.codec.fix.Header, order_status_request: roq.codec.fix.OrderStatusRequest):
#     print("order_status_request={}, header={}".format(order_status_request, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header, order_mass_status_request: roq.codec.fix.OrderMassStatusRequest
# ):
#     print("order_mass_status_request={}, header={}".format(order_mass_status_request, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, new_order_single: roq.codec.fix.NewOrderSingle):
#     print("new_order_single={}, header={}".format(new_order_single, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, order_cancel_request: roq.codec.fix.OrderCancelRequest):
#     print("order_cancel_request={}, header={}".format(order_cancel_request, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header,
#     order_cancel_replace_request: roq.codec.fix.OrderCancelReplaceRequest,
# ):
#     print("order_cancel_replace_request={}, header={}".format(order_cancel_replace_request, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header, order_mass_cancel_request: roq.codec.fix.OrderMassCancelRequest
# ):
#     print("order_mass_cancel_request={}, header={}".format(order_mass_cancel_request, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, order_cancel_reject: roq.codec.fix.OrderCancelReject):
#     print("order_cancel_reject={}, header={}".format(order_cancel_reject, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, execution_report: roq.codec.fix.ExecutionReport):
#     print("execution_report={}, header={}".format(execution_report, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header,
#     trade_capture_report_request: roq.codec.fix.TradeCaptureReportRequest,
# ):
#     print("trade_capture_report_request={}, header={}".format(trade_capture_report_request, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, trade_capture_report: roq.codec.fix.TradeCaptureReport):
#     print("trade_capture_report={}, header={}".format(trade_capture_report, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header, request_for_positions: roq.codec.fix.RequestForPositions
# ):
#     print("request_for_positions={}, header={}".format(request_for_positions, header))


# @typedispatch
# def callback(
#     header: roq.codec.fix.Header, request_for_positions_ack: roq.codec.fix.RequestForPositionsAck
# ):
#     print("request_for_positions_ack={}, header={}".format(request_for_positions_ack, header))


# @typedispatch
# def callback(header: roq.codec.fix.Header, position_report: roq.codec.fix.PositionReport):
#     print("position_report={}, header={}".format(position_report, header))


decoder = roq.codec.fix.Decoder()

# version 1
length = decoder.dispatch(callback, message)
print("length={}".format(length))
assert length == len(message)

# version 2
# header, length = decoder.decode(message)
# print("length={}".format(length))
# assert length==len(message)
# assert header.msg_type == roq.codec.fix.MsgType.LOGON
# logon = decoder.decode(roq.codec.fix.Logon, message)
# print('logon={}, header={}'.format(logon, header))

tmp = roq.codec.fix.SecListGrp(symbol="BTC-PERPETUAL", security_exchange="deribit")
security_list = roq.codec.fix.SecurityList(
    security_req_id="req1",
    security_response_id="resp1",
    security_request_result=roq.codec.fix.SecurityRequestResult.VALID,
    no_related_sym=[tmp],
)
print("security_list={}".format(security_list))

message = encoder.encode(security_list, datetime.now())

print('message="{}", length={}'.format(message.decode().replace(chr(1), "|"), len(message)))

length = decoder.dispatch(callback, message)

print("length={}".format(length))
