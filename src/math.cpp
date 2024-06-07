/* math.h - SIMD/SSE mathematics routines
 *
 * NVGT - NonVisual Gaming Toolkit
 * Copyright (c) 2022-2024 Sam Tupy
 * https://nvgt.gg
 * This software is provided "as-is", without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#include "math.h"
#include <sleef.h>

void RegisterSSEMath(asIScriptEngine *engine) {
engine->RegisterGlobalFunction("double sin(const double) const", asFUNCTION(Sleef_sin_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double cos(const double) const", asFUNCTION(Sleef_cos_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double sinpi(const double) const", asFUNCTION(Sleef_sinpi_u05), asCALL_CDECL);
engine->RegisterGlobalFunction("double tan(const double)", asFUNCTION(Sleef_tan_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double pow(const double, const double)", asFUNCTION(Sleef_pow_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double log(const double)", asFUNCTION(Sleef_log_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double log10(double)", asFUNCTION(Sleef_log10_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double log2(const double)", asFUNCTION(Sleef_log2_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double log1p(const double)", asFUNCTION(Sleef_log1p_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double exp(const double)", asFUNCTION(Sleef_exp_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double exp2(const double)", asFUNCTION(Sleef_exp2_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double exp10(const double)", asFUNCTION(Sleef_exp10_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double expm1(const double)", asFUNCTION(Sleef_expm1_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double sqrt(const double)", asFUNCTION(Sleef_sqrt_u05), asCALL_CDECL);
engine->RegisterGlobalFunction("double cbrt(const double)", asFUNCTION(Sleef_cbrt_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double hypot(const double, const double)", asFUNCTION(Sleef_hypot_u05), asCALL_CDECL);
engine->RegisterGlobalFunction("double asin(const double)", asFUNCTION(Sleef_asin_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double acos(const double)", asFUNCTION(Sleef_acos_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double atan(const double)", asFUNCTION(Sleef_atan_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double atan2(const double, const double)", asFUNCTION(Sleef_atan2_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double sinh(const double)", asFUNCTION(Sleef_sinh_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double cosh(const double)", asFUNCTION(Sleef_cosh_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double tanh(const double)", asFUNCTION(Sleef_tanh_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double asinh(const double)", asFUNCTION(Sleef_asinh_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double acosh(const double)", asFUNCTION(Sleef_acosh_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double atanh(const double)", asFUNCTION(Sleef_atanh_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double erf(const double)", asFUNCTION(Sleef_erf_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double erfc(const double)", asFUNCTION(Sleef_erfc_u15), asCALL_CDECL);
engine->RegisterGlobalFunction("double tgamma(const double)", asFUNCTION(Sleef_tgamma_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double lgamma(const double)", asFUNCTION(Sleef_lgamma_u10), asCALL_CDECL);
engine->RegisterGlobalFunction("double trunc(const double)", asFUNCTION(Sleef_trunc), asCALL_CDECL);
engine->RegisterGlobalFunction("double floor(const double)", asFUNCTION(Sleef_floor), asCALL_CDECL);
engine->RegisterGlobalFunction("double ceil(const double)", asFUNCTION(Sleef_ceil), asCALL_CDECL);
engine->RegisterGlobalFunction("double round(const double)", asFUNCTION(Sleef_round), asCALL_CDECL);
engine->RegisterGlobalFunction("double rint(const double)", asFUNCTION(Sleef_rint), asCALL_CDECL);
engine->RegisterGlobalFunction("double fma(const double, const double, const double)", asFUNCTION(Sleef_fma), asCALL_CDECL);
engine->RegisterGlobalFunction("double fmod(const double, const double)", asFUNCTION(Sleef_fmod), asCALL_CDECL);
engine->RegisterGlobalFunction("double remainder(const double, const double)", asFUNCTION(Sleef_remainder), asCALL_CDECL);
engine->RegisterGlobalFunction("double ldexp(const double, const int)", asFUNCTION(Sleef_ldexp), asCALL_CDECL);
engine->RegisterGlobalFunction("double frfrexp(const double)", asFUNCTION(Sleef_frfrexp), asCALL_CDECL);
engine->RegisterGlobalFunction("int expfrexp(const double)", asFUNCTION(Sleef_expfrexp), asCALL_CDECL);
engine->RegisterGlobalFunction("int ilogb(const double, const int)", asFUNCTION(Sleef_ilogb), asCALL_CDECL);
engine->RegisterGlobalFunction("double abs(const double)", asFUNCTION(Sleef_fabs), asCALL_CDECL);
engine->RegisterGlobalFunction("double max(const double, const double)", asFUNCTION(Sleef_fmax), asCALL_CDECL);
engine->RegisterGlobalFunction("double min(const double, const double)", asFUNCTION(Sleef_fmin), asCALL_CDECL);
engine->RegisterGlobalFunction("double dim(const double, const double)", asFUNCTION(Sleef_fdim), asCALL_CDECL);
engine->RegisterGlobalFunction("double copysign(const double, const double)", asFUNCTION(Sleef_copysign), asCALL_CDECL);
engine->RegisterGlobalFunction("double nextafter(const double, const double)", asFUNCTION(Sleef_nextafter), asCALL_CDECL);
}
