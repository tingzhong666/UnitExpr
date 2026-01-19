#include "test_framework.hpp"

void register_complex_tests(TestRunner& runner) {
    std::cout << "注册组合表达式测试..." << std::endl;
    
    // 1. 物理公式测试
    runner.add_test({"速度公式", "10m / 2s", "5m/s", false});
    runner.add_test({"加速度公式", "(20m/s - 10m/s) / 5s", "2m/s^2", false});
    runner.add_test({"动能公式", "0.5 * 2kg * (5m/s)^2", "25kg*m^2/s^2", false});
    
    // 2. 工程公式测试
    runner.add_test({"电阻公式", "10V / 2A", "5ohm", false});
    runner.add_test({"功率公式", "10V * 2A", "20W", false});
    runner.add_test({"能量公式", "20W * 3600s", "72000J", false});
    
    // 3. 复杂数学表达式
    runner.add_test({"三角函数组合", "sin(30deg)^2 + cos(30deg)^2", "1", false});
    runner.add_test({"指数对数组合", "ln(exp(2)) + log10(1000)", "5", false});
    runner.add_test({"嵌套函数", "sqrt(sin(90deg)^2 + cos(90deg)^2)", "1", false});
    
    // 4. 实际应用场景
    runner.add_test({"运动学问题", "10m/s * 5s + 0.5 * 2m/s^2 * (5s)^2", "75m", false});
    runner.add_test({"理想气体定律", "101325Pa * 0.0224m^3 / (8.314J/(mol*K) * 273.15K)", "1mol", false});
    runner.add_test({"货币转换", "100USD * 0.85EUR/USD", "85EUR", false});
}