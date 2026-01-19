cmake_minimum_required(VERSION 3.10)
project(UnitExprTests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/exprtk
    ${CMAKE_SOURCE_DIR}/includes/units
)

# 测试源文件
set(TEST_SOURCES
    main.cpp
    basic_tests.cpp
    unit_tests.cpp
    function_tests.cpp
    complex_tests.cpp
    error_tests.cpp
)

# 创建测试可执行文件
add_executable(unit_expr_tests ${TEST_SOURCES})

# 链接库
target_link_libraries(unit_expr_tests
    ${CMAKE_SOURCE_DIR}/libs/units.lib
)

# 输出目录
set_target_properties(unit_expr_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)