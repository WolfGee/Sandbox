cmake_minimum_required(VERSION 3.12)
project(ZeroMQ_Examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Find cppzmq
find_path(CPPZMQ_INCLUDE_DIR zmq.hpp)

# Find Protobuf
find_package(Protobuf REQUIRED)

# Generate protobuf files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS message.proto)

# Include generated protobuf headers
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Server executable
add_executable(server server.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(server ${ZMQ_LIBRARIES} ${Protobuf_LIBRARIES})
target_include_directories(server PRIVATE ${ZMQ_INCLUDE_DIRS} ${CPPZMQ_INCLUDE_DIR} ${Protobuf_INCLUDE_DIRS})
target_compile_options(server PRIVATE ${ZMQ_CFLAGS_OTHER})

# Client executable
add_executable(client client.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(client ${ZMQ_LIBRARIES} ${Protobuf_LIBRARIES})
target_include_directories(client PRIVATE ${ZMQ_INCLUDE_DIRS} ${CPPZMQ_INCLUDE_DIR} ${Protobuf_INCLUDE_DIRS})
target_compile_options(client PRIVATE ${ZMQ_CFLAGS_OTHER})