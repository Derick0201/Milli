set(ENABLE_TESTING FALSE CACHE BOOL "Generate test targets")

if (NOT ${ENABLE_TESTING})
    return()
endif ()

set(CMAKE_CXX_STANDARD 11)

hunter_add_package(Boost COMPONENTS test)
find_package(Boost CONFIG REQUIRED unit_test_framework)

enable_testing()

project(raii_test)

add_executable(${PROJECT_NAME} raii.cpp)

target_link_libraries(${PROJECT_NAME} Milli Boost::unit_test_framework)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

project(move_initializer_list_test)

add_executable(${PROJECT_NAME} make_container.cpp)

target_link_libraries(${PROJECT_NAME} Milli Boost::unit_test_framework)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

project(repeat_test)

add_executable(${PROJECT_NAME} repeat.cpp)

target_link_libraries(${PROJECT_NAME} Milli Boost::unit_test_framework)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

project(repeat_test_cpp14)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} repeat.cpp)

target_link_libraries(${PROJECT_NAME} Milli Boost::unit_test_framework)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})