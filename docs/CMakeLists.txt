
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )

# The doxygen_and_docs command couldn't find logos and such..
add_custom_target(docs
        doxygen ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} argreffunc)
endforeach()

add_test(NAME CompareToBoost0
        COMMAND $<TARGET_FILE:CompareToBoost0>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(CompareToBoost0 PROPERTIES
        PASS_REGULAR_EXPRESSION "Compression level is 0")
add_test(NAME CompareToBoost0-compression1
        COMMAND $<TARGET_FILE:CompareToBoost0> --compression=1
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(CompareToBoost0-compression1 PROPERTIES
        PASS_REGULAR_EXPRESSION "Compression level is 1")
add_test(NAME CompareToBoost0-help
        COMMAND $<TARGET_FILE:CompareToBoost0> --help
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME CompareToBoost1-help
        COMMAND $<TARGET_FILE:CompareToBoost1> --help
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(CompareToBoost0-help CompareToBoost1-help PROPERTIES
        PASS_REGULAR_EXPRESSION "--compression <compression>")

add_test(NAME CompareToTCLAP
         COMMAND $<TARGET_FILE:CompareToTCLAP>
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(CompareToTCLAP PROPERTIES
        PASS_REGULAR_EXPRESSION "My name is: homer")
add_test(NAME CompareToTCLAP-name
         COMMAND $<TARGET_FILE:CompareToTCLAP> --name you
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(CompareToTCLAP-name PROPERTIES
        PASS_REGULAR_EXPRESSION "My name is: you")
add_test(NAME CompareToTCLAP-name-reverse
         COMMAND $<TARGET_FILE:CompareToTCLAP> --name you --reverse
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(CompareToTCLAP-name-reverse PROPERTIES
         PASS_REGULAR_EXPRESSION "My name \\(spelled backwards\\) is: uoy")


