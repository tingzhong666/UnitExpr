#include "test_framework.hpp"

void register_function_tests(TestRunner& runner) {
    std::cout << "注册函数测试..." << std::endl;
    
    // 1. 三角函数（角度单位）
    runner.add_test({"正弦函数", "sin(30deg)", "0.5", false});
    runner.add_test({"余弦函数", "cos(60deg)", "0.5", false});
    runner.add_test({"正切函数", "tan(45deg)", "1", false});
    
    // 2. 反三角函数
    runner.add_test({"反正弦", "asin(0.5) to deg", "30deg", false});
    runner.add_test({"反余弦", "acos(0.5) to deg", "60deg", false});
    runner.add_test({"反正切", "atan(1) to deg", "45deg", false});
    
    // 3. 指数和对数函数
    runner.add_test({"自然指数", "exp(1)", "2.71828", false});
    runner.add_test({"自然对数", "ln(2.71828)", "1", false});
    runner.add_test({"常用对数", "log10(100)", "2", false});
    
    // 4. 其他数学函数
    runner.add_test({"平方根", "sqrt(16)", "4", false});
    runner.add_test({"绝对值", "abs(-5)", "5", false});
    runner.add_test({"最大值", "max(3, 7, 2)", "7", false});
    runner.add_test({"最小值", "min(3, 7, 2)", "2", false});
}