
add_executable(simple_pipe	simple_pipe.cpp)
target_link_libraries(simple_pipe tlpi)

add_executable(pipe_sync	pipe_sync.cpp)
target_link_libraries(pipe_sync tlpi)

add_executable(redirect_stdout redirect_stdout.cpp)
target_link_libraries(redirect_stdout tlpi)

add_executable(pipe_ls_wc pipe_ls_wc.cpp)
target_link_libraries(pipe_ls_wc tlpi)
