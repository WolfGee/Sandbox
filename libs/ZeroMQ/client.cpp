#include <zmq.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include "message.pb.h"

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    
    socket.connect("tcp://localhost:5555");
    
    std::cout << "Client connected to server on port 5555" << std::endl;
    
    for (int i = 0; i < 10; ++i) {
        // Create protobuf request
        zmq_example::Request request;
        request.set_content("Hello " + std::to_string(i));
        request.set_sequence_number(i);
        request.set_timestamp(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count()
        );
        
        // Serialize and send request
        std::string serialized_request;
        request.SerializeToString(&serialized_request);
        zmq::message_t request_msg(serialized_request.size());
        memcpy(request_msg.data(), serialized_request.c_str(), serialized_request.size());
        
        std::cout << "Sending: " << request.content() 
                 << " (seq: " << request.sequence_number() << ")" << std::endl;
        socket.send(request_msg, zmq::send_flags::none);
        
        // Receive reply
        zmq::message_t reply_msg;
        auto result = socket.recv(reply_msg, zmq::recv_flags::none);
        if (result) {
            // Parse protobuf response
            zmq_example::Response response;
            if (response.ParseFromArray(reply_msg.data(), reply_msg.size())) {
                std::cout << "Received: " << response.content() 
                         << " (seq: " << response.sequence_number()
                         << ", success: " << (response.success() ? "true" : "false")
                         << ", timestamp: " << response.timestamp() << ")" << std::endl;
            } else {
                std::cerr << "Failed to parse protobuf response" << std::endl;
            }
        }
    }
    
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}