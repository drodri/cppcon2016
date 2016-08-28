cmake_minimum_required(VERSION 3.5)

project(PythonCpp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(PYTHON_INCLUDE_DIR "C:/Python27/include")
set(PYTHON_LIBRARY "C:/Python27/libs/python27.lib")

add_subdirectory(boost)
add_subdirectory(pybind)