cmake_minimum_required(VERSION 2.6.0)

set(PROJ_NAME phobos2390)
set(REPO_NAME IDLP)

project("${PROJ_NAME}-${REPO_NAME}")

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_ROOT_DIR ${PROJ_DIR}/src/${REPO_NAME})

include_directories("${PROJ_DIR}/catch2/single_include")
include_directories("${PROJ_DIR}/src/")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj -g -std=c++20")

set(SOURCE_DIRECTORIES ${SRC_ROOT_DIR})

set(all_source_files "")
set(all_app_files "")
set(all_test_files "")
foreach(source_dir ${SOURCE_DIRECTORIES})
  message("Finding everything in ${source_dir}")
  file(GLOB dir_src_files ${source_dir}/*.cpp)
  set(all_source_files ${all_source_files}
                       ${dir_src_files})
  file(GLOB dir_app_files ${source_dir}/app/*.cpp)
  set(all_app_files ${all_app_files}
                    ${dir_app_files})
  file(GLOB dir_test_files ${source_dir}/test/*.cpp)
  set(all_test_files ${all_test_files}
                     ${dir_test_files})
endforeach()

set(LIB_NAME_SHARED ${PROJ_NAME}_${REPO_NAME})
set(LIB_NAME_STATIC ${PROJ_NAME}_${REPO_NAME}_s)
set(LIB_TEST_NAME "test_${PROJ_NAME}_${REPO_NAME}")

message("Building static library ${LIB_NAME_STATIC} with ${all_source_files}")
add_library(${LIB_NAME_STATIC} STATIC ${all_source_files})
message("Building shared library ${LIB_NAME_SHARED} with ${all_source_files}")
add_library(${LIB_NAME_SHARED} SHARED ${all_source_files})

foreach(app_file ${all_app_files})
  get_filename_component(app_name ${app_file} NAME_WE)
  message("Building app ${app_name}")
  add_executable(${app_name} ${app_file})
  target_link_libraries(${app_name} ${LIB_NAME_STATIC})
endforeach()

add_executable(${LIB_TEST_NAME} ${all_test_files})
target_link_libraries(${LIB_TEST_NAME} ${LIB_NAME_STATIC})
