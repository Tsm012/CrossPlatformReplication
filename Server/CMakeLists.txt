# CMakeList.txt : CMake project for ShantzCMake, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ShantzCMake "src/ShantzCMake.cpp"  "include/WindowsServer.h"  "include/LinuxServer.h" "include/AbstractServer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ShantzCMake PROPERTY CXX_STANDARD 20)
endif()

#Link the pthreads to executable
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

target_link_libraries(ShantzCMake Threads::Threads)