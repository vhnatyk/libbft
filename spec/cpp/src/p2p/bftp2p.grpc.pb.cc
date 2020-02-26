// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: bftp2p.proto

#include "bftp2p.pb.h"
#include "bftp2p.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace p2p {

static const char* P2P_method_names[] = {
  "/p2p.P2P/register_me",
  "/p2p.P2P/update_services",
};

std::unique_ptr< P2P::Stub> P2P::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< P2P::Stub> stub(new P2P::Stub(channel));
  return stub;
}

P2P::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_register_me_(P2P_method_names[0], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_update_services_(P2P_method_names[1], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientReader< ::p2p::Url>* P2P::Stub::register_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request) {
  return ::grpc::internal::ClientReaderFactory< ::p2p::Url>::Create(channel_.get(), rpcmethod_register_me_, context, request);
}

void P2P::Stub::experimental_async::register_me(::grpc::ClientContext* context, ::p2p::Url* request, ::grpc::experimental::ClientReadReactor< ::p2p::Url>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::p2p::Url>::Create(stub_->channel_.get(), stub_->rpcmethod_register_me_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::p2p::Url>* P2P::Stub::Asyncregister_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::p2p::Url>::Create(channel_.get(), cq, rpcmethod_register_me_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::p2p::Url>* P2P::Stub::PrepareAsyncregister_meRaw(::grpc::ClientContext* context, const ::p2p::Url& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::p2p::Url>::Create(channel_.get(), cq, rpcmethod_register_me_, context, request, false, nullptr);
}

::grpc::ClientReaderWriter< ::p2p::Url, ::p2p::Url>* P2P::Stub::update_servicesRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::p2p::Url, ::p2p::Url>::Create(channel_.get(), rpcmethod_update_services_, context);
}

void P2P::Stub::experimental_async::update_services(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::p2p::Url,::p2p::Url>* reactor) {
  ::grpc::internal::ClientCallbackReaderWriterFactory< ::p2p::Url,::p2p::Url>::Create(stub_->channel_.get(), stub_->rpcmethod_update_services_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>* P2P::Stub::Asyncupdate_servicesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::p2p::Url, ::p2p::Url>::Create(channel_.get(), cq, rpcmethod_update_services_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::p2p::Url, ::p2p::Url>* P2P::Stub::PrepareAsyncupdate_servicesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::p2p::Url, ::p2p::Url>::Create(channel_.get(), cq, rpcmethod_update_services_, context, false, nullptr);
}

P2P::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      P2P_method_names[0],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< P2P::Service, ::p2p::Url, ::p2p::Url>(
          std::mem_fn(&P2P::Service::register_me), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      P2P_method_names[1],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< P2P::Service, ::p2p::Url, ::p2p::Url>(
          std::mem_fn(&P2P::Service::update_services), this)));
}

P2P::Service::~Service() {
}

::grpc::Status P2P::Service::register_me(::grpc::ServerContext* context, const ::p2p::Url* request, ::grpc::ServerWriter< ::p2p::Url>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status P2P::Service::update_services(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::p2p::Url, ::p2p::Url>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace p2p
