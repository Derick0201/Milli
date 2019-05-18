set(ENABLE_TESTING FALSE CACHE BOOL "Generate test targets")

if (NOT ${ENABLE_TESTING})
    return()
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fcoroutines-ts")
endif ()

function(create_test)
    set(src SOURCES)
    set(cpp_standards CXX_STANDARDS SOURCES)
    set(name NAME)
    CMAKE_PARSE_ARGUMENTS(create_test "" "${name}" "${cpp_standards}" ${ARGN})

    if(NOT create_test_NAME)
        message(FATAL_ERROR "create_test function called without the NAME argument")
    endif()

    if(NOT create_test_SOURCES)
        message(FATAL_ERROR "create_test function called without the SOURCES argument")
    endif()

    if(NOT create_test_CXX_STANDARDS)
        list(APPEND create_test_CXX_STANDARDS "17")
    endif()

    foreach(standard ${create_test_CXX_STANDARDS})
        enable_testing()
        set(target_name "${create_test_NAME}_cpp${standard}")
        project(${target_name})
        set(CMAKE_CXX_STANDARD ${standard})
        add_executable(${target_name} ${create_test_SOURCES})
        target_compile_definitions(${target_name} PRIVATE MILLI_TEST)
        target_link_libraries(${target_name} Milli Boost::unit_test_framework)
        add_test(NAME ${target_name} COMMAND ${target_name})
    endforeach()
endfunction()

hunter_add_package(Boost COMPONENTS test)
find_package(Boost CONFIG REQUIRED unit_test_framework)

create_test(NAME raii_test SOURCES raii.cpp CXX_STANDARDS 17 20)
create_test(NAME make_container_test SOURCES make_container.cpp CXX_STANDARDS 17 20)
create_test(NAME repeat_test SOURCES repeat.cpp CXX_STANDARDS 17 20)
create_test(NAME strong_assert SOURCES strong_assert.cpp CXX_STANDARDS 17 20)
create_test(NAME lazy SOURCES lazy.cpp CXX_STANDARDS 17 20)
