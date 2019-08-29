set(SRCS
    channel.cpp
    common.cpp
    event_loop.cpp
    poller.cpp
    event_loop_thread_pool.cpp
    event_loop_thread.cpp
    http_content.cpp
    main.cpp
    server.cpp
)
include_directories(${PROJECT_SOURCE_DIR}/src/server/base)

add_executable(WebServer ${SRCS})
target_link_libraries(WebServer libserver_base)

add_subdirectory(base)
add_subdirectory(tests)