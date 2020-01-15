cmake_minimum_required(VERSION 3.14)
project(gpa_calculator)
set(cmake_c_compiler clang)
set(cmake_cxx_compiler clang++)
set(cmake_cxx_standard_required on)
set(THREADS_PREFER_PTHREAD_FLAG ON)


add_executable(calculate calculate.cc)
set_property(TARGET calculate PROPERTY CXX_STANDARD 17)
target_compile_options(calculate PRIVATE -Werror -Wall -Wextra)
