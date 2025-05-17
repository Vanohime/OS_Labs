# CMakeList.txt : CMake project for lab5 C++ 98, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("lab5 C++ 14")

# Add source to this project's executable.



add_executable (Server   "Client/Client.h" "Shared/Employee.h"  "Client/Client.cpp" "Server/Server.h" "Server/Server.cpp" "ServerMain.cpp")
add_executable (Client "Client/Client.h" "Shared/Employee.h"  "Client/Client.cpp" "ClientMain.cpp")



find_package(CxxTest)
if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  add_executable (Tests "tests/test_server.h" "tests/test_runner.cpp" "Client/Client.h" "Shared/Employee.h"  "Client/Client.cpp" "Server/Server.h" "Server/Server.cpp")

  enable_testing()
  add_test(NAME Tests COMMAND Tests)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 20)
  set_property(TARGET Client PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

