add_executable (Reporter "reporter.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Main PROPERTY CXX_STANDARD 20)
endif()