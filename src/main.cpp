#include "includes/units/units.hpp"
#include "includes/units/units_math.hpp"
#include "includes/angelscript/angelscript.h"
#include <cassert>

using namespace units;

// cpp的全局函数 注册为as的某个类型的构造 必须这种格式
void mToPm(const measurement& other, precise_measurement &a)
{
    a = other;
}

double sin(const measurement& a)
{
    return units::sin(a);
}


int _asRegister(asIScriptEngine *engine)
{
    int r = 0;

    r = engine->RegisterObjectType("precise_measurement", sizeof(precise_measurement), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    if(r < 0) return r;
    r = engine->RegisterObjectType("measurement", sizeof(measurement), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    if(r < 0) return r;
    r = engine->RegisterObjectType("precise_unit", sizeof(precise_unit), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    r = engine->RegisterObjectType("unit", sizeof(unit), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    if(r < 0) return r;

    // =========== 运算符
    // cpp成员 as成员
    r = engine->RegisterObjectMethod(
        "precise_measurement", 
        "precise_measurement opMul(const precise_measurement &in) const",
        asMETHODPR(precise_measurement, operator*, (const precise_measurement&) const, precise_measurement), 
        asCALL_THISCALL);
    if(r < 0) return r;

    r = engine->RegisterObjectMethod(
        "measurement", 
        "measurement opAdd(const measurement &in) const",
        asMETHODPR(measurement, operator+, (const measurement&) const, measurement), 
        asCALL_THISCALL);
    if(r < 0) return r;

    // cpp全局函数 as成员
    r = engine->RegisterObjectMethod(
        "unit", 
        "measurement opMul_r(double) const",
        asFUNCTIONPR(operator*,  (double, const unit&), measurement), 
        asCALL_CDECL_OBJLAST);
    if(r < 0) return r;
    // =========== as全局
    r = engine->RegisterGlobalFunction(
        "double sin(const measurement &in)",
        asFUNCTIONPR(sin, (const measurement&), double),
        asCALL_CDECL
    );
    // =========== 

    // cpp全局 as构造
    r = engine->RegisterObjectBehaviour(
        "precise_measurement", 
        asBEHAVE_CONSTRUCT,
        "void f(const measurement &in)",                     // 脚本中看到的签名
        asFUNCTION(mToPm),             // 全局函数指针
        asCALL_CDECL_OBJLAST);
    if(r < 0) return r;

    // =========== as全局变量
    // 单位
    r = engine->RegisterGlobalProperty("const unit m", (void*)&m);
    if(r < 0) return r;
    r = engine->RegisterGlobalProperty("const unit cm", (void*)&cm);
    if(r < 0) return r;
    r = engine->RegisterGlobalProperty("const unit deg", (void*)&deg);
    if(r < 0) return r;
    r = engine->RegisterGlobalProperty("const unit s", (void*)&s);

    return r;
}

asERetCodes asRegister(asIScriptEngine *engine)
{
    auto r = _asRegister(engine);
    if(r < 0) return (asERetCodes)_asRegister(engine);
    else return asERetCodes::asSUCCESS;
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
    auto status = asRegister(engine);
    if(status < 0)
    {
        printf("注册失败 %d\n", status);
        return;
    }
    engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

    asIScriptModule *mod = engine->GetModule("temp", asGM_ALWAYS_CREATE);
    mod->AddScriptSection("expr", "precise_measurement GetResult() { return 2 * m + sin(60*deg)*cm; }");
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
    printf("结果: %s\n", to_string(result->units()).c_str());
}