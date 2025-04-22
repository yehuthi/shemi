cmake_minimum_required(VERSION 3.16)
project(shemi LANGUAGES C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<COMPILE_LANGUAGE:C>:
    $<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
    $<$<C_COMPILER_ID:MSVC>:/W4>
  >
)

option(SHEMI_ENABLE_AVX2 "Enable AVX2" ON)
option(SHEMI_ENABLE_SSE4_2 "Enable SSE4.2" ON)
if (SHEMI_ENABLE_AVX2)
	target_compile_options(${PROJECT_NAME} PUBLIC
		$<$<AND:$<CXX_COMPILER_ID:GNU,Clang>>:-mavx2>
		$<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
	)
endif()
if (SHEMI_ENABLE_SSE4_2)
	target_compile_options(${PROJECT_NAME} PUBLIC
		$<$<AND:$<CXX_COMPILER_ID:GNU,Clang>>:-msse4.2>
		$<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2>
	)
endif()

option(SHEMI_ENABLE_TESTING "Build test suite" OFF)
option(SHEMI_ENABLE_BENCHMARKS "Build benchmarks" OFF)
if (SHEMI_ENABLE_TESTING OR SHEMI_ENABLE_BENCHMARKS)
  enable_language(CXX)
  set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if (SHEMI_ENABLE_TESTING)
  enable_testing()
  find_package(GTest REQUIRED)
  add_executable(${PROJECT_NAME}_test test/suite.cpp)
  target_link_libraries(${PROJECT_NAME}_test GTest::gtest_main ${PROJECT_NAME})
  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME}_test)
endif()
if (SHEMI_ENABLE_BENCHMARKS)
  find_package(benchmark REQUIRED)
  add_executable(${PROJECT_NAME}_bench bench/bench.cpp)
  target_link_libraries(${PROJECT_NAME}_bench PRIVATE
    benchmark::benchmark_main ${PROJECT_NAME})
endif()
