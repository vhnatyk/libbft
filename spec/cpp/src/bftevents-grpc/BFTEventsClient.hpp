#ifndef BFTEVENTSCLIENT_HPP
#define BFTEVENTSCLIENT_HPP

#include <memory>
#include <sstream>
#include <string>

#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using bftevent::BFTEvent;
using bftevent::EventInform;
using bftevent::EventReply;

class BFTEventsClient
{
private:
   std::unique_ptr<BFTEvent::Stub> stub_;

public:
   int me{ 0 };

   BFTEventsClient(int _me, std::string toAddress)
     : stub_(
         BFTEvent::NewStub(
           std::shared_ptr<Channel>(
             grpc::CreateChannel(toAddress, grpc::InsecureChannelCredentials()))))
     , me(_me)
   {
      //
   }

   explicit BFTEventsClient(int _me)
     : stub_(
         BFTEvent::NewStub(
           std::shared_ptr<Channel>(
             grpc::CreateChannel(getAddress(_me), grpc::InsecureChannelCredentials()))))
     , me(_me)
   {
      //
   }

   // helper function to calculate address
   static std::string getAddress(int me)
   {
      std::stringstream ss;
      ss << "0.0.0.0:500" << me; // 0 -> 5000
      std::string _address = ss.str();
      return _address;
   }

   bool informEvent(int from, std::string event, std::vector<std::string> eventArgs, int delayMS = 0)
   {
      EventInform request;

      request.set_from(from);
      request.set_event(event);
      request.set_delay(delayMS);

      for (const auto & eventArg : eventArgs) {
         request.add_event_args(eventArg);
      }

      EventReply reply;

      ClientContext context;

      Status status = stub_->informEvent(&context, request, &reply);

      return status.ok();
      /*
      if (status.ok()) {
         return reply.gotit();
      } else {
         std::cout << status.error_code() << ": " << status.error_message() << std::endl;
         return -1;
      }
      */
   }
};

using TBFTEventsClient = std::shared_ptr<BFTEventsClient>;

#endif // BFTEVENTSCLIENT_HPP
