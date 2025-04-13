cmake_minimum_required(VERSION 3.10)
project(Lab4_98)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Убираем предупреждения безопасности в MSVC
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# ================= RECEIVER =================
add_executable(Receiver
    Receiver/start.cpp
    Receiver/Receiver.cpp
)

target_include_directories(Receiver PRIVATE Receiver)

# ================= SENDER =================
add_executable(Sender
    Sender/start.cpp
    Sender/Sender.cpp
)

target_include_directories(Sender PRIVATE Sender)
