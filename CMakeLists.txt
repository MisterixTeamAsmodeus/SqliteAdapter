cmake_minimum_required(VERSION 3.8)

set(sqlite_adapter_project_name SqliteAdapter)
option(SQLITE_ADAPTER_EXAMPLE "Add examples file as executable (on|off)" OFF)

project(${sqlite_adapter_project_name} LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if (NOT TARGET SQLite3)
    add_subdirectory(include/external/sqlite-amalgamation)
endif ()

if (NOT TARGET DatabaseAdapter)
    add_subdirectory(include/external/DatabaseAdapter)
endif ()

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        DatabaseAdapter
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/external
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        SQLite3
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

install(DIRECTORY include/
        DESTINATION include
)
