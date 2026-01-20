// 假設你已經把 LLNL/units 加進專案並能編譯
#include "includes/units/units.hpp"
#include "includes/angelscript/angelscript.h"
#include <cassert>

using namespace units;


// 簡單的 to_string 包裝（因為 AS 喜歡 std::string）
static std::string precise_measurement_to_string(const precise_measurement& m)
{
    return to_string(m);   // LLNL 內建支援
}

// 加法安全包裝（LLNL 會在單位不符時 throw，這裡轉成 AS 異常）
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

// 同理可做減、乘、除 … 這裡只示範加法與乘法
static precise_measurement precise_measurement_mul(const precise_measurement& a, const precise_measurement& b)
{
    return a * b;   // 乘法通常不會 throw
}

// 註冊函數（在你的引擎初始化處呼叫）
void RegisterLLNLUnitsMinimal(asIScriptEngine* engine)
{
    // int r = 0;

    // // 註冊類型（值類型，POD 大小固定）
    // r = engine->RegisterObjectType("precise_measurement", sizeof(precise_measurement), asOBJ_VALUE | asOBJ_POD);
    // assert(r >= 0);


    // // 運算子（最常用兩種）
    // r = engine->RegisterObjectMethod("precise_measurement", "precise_measurement opAdd(const precise_measurement &in) const",
    //     asFUNCTION(precise_measurement_add), asCALL_CDECL_OBJLAST);

    // r = engine->RegisterObjectMethod("precise_measurement", "precise_measurement opMul(const precise_measurement &in) const",
    //     asFUNCTION(precise_measurement_mul), asCALL_CDECL_OBJLAST);

    // // 輸出（很重要）
    // r = engine->RegisterObjectMethod("precise_measurement", "string toString() const",
    //     asFUNCTION(precise_measurement_to_string), asCALL_CDECL_OBJLAST);

    // // 幾個常用單位（全局函數） 这里不能用labmda
    // // r = engine->RegisterGlobalFunction("precise_measurement meter(double)", asFUNCTION([](double v){ return precise_measurement(v, m); }), asCALL_CDECL);
    // // r = engine->RegisterGlobalFunction("precise_measurement second(double)", asFUNCTION([](double v){ return precise_measurement(v, s); }), asCALL_CDECL);
    // // r = engine->RegisterGlobalFunction("precise_measurement kilogram(double)", asFUNCTION([](double v){ return precise_measurement(v, kg); }), asCALL_CDECL);

    // engine->RegisterGlobalProperty("const precise_measurement m", (void*)&units::m);
}

void main()
{
    auto engine = asCreateScriptEngine();
    //RegisterLLNLUnitsMinimal(engine);

    asIScriptModule *mod = engine->GetModule("temp", asGM_ALWAYS_CREATE);
    mod->AddScriptSection("expr", "double GetResult() { return 1 + 2; }");
    mod->Build();
    asIScriptContext *ctx = engine->CreateContext();
    asIScriptFunction *func = mod->GetFunctionByName("GetResult");
    ctx->Prepare(func);
    ctx->Execute();
    auto result = ctx->GetReturnDouble();
    ctx->Release();

    //auto redD = *(double*)result;
    printf("结果: %g\n", result);

}