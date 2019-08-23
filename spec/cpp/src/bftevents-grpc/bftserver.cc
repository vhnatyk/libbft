#include <string>

#include "bftevent.grpc.pb.h" // generate by protoc (see "bftevent.proto")
#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using bftevent::BFTEvent;
using bftevent::EventInform;
using bftevent::EventReply;

class BFTEventService final : public BFTEvent::Service
{
   Status sendRequest(ServerContext* context, const EventInform* request, EventReply* reply) override
   {
      int from = request->from();
      std::string message = request->message();

      int gotit = 99;

      reply->set_gotit(gotit);

      return Status::OK;
   }
};

using namespace std;

void
Run()
{
   std::string address("0.0.0.0:5000");
   BFTEventService service;

   ServerBuilder builder;

   builder.AddListeningPort(address, grpc::InsecureServerCredentials());
   builder.RegisterService(&service);

   std::unique_ptr<Server> server(builder.BuildAndStart());
   std::cout << "Server listening on port: " << address << std::endl;

   server->Wait();
}

int
main(int argc, char** argv)
{
   Run();

   return 0;
}
