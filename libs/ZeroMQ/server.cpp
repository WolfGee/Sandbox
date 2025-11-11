#include <zmq.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "message.pb.h"

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    
    socket.bind("tcp://*:5555");
    
    std::cout << "Server listening on port 5555..." << std::endl;
    
    while (true) {
        zmq::message_t request_msg;
        
        // Receive request from client
        auto result = socket.recv(request_msg, zmq::recv_flags::none);
        if (result) {
            // Parse protobuf request
            zmq_example::Request request;
            if (request.ParseFromArray(request_msg.data(), request_msg.size())) {
                std::cout << "Received: " << request.content() 
                         << " (seq: " << request.sequence_number() 
                         << ", timestamp: " << request.timestamp() << ")" << std::endl;
                
                // Process request
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                // Create protobuf response
                zmq_example::Response response;
                response.set_content("Echo: " + request.content());
                response.set_sequence_number(request.sequence_number());
                response.set_success(true);
                response.set_timestamp(
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()
                    ).count()
                );
                
                // Serialize and send response
                std::string serialized_response;
                response.SerializeToString(&serialized_response);
                zmq::message_t response_msg(serialized_response.size());
                memcpy(response_msg.data(), serialized_response.c_str(), serialized_response.size());
                socket.send(response_msg, zmq::send_flags::none);
            } else {
                std::cerr << "Failed to parse protobuf request" << std::endl;
            }
        }
    }
    
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}