# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(UnitExpr)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_options(/bigobj)
endif()


# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/exprtk
    ${CMAKE_SOURCE_DIR}/includes/units
    ${CMAKE_SOURCE_DIR}/includes/angelscript
)

# 源文件
set(unit_expr_SOURCES
    src/main.cpp
)

# 创建测试可执行文件
add_executable(unit_expr ${unit_expr_SOURCES})

# 链接库
target_link_libraries(unit_expr
    ${CMAKE_SOURCE_DIR}/libs/units.lib
    ${CMAKE_SOURCE_DIR}/libs/angelscript.lib
)

# 输出目录
set_target_properties(unit_expr PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
