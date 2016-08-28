find_package(Boost COMPONENTS python)

include_directories(${Boost_INCLUDE_DIRS})

add_library(boost1 SHARED boost1.cpp)
target_include_directories(boost1 PRIVATE ${PYTHON_INCLUDE_DIR})
target_link_libraries(boost1 ${PYTHON_LIBRARY} ${CONAN_LIBS})
if(WIN32)
  set_target_properties(boost1 PROPERTIES SUFFIX ".pyd")
endif()
	