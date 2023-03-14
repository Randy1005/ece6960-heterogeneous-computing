

find_package(Threads REQUIRED)

add_library(threadpool threadpool.cpp)
target_link_libraries(threadpool Threads::Threads)

set_property(TARGET threadpool PROPERTY CXX_STANDARD 20)
target_compile_options(threadpool INTERFACE -Wall -Wextra -Wfatal-errors)
