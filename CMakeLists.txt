project(filesort)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -march=native -pthread")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")
add_executable(bin/filesort
	src/file_sort.hpp
	src/file_sort_opt.hpp
	src/main.cpp
)
install(TARGETS bin/filesort DESTINATION bin/)
