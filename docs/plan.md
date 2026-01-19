# 更新后的单位表达式库（UnitExpr）设计与开发计划（修订版）

感谢您的反馈。首先澄清："无单位函数"是我在风险与备选部分提到的简化备选方案（仅作为如果集成复杂时的后备），但我不会擅自去掉任何功能——计划将完全支持自定义函数中的嵌套带量纲表达式（e.g., custom_func(2m + sin(3s))，处理单位兼容/运算）。嵌套深度无限，不设置任何限制（依赖ExprTk内置能力）。变量不支持数组（仅简单UnitValue值）。以下是修订后的计划，整合这些点，保持初级开发者友好、自包含。

## 1. 项目概述（修订）
- **目标**：C++14库，支持ExprTk表达式解析 + LLNL/Units单位处理。变量：外部注入，支持带单位值。自定义函数：支持注入，内部可嵌套带量纲表达式（e.g., 函数体处理UnitValue输入/输出，确保单位运算）。
- **范围修订**：函数嵌套/括号/自定义扩展全支持带单位；无嵌套限制；变量无数组支持。
- **假设**：自定义函数输入/输出为UnitValue，支持单位检查（e.g., 如果函数如"force = mass * accel"，自动处理kg*m/s^2）。如果嵌套过深导致性能问题，ExprTk会自然处理（无人工限制）。
- **权衡**：无限嵌套提供灵活性，但可能导致复杂错误（e.g., 深层单位不匹配）——我们将在ErrorHandler中捕获。您对错误报告的详细程度有偏好（如简短消息 vs. 栈追踪）？

## 2. 依赖准备（只读步骤，无修改）
- **步骤1**：验证现有结构（使用read-only工具分析）。
  - Glob test/**.cpp确认测试覆盖嵌套/函数（e.g., complex_tests.cpp有"嵌套函数"，可扩展验证自定义/带单位嵌套）。
  - Grep搜索"function"或"nested"分析当前支持（无修改，仅观察）。
- **步骤2**：文档分析（webfetch如果需要）。
  - Fetch ExprTk docs（url: https://github.com/ArashPartow/exprtk/blob/master/readme.txt）确认自定义函数支持多参数/嵌套（e.g., add_function允许lambda处理UnitValue向量）。
  - Units docs：验证带量纲运算在嵌套中的兼容性。
- **注意**：纯观察；无下载/修改。如果依赖缺失，询问用户路径。

## 3. 项目结构设计（规划，无创建）
- **目录布局**（实施时手动创建）：
  - 与之前相同，新测试文件test/variable_injection_tests.cpp验证注入；可选docs/custom_functions.md（规划自定义示例，无写）。
- **核心组件修订**：
  - **UnitValue**：不变，支持嵌套运算/单位传播。
  - **ExprEvaluator**：evaluate(const std::string& expr, const std::map<std::string, UnitValue>& vars = {}, const std::map<std::string, CustomFuncPtr>& custom_funcs = {})。
    - 注入custom_funcs到symbol_table，支持内部嵌套带量纲（e.g., func解析子表达式时自动处理UnitValue）。
  - **ErrorHandler**：捕获嵌套单位错误（如不兼容）+自定义函数异常，无深度限制。
  - **新组件**：CustomFuncPtr typedef为UnitValue(*)(const std::vector<UnitValue>&)，确保函数处理带单位输入/嵌套。

## 4. 开发步骤（详细规划，待用户确认执行）
- **步骤1：设置构建系统**（规划CMakeLists.txt内容，无写）。
  - 与之前类似，添加测试支持自定义函数注入（e.g., add_test_with_funcs）。
- **步骤2：实现核心类**（伪代码规划）。
  - include/unit_expr.h：
    ```
    using CustomFuncPtr = UnitValue(*)(const std::vector<UnitValue>&);  // 支持多参、带单位嵌套
    class ExprEvaluator {
      std::pair<std::string, bool> evaluate(const std::string& expr, const std::map<std::string, UnitValue>& vars = {}, const std::map<std::string, CustomFuncPtr>& custom_funcs = {});
    };
    ```
  - src/unit_expr.cpp：evaluate逻辑。
    1. 创建symbol_table<UnitValue> table。
    2. 注册内置单位/函数。
    3. 注入vars和custom_funcs（table.add_function(key, func_ptr)）。
    4. exprtk::expression<UnitValue> expression(table);  // 内置无限嵌套支持
    5. expression.compile(expr);  // 处理括号/函数嵌套，包括带单位子表达式
    6. UnitValue result = expression.value();  // 传播单位
    7. 格式化（round to 1e-3，append unit）。
    8. catch返回错误（如"Unit mismatch in nested expression"）。
- **步骤3：扩展功能**。
  - 函数嵌套/括号：ExprTk内置，支持无限带单位嵌套（e.g., "sin((2m / 1s) ^ 2)" -> 处理m/s单位）。
  - 自定义函数：注入示例custom_funcs={{"velocity", [](const std::vector<UnitValue>& args){ return args[0] / args[1]; }}}（支持distance/time -> speed单位）。
  - 变量注入：支持带单位，无数组（e.g., "x + 2m" with vars={{"x",{3,units::meter}}}）。
  - 精度：1e-3阈值。
- **步骤4：集成测试**。
  - 扩展test_framework.hpp：add_test_with_vars_and_funcs（支持vars + custom_funcs）。
  - 新文件test/variable_injection_tests.cpp：添加注入测试。
  - 扩展function_tests/complex_tests：验证嵌套/自定义（e.g., "custom_func(2m + sin(30deg))" with injected func）。
  - 运行规划：构建后./tests，验证通过率。
- **步骤5：调试与优化**。
  - 聚焦带单位嵌套错误、自定义函数单位传播。
  - 使用日志观察（无修改）。

## 5. 时间估计与里程碑（修订）
- 准备（1小时）：分析嵌套/自定义。
- 核心实现（4小时）：确保自定义支持带单位嵌套。
- 功能扩展（3小时）：测试无限嵌套。
- 测试（3小时）：新API/文件验证。
- 总计：11小时。

## 6. 风险与备选
- 风险：深层嵌套性能（无限设计下可能慢） -> 监控测试时间，无限制如要求。
- 备选：如果自定义单位传播复杂，提供调试钩子（非简化功能）。
- 澄清问题：具体自定义函数示例（e.g., 物理如"gravity"或数学如"integrate"）？错误报告偏好？计划现在合适吗，是否准备进入实施阶段（切换出plan mode）？