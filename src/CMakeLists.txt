# find source files
file(GLOB_RECURSE ARF_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/arf/*.cpp)

add_library(argreffunc STATIC ${ARF_SOURCES})

target_compile_features(argreffunc
        PRIVATE cxx_range_for cxx_auto_type)

target_include_directories(argreffunc
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
