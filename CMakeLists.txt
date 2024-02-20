cmake_minimum_required(VERSION 3.27)

project(typoglycemia)

file(GLOB_RECURSE SOURCE "Source/*.cpp" "Source/**/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE})
