cmake_minimum_required(VERSION 2.8)

project(kron)

option(KRON_DEBUG "Compile with debug symbols" OFF)

if(UNIX)
	set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
	find_package(Threads REQUIRED)
	if(CMAKE_USE_PTHREADS_INIT)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	endif()
	target_link_libraries (kronos ${CMAKE_THREAD_LIBS_INIT})
endif()

if(UNIX)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
if(APPLE)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/sched")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/core")
