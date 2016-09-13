find_package(Boost COMPONENTS python)

add_library(boost_math SHARED boost_math.cpp)
target_link_libraries(boost_math ${PYTHON_LIBRARY} ${CONAN_LIBS})
if(WIN32)
  set_target_properties(boost_math PROPERTIES SUFFIX ".pyd")
endif()
	