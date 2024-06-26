cmake_minimum_required(VERSION 2.6.0)

set(PROJ_NAME phobos2390)
set(REPO_NAME IDLP)

project("${PROJ_NAME}-${REPO_NAME}")

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_ROOT_DIR ${PROJ_DIR}/src/${REPO_NAME})

set(APP_DIR ${CMAKE_CURRENT_BINARY_DIR}/apps)

include_directories("${PROJ_DIR}/catch2/single_include")
include_directories("${PROJ_DIR}/src/")

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj -g -std=c++20")

#set(CMAKE_C_COMPILER "/root/GCC-11.2.0/bin/gcc")
#set(CMAKE_CXX_COMPILER "/root/GCC-11.2.0/bin/g++")
#set(CMAKE_CXX_FLAGS "-g -std=c++20")

set(CMAKE_C_COMPILER "/opt/rh/devtoolset-11/root/bin/gcc")
set(CMAKE_CXX_COMPILER "/opt/rh/devtoolset-11/root/bin/g++")
set(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage -std=c++20")

set(SOURCE_DIRECTORIES ${SRC_ROOT_DIR})

set(all_source_files "")
set(all_app_files "")
set(all_test_files "")
set(all_header_files "")
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

  message("Calculating relative path to ${SRC_ROOT_DIR} ${source_dir}")
  file(RELATIVE_PATH relative_source_path ${SRC_ROOT_DIR} ${source_dir})
  set(dest_directory ${CMAKE_CURRENT_BINARY_DIR}/include/${REPO_NAME}/${relative_source_path})
  message("Making install directory at ${dest_directory}")
  file(MAKE_DIRECTORY ${dest_directory})

  file(GLOB dir_header_files ${source_dir}/*.h*)
  foreach(header_file ${dir_header_files})
      get_filename_component(header_file_name ${header_file} NAME)
      file(COPY_FILE ${header_file} ${dest_directory}/${header_file_name})
  endforeach()
endforeach()

set(LIB_NAME_SHARED ${PROJ_NAME}_${REPO_NAME})
set(LIB_NAME_STATIC ${PROJ_NAME}_${REPO_NAME}_s)
set(LIB_TEST_NAME "test_${PROJ_NAME}_${REPO_NAME}")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(EXTRA_LIBS -pthread)

message("Building static library ${LIB_NAME_STATIC} with ${all_source_files}")
add_library(${LIB_NAME_STATIC} STATIC ${all_source_files})
message("Building shared library ${LIB_NAME_SHARED} with ${all_source_files}")
add_library(${LIB_NAME_SHARED} SHARED ${all_source_files})

message("Making applications directory ${APP_DIR}")
file(MAKE_DIRECTORY ${APP_DIR})
foreach(app_file ${all_app_files})
  get_filename_component(app_name ${app_file} NAME_WE)
  message("Building app ${APP_DIR}/${app_name}")
  add_executable(${APP_DIR}/${app_name} ${app_file})
  target_link_libraries(${APP_DIR}/${app_name} ${LIB_NAME_STATIC})
endforeach()

add_executable(${LIB_TEST_NAME} ${all_test_files})
target_link_libraries(${LIB_TEST_NAME} ${LIB_NAME_STATIC} ${EXTRA_LIBS})

