#include "test_framework.hpp"

// 测试用例定义将在单独的模块中
extern void register_basic_tests(TestRunner& runner);
extern void register_unit_tests(TestRunner& runner);
extern void register_function_tests(TestRunner& runner);
extern void register_complex_tests(TestRunner& runner);
extern void register_error_tests(TestRunner& runner);

int main() {
    TestRunner runner;
    
    // 注册所有测试
    register_basic_tests(runner);
    register_unit_tests(runner);
    register_function_tests(runner);
    register_complex_tests(runner);
    register_error_tests(runner);
    
    // 运行测试
    runner.run_all();
    
    return 0;
}