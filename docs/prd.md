## 编译环境：
- cmake工程
- msvc2022_amd64编译器
- cmake路径`C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`

## 技术栈：
- cpp14
- exprtk
- LLNL/Units


## 测试：
60个

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


功能：
- 字符串表达式
- 支持量纲
- 支持运算符注册
- 支持函数注册
- 支持括号
- 支持无限嵌套
- 支持量纲计算+数值计算

