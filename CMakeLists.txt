cmake_minimum_required(VERSION 3.14)
project(clich)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# 出力ファイルを指定
add_executable(clich clich.cpp)
add_executable(clich_server clich_server.cpp)
target_link_libraries(clich PRIVATE ftxui::component)
target_link_libraries(clich_server PRIVATE ftxui::component)
