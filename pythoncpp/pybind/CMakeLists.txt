find_package(PythonLibs REQUIRED)


# The above sometimes returns version numbers like "3.4.3+"; the "+" must be removed for the next lines to work
string(REPLACE "+" "" PYTHONLIBS_VERSION_STRING "+${PYTHONLIBS_VERSION_STRING}")

# Uncomment the following line if you will also require a matching Python interpreter
# find_package(PythonInterp ${PYTHONLIBS_VERSION_STRING} EXACT REQUIRED)


# Include path for Python header files
include_directories(${PYTHON_INCLUDE_DIR})

# Include path for pybind11 header files -- this may need to be changed depending on your setup
include_directories(${PROJECT_SOURCE_DIR}/pybind11/include)

# Create the binding library
add_library(example SHARED
  pybind1.cpp
  # ... extra files go here ...
)

# Don't add a 'lib' prefix to the shared library
set_target_properties(example PROPERTIES PREFIX "")

if (WIN32)
  if (MSVC)
    # /bigobj is needed for bigger binding projects due to the limit to 64k
    # addressable sections. /MP enables multithreaded builds (relevant when
    # there are many files).
    set_target_properties(example PROPERTIES COMPILE_FLAGS "/MP /bigobj ")

    if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
      # Enforce size-based optimization and link time code generation on MSVC
      # (~30% smaller binaries in experiments).
      set_target_properties(example APPEND_STRING PROPERTY COMPILE_FLAGS "/Os /GL ")
      set_target_properties(example APPEND_STRING PROPERTY LINK_FLAGS "/LTCG ")
    endif()
  endif()

  # .PYD file extension on Windows
  set_target_properties(example PROPERTIES SUFFIX ".pyd")

  # Link against the Python shared library
  target_link_libraries(example ${PYTHON_LIBRARY})
elseif (UNIX)
  # It's quite common to have multiple copies of the same Python version
  # installed on one's system. E.g.: one copy from the OS and another copy
  # that's statically linked into an application like Blender or Maya.
  # If we link our plugin library against the OS Python here and import it
  # into Blender or Maya later on, this will cause segfaults when multiple
  # conflicting Python instances are active at the same time (even when they
  # are of the same version).

  # Windows is not affected by this issue since it handles DLL imports
  # differently. The solution for Linux and Mac OS is simple: we just don't
  # link against the Python library. The resulting shared library will have
  # missing symbols, but that's perfectly fine -- they will be resolved at
  # import time.

  # .SO file extension on Linux/Mac OS
  set_target_properties(example PROPERTIES SUFFIX ".so")

  # Strip unnecessary sections of the binary on Linux/Mac OS
  if(APPLE)
    set_target_properties(example PROPERTIES MACOSX_RPATH ".")
    set_target_properties(example PROPERTIES LINK_FLAGS "-undefined dynamic_lookup ")
    if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
      add_custom_command(TARGET example POST_BUILD COMMAND strip -u -r ${PROJECT_BINARY_DIR}/example.so)
    endif()
  else()
    if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
      add_custom_command(TARGET example POST_BUILD COMMAND strip ${PROJECT_BINARY_DIR}/example.so)
    endif()
  endif()
endif()
