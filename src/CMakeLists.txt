# find source files
file(GLOB_RECURSE ARF_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/arf/*.cpp)

add_library(argreffunc STATIC ${ARF_SOURCES})

# TODO: note the interface is more leaky than it should be.
target_compile_features(argreffunc
        PUBLIC cxx_range_for cxx_auto_type cxx_strong_enums)

target_include_directories(argreffunc
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
