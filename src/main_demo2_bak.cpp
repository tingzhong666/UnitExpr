// 假設你已經把 LLNL/units 加進專案並能編譯
#include "includes/units/units.hpp"
#include "includes/angelscript/angelscript.h"
#include <cassert>

using namespace units;

static precise_measurement precise_measurement_add(const precise_measurement& a, const precise_measurement& b)
{
    try {
        return a + b;
    } catch (...) {
        asIScriptContext* ctx = asGetActiveContext();
        if (ctx) ctx->SetException("Unit mismatch in addition");
        return precise_measurement();
    }
}

// cpp的全局函数 注册为as的某个类型的构造 必须这种格式
static void mToPm(const measurement& other, precise_measurement &a)
{
    a = other;
}

static precise_measurement precise_measurement_mul(measurement a, const measurement& b)
{
    return a + b;
}

void RegisterLLNLUnitsMinimal(asIScriptEngine *engine)
{
    int r = 0;

    r = engine->RegisterObjectType("precise_measurement", sizeof(precise_measurement), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    r = engine->RegisterObjectType("measurement", sizeof(measurement), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    r = engine->RegisterObjectType("precise_unit", sizeof(precise_unit), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    r = engine->RegisterObjectType("unit", sizeof(unit), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);

    r = engine->RegisterObjectMethod("precise_measurement", "precise_measurement opAdd(const precise_measurement &in) const",
        asFUNCTION(precise_measurement_add), asCALL_CDECL_OBJLAST);

    r = engine->RegisterObjectMethod(
        "precise_measurement", 
        "precise_measurement opMul(const precise_measurement &in) const",
        asMETHODPR(precise_measurement, operator*, (const precise_measurement&) const, precise_measurement), 
        asCALL_THISCALL);

    r = engine->RegisterObjectMethod(
        "measurement", 
        "measurement opAdd(const measurement &in) const",
        asMETHODPR(measurement, operator+, (const measurement&) const, measurement), 
        asCALL_THISCALL);

    r = engine->RegisterObjectMethod(
        "unit", 
        "measurement opMul_r(double) const",
        asFUNCTIONPR(operator*,  (double, const unit&), measurement), 
        asCALL_CDECL_OBJLAST);

    // cpp全局 as构造
    r = engine->RegisterObjectBehaviour(
        "precise_measurement", 
        asBEHAVE_CONSTRUCT,
        "void f(const measurement &in)",                     // 脚本中看到的签名
        asFUNCTION(mToPm),             // 全局函数指针
        asCALL_CDECL_OBJLAST);

    engine->RegisterGlobalProperty("const unit m", (void*)&m);
    engine->RegisterGlobalProperty("const unit cm", (void*)&cm);
    engine->RegisterGlobalProperty("const unit s", (void*)&s);
}

void MessageCallback(const asSMessageInfo *msg, void *param)
{
    const char *type = "ERR ";
    if (msg->type == asMSGTYPE_WARNING)
        type = "WARN";
    else if (msg->type == asMSGTYPE_INFORMATION)
        type = "INFO";
    printf("%s (%d; %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}
void main()
{
    auto engine = asCreateScriptEngine();
    RegisterLLNLUnitsMinimal(engine);
    engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

    asIScriptModule *mod = engine->GetModule("temp", asGM_ALWAYS_CREATE);
    mod->AddScriptSection("expr", "precise_measurement GetResult() { return 2 * m + 4 * cm; }");
    auto r = mod->Build();
    if (r < 0)
    {
        printf("编译失败\n");
        return;
    }
    asIScriptContext *ctx = engine->CreateContext();
    asIScriptFunction *func = mod->GetFunctionByName("GetResult");
    ctx->Prepare(func);
    r = ctx->Execute();
    if (r != asEXECUTION_FINISHED)
    {
        printf("执行失败: %d\n", r);
        if (r == asEXECUTION_EXCEPTION)
        {
            printf("异常: %s\n", ctx->GetExceptionString());
        }
        return;
    }
    auto resultObj = ctx->GetReturnAddress();
    if (resultObj == nullptr)
    {
        printf("返回对象为空\n");
        return;
    }
    ctx->Release();

    auto result = static_cast<precise_measurement*>(resultObj);
    printf("结果: %g\n", result->value());
    printf("结果: %s\n", to_string(result->units()));
}