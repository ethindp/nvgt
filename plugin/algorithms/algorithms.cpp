#include "../../src/nvgt_plugin.h"
#include "scriptarray.h"
#include <cstdint>
#include <cassert>

enum class ExecutionPolicy {
seq,
par,
unseq,
par_unseq
};

void for_each(CScriptArray* arr, asIScriptFunction* func, ExecutionPolicy policy = ExecutionPolicy::seq) {
if (!arr || !func || arr->IsEmpty()) return;
switch (policy) {
case ExecutionPolicy::seq: {
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
}
break;
case ExecutionPolicy::par: {
#pragma omp parallel for
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
}
break;
case ExecutionPolicy::unseq: {
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
}
break;
case ExecutionPolicy::par_unseq: {
#pragma omp parallel
{
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
}
}
break;
}
}

void for_each_n(CScriptArray* arr, asIScriptFunction* func, std::size_t n, ExecutionPolicy policy = ExecutionPolicy::seq) {
if (!arr || !func || arr->IsEmpty() || n >= arr->GetSize()) return;
switch (policy) {
case ExecutionPolicy::seq: {
for (std::int64_t i = 0; i < n; ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
}
break;
case ExecutionPolicy::par: {
#pragma omp parallel for
for (std::int64_t i = 0; i < n; ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
}
break;
case ExecutionPolicy::unseq: {
#pragma omp simd
for (std::int64_t i = 0; i < n; ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
}
break;
case ExecutionPolicy::par_unseq: {
#pragma omp parallel
{
#pragma omp simd
for (std::int64_t i = 0; i < n; ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(func);
ctx->SetArgObject(0, &element);
ctx->Execute();
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
}
}
break;
}
}

bool all_of(CScriptArray* arr, asIScriptFunction* pred, ExecutionPolicy policy = ExecutionPolicy::seq) {
if (!arr || !pred || arr->IsEmpty()) return false;
switch (policy) {
case ExecutionPolicy::seq: {
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
return false;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
}
break;
case ExecutionPolicy::par: {
bool ret = true;
#pragma omp parallel for
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = false;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
return ret;
}
break;
case ExecutionPolicy::unseq: {
bool ret = true;
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = false;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
return ret;
}
break;
case ExecutionPolicy::par_unseq: {
bool ret = true;
#pragma omp parallel
{
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = false;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
}
return ret;
}
break;
}
return true;
}

bool none_of(CScriptArray* arr, asIScriptFunction* pred, ExecutionPolicy policy = ExecutionPolicy::seq) {
if (!arr || !pred || arr->IsEmpty()) return false;
switch (policy) {
case ExecutionPolicy::seq: {
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
return false;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
}
break;
case ExecutionPolicy::par: {
bool ret = true;
#pragma omp parallel for
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = false;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
return ret;
}
break;
case ExecutionPolicy::unseq: {
bool ret = true;
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = false;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
return ret;
}
break;
case ExecutionPolicy::par_unseq: {
bool ret = true;
#pragma omp parallel
{
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = false;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
}
return ret;
}
break;
}
return true;
}

bool any_of(CScriptArray* arr, asIScriptFunction* pred, ExecutionPolicy policy = ExecutionPolicy::seq) {
if (!arr || !pred || arr->IsEmpty()) return false;
switch (policy) {
case ExecutionPolicy::seq: {
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
return true;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
}
break;
case ExecutionPolicy::par: {
bool ret = false;
#pragma omp parallel for
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = true;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
return ret;
}
break;
case ExecutionPolicy::unseq: {
bool ret = false;
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
auto element = arr->At(i);
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = true;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
}
return ret;
}
break;
case ExecutionPolicy::par_unseq: {
bool ret = false;
#pragma omp parallel
{
#pragma omp simd
for (std::int64_t i = 0; i < arr->GetSize(); ++i) {
const auto ctx = asGetActiveContext()->GetEngine()->RequestContext();
asAcquireSharedLock();
auto element = arr->At(i);
asReleaseSharedLock();
ctx->Prepare(pred);
ctx->SetArgObject(0, &element);
ctx->Execute();
if (ctx->GetReturnByte()) {
ret = true;
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
break;
}
asGetActiveContext()->GetEngine()->ReturnContext(ctx);
asThreadCleanup();
}
}
return ret;
}
break;
}
return false;
}

plugin_main(nvgt_plugin_shared* shared) {
	prepare_plugin(shared);
	shared->script_engine->BeginConfigGroup("algorithms");
	shared->script_engine->SetDefaultNamespace("std");
	shared->script_engine->RegisterEnum("execution_policy");
	shared->script_engine->RegisterEnumValue("execution_policy", "seq", static_cast<int>(ExecutionPolicy::seq));
	shared->script_engine->RegisterEnumValue("execution_policy", "par", static_cast<int>(ExecutionPolicy::par));
	shared->script_engine->RegisterEnumValue("execution_policy", "unseq", static_cast<int>(ExecutionPolicy::unseq));
	shared->script_engine->RegisterEnumValue("execution_policy", "par_unseq", static_cast<int>(ExecutionPolicy::par_unseq));
	shared->script_engine->SetDefaultNamespace("");
	int r = 0;
	r = shared->script_engine->RegisterObjectMethod("array<T>", "void for_each(? &in, const int)", asFUNCTION(for_each), asCALL_CDECL_OBJFIRST); assert(r != 0);
	r = shared->script_engine->RegisterObjectMethod("array<T>", "void for_each_n(? &in, const int64, const int)", asFUNCTION(for_each_n), asCALL_CDECL_OBJFIRST); assert(r != 0);
	r = shared->script_engine->RegisterObjectMethod("array<T>", "bool any_of(? &in, const int)", asFUNCTION(any_of), asCALL_CDECL_OBJFIRST); assert(r != 0);
	r = shared->script_engine->RegisterObjectMethod("array<T>", "bool none_of(? &in, const int)", asFUNCTION(none_of), asCALL_CDECL_OBJFIRST); assert(r != 0);
	r = shared->script_engine->RegisterObjectMethod("array<T>", "bool all_of(? &in, const int)", asFUNCTION(all_of), asCALL_CDECL_OBJFIRST); assert(r != 0);
	shared->script_engine->EndConfigGroup();
	return true;
}
