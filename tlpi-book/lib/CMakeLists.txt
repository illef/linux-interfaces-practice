file(GLOB_RECURSE SRC "*.c")

add_library(tlpi ${SRC})
target_include_directories(tlpi PUBLIC ./)
