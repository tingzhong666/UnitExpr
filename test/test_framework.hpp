#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

struct TestCase {
    std::string name;
    std::string expression;
    std::string expected;
    bool should_fail;
};

class TestRunner {
public:
    void add_test(const TestCase& test) {
        tests_.push_back(test);
    }
    
    void run_all() {
        std::cout << "运行单元表达式测试套件" << std::endl;
        std::cout << "=====================" << std::endl;
        
        int passed = 0;
        int failed = 0;
        
        for (const auto& test : tests_) {
            std::cout << "\n测试: " << test.name << std::endl;
            std::cout << "表达式: " << test.expression << std::endl;
            
            try {
                std::string result = evaluate_expression(test.expression);
                
                if (test.should_fail) {
                    std::cout << "❌ 预期失败但通过了" << std::endl;
                    std::cout << "结果: " << result << std::endl;
                    failed++;
                } else {
                    if (result == test.expected) {
                        std::cout << "✅ 通过" << std::endl;
                        std::cout << "结果: " << result << std::endl;
                        passed++;
                    } else {
                        std::cout << "❌ 失败" << std::endl;
                        std::cout << "预期: " << test.expected << std::endl;
                        std::cout << "实际: " << result << std::endl;
                        failed++;
                    }
                }
            } catch (const std::exception& e) {
                if (test.should_fail) {
                    std::cout << "✅ 按预期失败: " << e.what() << std::endl;
                    passed++;
                } else {
                    std::cout << "❌ 意外失败: " << e.what() << std::endl;
                    failed++;
                }
            }
        }
        
        std::cout << "\n=====================" << std::endl;
        std::cout << "总计: " << tests_.size() << " 个测试" << std::endl;
        std::cout << "通过: " << passed << std::endl;
        std::cout << "失败: " << failed << std::endl;
        std::cout << "=====================" << std::endl;
    }
    
private:
    std::string evaluate_expression(const std::string& expr) {
        // 占位实现 - 实际将调用unit_expr库
        return "模拟结果";
    }
    
    std::vector<TestCase> tests_;
};

// 测试注册函数声明
void register_basic_tests(TestRunner& runner);
void register_unit_tests(TestRunner& runner);
void register_function_tests(TestRunner& runner);
void register_complex_tests(TestRunner& runner);
void register_error_tests(TestRunner& runner);

#endif // TEST_FRAMEWORK_HPP