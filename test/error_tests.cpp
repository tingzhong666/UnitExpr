#include "test_framework.hpp"

void register_error_tests(TestRunner& runner) {
    std::cout << "注册错误处理测试..." << std::endl;
    
    // 1. 单位不匹配错误
    runner.add_test({"长度加时间", "2m + 3s", "", true});
    runner.add_test({"质量加速度度", "5kg * 2m/s", "", true});
    runner.add_test({"不同单位加法", "2m + 3kg", "", true});
    
    // 2. 数学错误
    runner.add_test({"除以零", "5 / 0", "", true});
    runner.add_test({"负数平方根", "sqrt(-4)", "", true});
    runner.add_test({"无效对数", "log10(-10)", "", true});
    
    // 3. 语法错误
    runner.add_test({"缺少运算符", "2 3", "", true});
    runner.add_test({"括号不匹配", "(2 + 3", "", true});
    runner.add_test({"无效函数名", "foo(5)", "", true});
    
    // 4. 语义错误
    runner.add_test({"未定义变量", "x + 5", "", true});
    runner.add_test({"无效单位", "5foo", "", true});
    runner.add_test({"单位转换错误", "5m to kg", "", true});
}