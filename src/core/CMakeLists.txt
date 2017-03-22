add_executable(kronos kronos.cpp scheduler.cpp module.cpp ${SRC})
target_link_libraries(kronos ${CMAKE_THREAD_LIBS_INIT} sqlite3)
