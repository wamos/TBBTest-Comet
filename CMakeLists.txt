 	###
	# Reference: https://github.com/01org/tbb/tree/tbb_2018/cmake#source-code-integration-of-intel-tbb-to-the-sub-string-finder-sample-linux-os
	###
	cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
	project(tbb_stages CXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g")	
	add_executable(async_thread async_thread.cpp)
	add_executable(async_basic async_basic.cpp)

	include(${TBB_ROOT}/cmake/TBBBuild.cmake)
	tbb_build(TBB_ROOT ${TBB_ROOT} CONFIG_DIR TBB_DIR MAKE_ARGS tbb_cpf=1)
	find_package(TBB REQUIRED tbb_preview)
	# Link Intel TBB imported targets to the executable;
	# "TBB::tbb_preview" can be used instead of "${TBB_IMPORTED_TARGETS}".
	target_link_libraries(async_thread ${TBB_IMPORTED_TARGETS})
	target_link_libraries(async_basic ${TBB_IMPORTED_TARGETS})
