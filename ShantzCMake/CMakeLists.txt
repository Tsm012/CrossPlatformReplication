# CMakeList.txt : CMake project for ShantzCMake, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ShantzCMake "ShantzCMake.cpp" "ShantzCMake.h" "Socket.h" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ShantzCMake PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
