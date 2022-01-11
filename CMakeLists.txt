project(test)
SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -march=native -pthread -lgmp -lgmpxx")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 -march=native -pthread")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")
add_executable(bin/filesort
	src/filesort.hpp
	src/main.cpp
)
include_directories(/usr/local/boost/include)
#target_link_libraries(bin/algorithm libboost_thread.so libboost_system.so)
