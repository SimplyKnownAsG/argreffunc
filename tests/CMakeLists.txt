# get the test engine (Catch)
file(DOWNLOAD
        https://github.com/catchorg/Catch2/releases/download/v1.12.2/catch.hpp
        ${CMAKE_BINARY_DIR}/3rd-party/include/catch.hpp)

file(GLOB TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Make test executable
add_executable(arf_tests ${TEST_SOURCES})

target_link_libraries(arf_tests argreffunc)
target_compile_features(arf_tests PRIVATE cxx_range_for cxx_auto_type)

target_include_directories(arf_tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_BINARY_DIR}/3rd-party/include)

# the ctest thing is far too complicated...
add_test(NAME arf_tests
         COMMAND $<TARGET_FILE:arf_tests>
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
         )

add_custom_target(run_tests
        COMMAND $<TARGET_FILE:arf_tests>)

