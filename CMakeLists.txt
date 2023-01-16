cmake_minimum_required(VERSION 3.0.0)
project(cmakeSample VERSION 0.1.0)

include(CTest)
enable_testing()

# cmakelists 设置c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")

endif()
# cmakelists 设置c++11


link_directories(
    ./lib
)

add_executable(cmakeSample
 src/main.cpp
 src/audio_record.cpp
 src/log.cpp
 src/audio_resample.cpp
 src/include/audio_record.hpp
 src/include/log.hpp
 src/include/audio_resample.hpp
 )

include_directories(
    ./include
    ./src/include
)

target_link_libraries(${PROJECT_NAME} -lavdevice -lavutil -lavformat -lswresample)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
