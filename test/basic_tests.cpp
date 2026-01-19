#include "test_framework.hpp"

void register_basic_tests(TestRunner& runner) {
    std::cout << "注册基础数值运算测试..." << std::endl;
    
    // 1. 简单数值计算
    runner.add_test({"基础加法", "2 + 3", "5", false});
    runner.add_test({"基础减法", "5 - 3", "2", false});
    runner.add_test({"基础乘法", "2 * 3", "6", false});
    runner.add_test({"基础除法", "6 / 2", "3", false});
    
    // 2. 浮点数运算
    runner.add_test({"浮点加法", "2.5 + 3.5", "6", false});
    runner.add_test({"浮点乘法", "2.5 * 4", "10", false});
    runner.add_test({"浮点除法", "10.0 / 4.0", "2.5", false});
    
    // 3. 复杂算术运算
    runner.add_test({"混合运算1", "2 + 3 * 4", "14", false});
    runner.add_test({"混合运算2", "(2 + 3) * 4", "20", false});
    runner.add_test({"指数运算", "2 ^ 3", "8", false});
    
    // 4. 负数运算
    runner.add_test({"负数加法", "-5 + 3", "-2", false});
    runner.add_test({"负数乘法", "-2 * -3", "6", false});
    runner.add_test({"带括号负数", "-(2 + 3)", "-5", false});
}