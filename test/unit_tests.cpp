#include "test_framework.hpp"

void register_unit_tests(TestRunner& runner) {
    std::cout << "注册单位运算测试..." << std::endl;
    
    // 1. 基本单位运算
    runner.add_test({"长度加法", "2m + 3m", "5m", false});
    runner.add_test({"长度减法", "5m - 3m", "2m", false});
    runner.add_test({"带单位乘法", "2m * 3", "6m", false});
    runner.add_test({"带单位除法", "6m / 2", "3m", false});
    
    // 2. 单位转换
    runner.add_test({"米到千米", "1000m to km", "1km", false});
    runner.add_test({"小时到秒", "1h to s", "3600s", false});
    runner.add_test({"千米每小时到米每秒", "36km/h to m/s", "10m/s", false});
    
    // 3. 复合单位运算
    runner.add_test({"速度计算", "10m / 2s", "5m/s", false});
    runner.add_test({"面积计算", "2m * 3m", "6m^2", false});
    runner.add_test({"体积计算", "2m * 3m * 4m", "24m^3", false});
    
    // 4. 单位抵消
    runner.add_test({"单位抵消1", "10m / 2m", "5", false});
    runner.add_test({"单位抵消2", "(5m/s) * 2s", "10m", false});
    runner.add_test({"复杂单位抵消", "(10kg * 5m/s^2) / 2kg", "25m/s^2", false});
}