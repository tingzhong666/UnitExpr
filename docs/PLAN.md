# exprtk + LLNL/Units 量纲计算器开发计划

## 核心目标
构建 C++14 量纲表达式计算器，支持运行时用户输入解析+计算。

## 技术栈
- **解析**: exprtk
- **量纲**: LLNL/Units
- **测试**: 60+ 测试用例 (main.cpp 调用)

## 文件结构
```
src/
├── dim_expr.hpp     # 核心类
├── unit_parser.hpp  # 单位解析
└── main.cpp         # 60+ 测试

CMakeLists.txt
```

## 测试分类 (60个)
### 基础 (36个)
1-8: 数值/常数

9-16: 运算符

17-28: 内置函数

29-36: 变量

### 组合测试 (24个)
37: sin(30deg)*2m/s + cos(45deg)*km/h

38: (x+1)*y*m^2/s^2 (x=2kg,y=3)

39: 2*pi*r*v (r=1m,v=10m/s)

40: F/m*a (F=9.8N,m=1kg,a=2m/s^2)

41: sin(PI/6)^2 + cos(PI/6)^2

42: log(exp(3))*sqrt(16)

43: (2+3)*(4-1)/2^2

44: max(1m/s,10km/h) + min(5kg,2g)

45: x*y/z → 单位自动抵消

46: 理想气体 PV=nRT 完整

47-60: 错误测试 (单位不匹配等)

## 实施顺序
1. 下载 LLNL/Units → libs/units/
2. CMakeLists.txt
3. unit_parser.hpp (m/s → quantity)
4. dim_expr.hpp (exprtk包装)
5. main.cpp (测试)
6. 验证运行

## 关键创新
- 正则解析复合单位 (km/h → km/h)
- 自动单位转换/检查
- exprtk 回调注册单位函数

## 状态: 已保存，不执行
- exprtk ✓ 可用
- LLNL/Units ❌ 需下载
- C++14 环境 ✓