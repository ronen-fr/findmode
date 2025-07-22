cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(mode CXX)

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -O3 -g -fomit-frame-pointer -I/home/rfriedma/apps/fmt/include -L/home/rfriedma/apps/fmt /home/rfriedma/apps/fmt/libfmt.a")
find_package(benchmark REQUIRED)


#set(SOURCE mode.cc mclct_test.cc)
set(SOURCE mode.cc mode_pmr.cc h.cc)


add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} benchmark::benchmark /home/rfriedma/apps/fmt/libfmt.a)


