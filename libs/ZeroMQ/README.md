# ZeroMQ + Protocol Buffers Example

This project demonstrates a simple client-server communication system using ZeroMQ for messaging and Protocol Buffers for message serialization.

## Overview

The example implements a Request-Reply pattern where:
- **Server** listens on port 5555 and processes incoming requests
- **Client** sends 10 messages to the server and receives responses
- **Messages** are structured using Protocol Buffers for type safety and cross-platform compatibility

## Architecture

### Message Format

The communication uses two Protocol Buffer message types defined in `message.proto`:

```protobuf
message Request {
  string content = 1;           // The actual message content
  int32 sequence_number = 2;    // Message sequence for ordering
  int64 timestamp = 3;          // Client-side timestamp
}

message Response {
  string content = 1;           // Echo response with processed content
  int32 sequence_number = 2;    // Matching sequence number from request
  bool success = 3;             // Processing status
  int64 timestamp = 4;          // Server-side timestamp
}
```

### Components

#### Server (`server.cpp`)
- Binds to `tcp://*:5555` using ZeroMQ REP (Reply) socket
- Receives serialized Protocol Buffer requests
- Deserializes messages and extracts content, sequence number, and timestamp
- Processes each request (adds 100ms delay to simulate work)
- Creates structured responses with echo content and server timestamp
- Serializes responses and sends back to client

#### Client (`client.cpp`)
- Connects to `tcp://localhost:5555` using ZeroMQ REQ (Request) socket
- Creates 10 structured requests with incremental sequence numbers
- Adds client-side timestamps to each message
- Serializes requests using Protocol Buffers
- Sends requests and waits for responses
- Deserializes and displays response details including server timestamps

## Prerequisites

### System Dependencies
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install libzmq3-dev libcppzmq-dev
sudo apt-get install libprotobuf-dev protobuf-compiler

# CentOS/RHEL/Fedora
sudo yum install cmake gcc-c++
sudo yum install zeromq-devel cppzmq-devel
sudo yum install protobuf-devel protobuf-compiler

# macOS (using Homebrew)
brew install cmake zeromq cppzmq protobuf
```

## Building the Project

```bash
# Clone or navigate to project directory
cd ZeroMQ

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the executables
make

# Alternatively, build with specific number of parallel jobs
make -j$(nproc)
```

### Build Artifacts
After successful build, you'll have:
- `server` - The server executable
- `client` - The client executable
- `message.pb.h` and `message.pb.cc` - Auto-generated Protocol Buffer files

## Running the Example

### Start the Server
Open a terminal and run:
```bash
cd build
./server
```

Expected output:
```
Server listening on port 5555...
Received: Hello 0 (seq: 0, timestamp: 1699123456789)
Received: Hello 1 (seq: 1, timestamp: 1699123456890)
...
```

### Start the Client
Open another terminal and run:
```bash
cd build
./client
```

Expected output:
```
Client connected to server on port 5555
Sending: Hello 0 (seq: 0)
Received: Echo: Hello 0 (seq: 0, success: true, timestamp: 1699123456892)
Sending: Hello 1 (seq: 1)
Received: Echo: Hello 1 (seq: 1, success: true, timestamp: 1699123456993)
...
```