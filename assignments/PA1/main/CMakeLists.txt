add_executable(main main.cpp)

# options/flags pass to the compiler
target_compile_features(threadpool INTERFACE cxx_std_20)
target_compile_options(threadpool INTERFACE -Wall -Wextra -Wfatal-errors)

# include and libary linking
target_link_libraries(main LINK_PUBLIC threadpool)
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR})

