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
        return precise_measurement();   // 或其他標記值
    }
}

static precise_measurement precise_measurement_mul(const precise_measurement& a, const precise_measurement& b)
{
    return a * b;   // 乘法通常不會 throw
}

void RegisterLLNLUnitsMinimal(asIScriptEngine* engine)
{
    int r = 0;

    r = engine->RegisterObjectType("precise_measurement", sizeof(precise_measurement), asOBJ_VALUE | asOBJ_POD);
    r = engine->RegisterObjectType("precise_unit", sizeof(precise_unit), asOBJ_VALUE | asOBJ_POD);


    r = engine->RegisterObjectMethod("precise_measurement", "precise_measurement opAdd(const precise_measurement &in) const",
        asFUNCTION(precise_measurement_add), asCALL_CDECL_OBJLAST);

    r = engine->RegisterObjectMethod("precise_measurement", "precise_measurement opMul(const precise_measurement &in) const",
        asFUNCTION(precise_measurement_mul), asCALL_CDECL_OBJLAST);

    // 这里不能用labmda
    // r = engine->RegisterGlobalFunction("precise_measurement meter(double)", asFUNCTION([](double v){ return precise_measurement(v, m); }), asCALL_CDECL);
    // r = engine->RegisterGlobalFunction("precise_measurement second(double)", asFUNCTION([](double v){ return precise_measurement(v, s); }), asCALL_CDECL);
    // r = engine->RegisterGlobalFunction("precise_measurement kilogram(double)", asFUNCTION([](double v){ return precise_measurement(v, kg); }), asCALL_CDECL);

    engine->RegisterGlobalProperty("const precise_unit m", (void*)&units::m);
}
void MessageCallback(const asSMessageInfo *msg, void *param) {
    const char *type = "ERR ";
    if (msg->type == asMSGTYPE_WARNING) type = "WARN";
    else if (msg->type == asMSGTYPE_INFORMATION) type = "INFO";
    printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}
void main()
{
    auto engine = asCreateScriptEngine();
    RegisterLLNLUnitsMinimal(engine);
    engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

    asIScriptModule *mod = engine->GetModule("temp", asGM_ALWAYS_CREATE);
    mod->AddScriptSection("expr", "precise_measurement GetResult() { return 1 * m + 2 * m; }");
    auto r = mod->Build();
    if (r < 0) {
        printf("编译失败\n");
        return;
    }
    asIScriptContext *ctx = engine->CreateContext();
    asIScriptFunction *func = mod->GetFunctionByName("GetResult");
    ctx->Prepare(func);
    r = ctx->Execute();
    if (r != asEXECUTION_FINISHED) {
        printf("执行失败: %d\n", r);
        if (r == asEXECUTION_EXCEPTION) {
            printf("异常: %s\n", ctx->GetExceptionString());
        }
        return;
    }
    auto resultObj = ctx->GetReturnAddress();
    if (resultObj == nullptr) {
        printf("返回对象为空\n");
        return;
    }
    ctx->Release();

    auto result = static_cast<precise_measurement*>(resultObj);
    printf("结果: %g\n", result->value());
}