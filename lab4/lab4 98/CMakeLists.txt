cmake_minimum_required(VERSION 3.10)
include(FetchContent)
project(Lab4_98)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(Receiver
    Receiver/start.cpp
    Receiver/Receiver.cpp
)
target_include_directories(Receiver PRIVATE Receiver)

add_executable(Sender
    Sender/start.cpp
    Sender/Sender.cpp
)

target_include_directories(Sender PRIVATE Sender)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests tests/TestReceiverSender.cpp Sender/Sender.cpp Receiver/Receiver.cpp)
target_link_libraries(tests PRIVATE gtest gtest_main)
target_include_directories(tests PRIVATE Sender PRIVATE Receiver)

enable_testing()
add_test(NAME tests COMMAND tests)



