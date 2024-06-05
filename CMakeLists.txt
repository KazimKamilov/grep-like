cmake_minimum_required(VERSION 3.28.2)
project(grep-like LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

set(ROOT_FOLDER "source")

file(GLOB APP_MODULES ${CMAKE_CURRENT_LIST_DIR}/source/*.cppm)
source_group(${ROOT_FOLDER} FILE_SET CXX_MODULES FILES ${APP_MODULES} ${CMAKE_CURRENT_LIST_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${APP_MODULES} ${CMAKE_CURRENT_LIST_DIR}/source/Main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE /MP /MT)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/DEBUG")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin")

################################
# SOME TESTS
################################

file(GLOB TESTS_FILES ${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp)

foreach(TEST_FILE ${TESTS_FILES})
    file(GLOB APP_MODULES ${CMAKE_CURRENT_LIST_DIR}/source/*.cppm)
    source_group(${ROOT_FOLDER} FILE_SET CXX_MODULES FILES ${APP_MODULES} ${TEST_FILE})
    get_filename_component(TEST_FILENAME ${TEST_FILE} NAME_WE)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/deps/catch2/include)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/deps/catch2/lib)

    add_executable(${TEST_FILENAME} ${APP_MODULES} ${TEST_FILE})

    target_compile_options(${TEST_FILENAME} PRIVATE /MP /MT)
    target_link_libraries(${TEST_FILENAME} PRIVATE Catch2 Catch2Main)
    set_target_properties(${TEST_FILENAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin")
    set_target_properties(${TEST_FILENAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin")
endforeach()
