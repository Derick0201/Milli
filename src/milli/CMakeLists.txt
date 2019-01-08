project(Milli)

set(SOURCES
        raii.hpp
        strong_assert.hpp
        optional.hpp
        make_container.hpp
        repeat.hpp)

set(ABSOLUTE_SOURCES "")

foreach(file ${SOURCES})
    list(APPEND ABSOLUTE_SOURCES "${PROJECT_SOURCE_DIR}/${file}")
endforeach()

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${ABSOLUTE_SOURCES})
target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/../")