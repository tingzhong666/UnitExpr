// 假設你已經把 LLNL/units 加進專案並能編譯
#include "includes/units/units.hpp"
#include "includes/angelscript/angelscript.h"
#include <cassert>

using namespace units;


struct ScriptUnitData
{
    int meters;
    int kilograms;
    int seconds;
    int amperes;
    int kelvins;
    int moles;
    int candelas;
    int currencys;
    int counts;
    int radians;
    unsigned int per_unit;
    unsigned int iflag;
    unsigned int eflag;
    unsigned int equation;

    ScriptUnitData() = default;
    ScriptUnitData(const detail::unit_data& other) :
            meters(other.meter()), seconds(other.second()), kilograms(other.kg()),
            amperes(other.ampere()), candelas(other.candela()), kelvins(other.kelvin()),
            moles(other.mole()), radians(other.radian()), currencys(other.currency()),
            counts(other.count()), per_unit(other.is_per_unit()), iflag(other.has_i_flag()), 
            eflag(other.has_e_flag()),
            equation(other.is_equation()){}
};

struct ScriptPreciseUnit
{
    double multiplier_{1.0};
    ScriptUnitData base_units_;
    std::uint32_t commodity_{0};


    ScriptPreciseUnit() = default;
    ScriptPreciseUnit(const precise_unit& other) :
        multiplier_(other.multiplier()), base_units_(other.base_units()), commodity_(other.commodity()){}

};

namespace Unit
{
    static ScriptPreciseUnit m(precise::meter);
    static ScriptPreciseUnit cm(precise::cm);
    static ScriptPreciseUnit s(precise::s);
}

struct ScriptPreciseMeasurement
{
    double value_{0.0};
    ScriptPreciseUnit units_;

    ScriptPreciseMeasurement() = default;
    ScriptPreciseMeasurement(double value, const ScriptPreciseUnit& units) :
        value_(value), units_(units)
    {
    }
    ScriptPreciseMeasurement(const precise_measurement& other) :
        value_(other.value()), units_(other.units())
    {
    }
};

detail::unit_data unitDataCreate(const ScriptUnitData &sud)
{
    return detail::unit_data(sud.meters,
                     sud.kilograms,
                     sud.seconds,
                     sud.amperes,
                     sud.kelvins,
                     sud.moles,
                     sud.candelas,
                     sud.currencys,
                     sud.counts,
                     sud.radians,
                     sud.per_unit,
                     sud.iflag,
                     sud.eflag,
                     sud.equation);
}

precise_unit preciseUnittCraete(const ScriptPreciseUnit &spu)
{
    auto ud = unitDataCreate(spu.base_units_);
    return precise_unit(spu.multiplier_, ud, spu.commodity_);
}

precise_measurement preciseMeasurementCreate(const ScriptPreciseMeasurement &spu)
{
    auto pu = preciseUnittCraete(spu.units_);
    return precise_measurement(spu.value_, pu);
}

static ScriptPreciseMeasurement precise_measurement_add(const ScriptPreciseMeasurement& a, const ScriptPreciseMeasurement& b)
{
    try {
        auto a2 = preciseMeasurementCreate(a);
        auto b2 = preciseMeasurementCreate(b);
        auto c2 = a2 + b2;
        return ScriptPreciseMeasurement(c2);
    } catch (...) {
        asIScriptContext* ctx = asGetActiveContext();
        if (ctx) ctx->SetException("Unit mismatch in addition");
        return ScriptPreciseMeasurement();
    }
}

static ScriptPreciseMeasurement precise_measurement_mul(const ScriptPreciseMeasurement& a, const ScriptPreciseMeasurement& b)
{
    auto a2 = preciseMeasurementCreate(a);
    auto b2 = preciseMeasurementCreate(b);
    auto c2 = a2 * b2;
    return ScriptPreciseMeasurement(c2);
}

static ScriptPreciseMeasurement precise_measurement_mul(double a, const ScriptPreciseUnit& b)
{
    return ScriptPreciseMeasurement(a,b);
}

void RegisterLLNLUnitsMinimal(asIScriptEngine *engine)
{
    int r = 0;

    r = engine->RegisterObjectType("ScriptUnitData", sizeof(ScriptUnitData), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    r = engine->RegisterObjectType("ScriptPreciseUnit", sizeof(ScriptPreciseUnit), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);
    r = engine->RegisterObjectType("ScriptPreciseMeasurement", sizeof(ScriptPreciseMeasurement), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDK);

    r = engine->RegisterObjectMethod("ScriptPreciseMeasurement", "ScriptPreciseMeasurement opAdd(const ScriptPreciseMeasurement &in) const",
        asFUNCTION(precise_measurement_add), asCALL_CDECL_OBJLAST);

    r = engine->RegisterObjectMethod("ScriptPreciseMeasurement", "ScriptPreciseMeasurement opMul(const ScriptPreciseMeasurement &in) const",
        asFUNCTIONPR(precise_measurement_mul, (const ScriptPreciseMeasurement&, const ScriptPreciseMeasurement&), ScriptPreciseMeasurement), 
        asCALL_CDECL_OBJLAST);

    r = engine->RegisterObjectMethod(
        "ScriptPreciseUnit", 
        "ScriptPreciseMeasurement opMul_r(double) const",
        asFUNCTIONPR(precise_measurement_mul, (double, const ScriptPreciseUnit&), ScriptPreciseMeasurement), 
        asCALL_CDECL_OBJLAST);

    // 这里不能用labmda
    // r = engine->RegisterGlobalFunction("precise_measurement meter(double)"; asFUNCTION([](double v){ return precise_measurement(v; m); }); asCALL_CDECL);
    // r = engine->RegisterGlobalFunction("precise_measurement second(double)"; asFUNCTION([](double v){ return precise_measurement(v; s); }); asCALL_CDECL);
    // r = engine->RegisterGlobalFunction("precise_measurement kilogram(double)"; asFUNCTION([](double v){ return precise_measurement(v; kg); }); asCALL_CDECL);

    engine->RegisterGlobalProperty("const ScriptPreciseUnit m", (void*)&Unit::m);
    engine->RegisterGlobalProperty("const ScriptPreciseUnit cm", (void*)&Unit::cm);
    engine->RegisterGlobalProperty("const ScriptPreciseUnit s", (void*)&Unit::s);

    auto qwe = 123 * units::m;
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
    mod->AddScriptSection("expr", "ScriptPreciseMeasurement GetResult() { return 2 * m + 2 * s; }");
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