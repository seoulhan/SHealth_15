cmake_minimum_required(VERSION 3.10)
project(CoreProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main library
add_library(shealth_lib
    src/main/cpp/SHealth.cpp
    src/main/cpp/SHealthDomain.cpp
    src/main/cpp/SHealthCsvLoader.cpp
    src/main/cpp/SHealthImputer.cpp
    src/main/cpp/SHealthStatistics.cpp
    src/main/cpp/SHealthPresenter.cpp
)
target_include_directories(shealth_lib PUBLIC src/main/cpp)

# Main executable
add_executable(SHealthBMI src/main/cpp/SHealthBMI.cpp)
target_link_libraries(SHealthBMI PRIVATE shealth_lib)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test executable
add_executable(SHealthBMITest src/test/cpp/SHealthBMITest.cpp)
target_compile_definitions(SHealthBMITest PRIVATE
    SHEALTH_TESTING
    "SHEALTH_TEST_FIXTURE_DIR=\"${CMAKE_SOURCE_DIR}/src/test/fixtures\""
    "SHEALTH_TEST_DATA_FILE=\"${CMAKE_SOURCE_DIR}/shealth.dat\""
)
target_link_libraries(SHealthBMITest PRIVATE shealth_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(SHealthBMITest)

# Golden Master (SHealthBMI stdout vs shealth.dat baseline)
add_executable(SHealthGoldenTest src/test/cpp/SHealthGoldenTest.cpp)
target_compile_definitions(SHealthGoldenTest PRIVATE
    "SHEALTH_GOLDEN_BASELINE_FILE=\"${CMAKE_SOURCE_DIR}/tests/golden/shealth_bmi_stdout.golden.txt\""
    "SHEALTH_GOLDEN_DATA_FILE=\"${CMAKE_SOURCE_DIR}/shealth.dat\""
    "SHEALTH_GOLDEN_BINARY_DIR=\"${CMAKE_BINARY_DIR}\""
    "SHEALTH_GOLDEN_SHEALTH_BMI_EXE=\"$<TARGET_FILE:SHealthBMI>\""
)
target_link_libraries(SHealthGoldenTest PRIVATE GTest::gtest_main)
add_dependencies(SHealthGoldenTest SHealthBMI)
gtest_discover_tests(SHealthGoldenTest)
