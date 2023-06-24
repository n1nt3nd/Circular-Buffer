include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(CCircularBufferTests CCircularBufferTests.cpp)

target_link_libraries(
        CCircularBufferTests
        c_circular_buffer
        c_circular_buffer_ext
        c_circular_buffer_iter
        GTest::gtest_main
)

target_include_directories(CCircularBufferTests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(CCircularBufferTests)
