cmake_minimum_required(VERSION 2.8.12)

project(kron)

option(KRON_DEBUG "Compile with debug symbols" OFF)

if(KRON_DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(UNIX)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
if(APPLE)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
if(UNIX AND NOT APPLE)
	set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
	find_package(Threads REQUIRED)
	if(CMAKE_USE_PTHREADS_INIT)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	endif()
endif()

add_compile_options(-DSPDLOG_ENABLE_SYSLOG)
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/external/spdlog/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/modules")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/core")
