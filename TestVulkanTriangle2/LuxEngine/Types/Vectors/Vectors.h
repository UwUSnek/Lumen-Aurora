#pragma once


#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "LuxEngine/Types/Integers/Integers.h"
#include "LuxEngine/macros.h"
#include "LuxEngine/Math/Algebra/Algebra.h"

#include "math.h"                                // for pow, sqrt
//#include "corecrt_math.h"                       // for pow, sqrt
#pragma warning( disable: 26451 )
#pragma warning( disable: 4003 )	//No enough arguments for macro
//type: the type of the function and the vectors
//typen: the type but with the capital letter
//pass: & to pass by reference vectors larger than 3 byte for better performance
#define __lp_vec_def(type, typen, pass)																																						\
typedef _vec2<type> v2##typen, vec2##typen;																																					\
typedef _vec3<type> v3##typen, vec3##typen;																																					\
typedef _vec4<type> v4##typen, vec4##typen;																																					\
\
static inline type __vectorcall dist(const vec2##typen##pass a, const vec2##typen##pass b) { return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2))); }										\
static inline type __vectorcall dist(const vec3##typen##pass a, const vec3##typen##pass b) { return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2))); }					\
static inline type __vectorcall dist(const vec4##typen& a, const vec4##typen& b) { return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2) + pow(b.w - a.w, 2))); }			\
\
static inline vec2##typen __vectorcall signNz(const vec2##typen##pass v) { return vec2##typen{signNz(v.x), signNz(v.y)}; }										\
static inline vec3##typen __vectorcall signNz(const vec3##typen##pass v) { return vec3##typen{signNz(v.x), signNz(v.y), signNz(v.z)}; }					\
static inline vec4##typen __vectorcall signNz(const vec4##typen& v) { return vec4##typen{signNz(v.x), signNz(v.y), signNz(v.z), signNz(v.w)}; }\
\
static inline vec2##typen __vectorcall sign(const vec2##typen##pass v) { return vec2##typen{sign(v.x), sign(v.y)}; }										\
static inline vec3##typen __vectorcall sign(const vec3##typen##pass v) { return vec3##typen{sign(v.x), sign(v.y), sign(v.z)}; }					\
static inline vec4##typen __vectorcall sign(const vec4##typen& v) { return vec4##typen{sign(v.x), sign(v.y), sign(v.z), sign(v.w)}; }\
\
static inline vec2##typen __vectorcall abs(const vec2##typen##pass v) { return vec2##typen{abs(v.x), abs(v.y)}; }										\
static inline vec3##typen __vectorcall abs(const vec3##typen##pass v) { return vec3##typen{abs(v.x), abs(v.y), abs(v.z)}; }					\
static inline vec4##typen __vectorcall abs(const vec4##typen& v) { return vec4##typen{abs(v.x), abs(v.y), abs(v.z), abs(v.w)}; }\
//static inline type __vectorcall length(const vec2##typen##pass v) { return scast<type>(sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2))); }														\
//static inline type __vectorcall length(const vec3##typen##pass v) { return scast<type>(sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2))); }									\
//static inline type __vectorcall length(const vec4##typen& v) { return scast<type>(sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2) + pow(b.w - a.w, 2))); }

__lp_vec_def(int8, i8)			__lp_vec_def(uint8, u8)			__lp_vec_def(bool, b)				__lp_vec_def(char8, c8)
__lp_vec_def(int16, i16, &)		__lp_vec_def(uint16, u16, &)										__lp_vec_def(char16, c16, &)
__lp_vec_def(int32, i32, &)		__lp_vec_def(uint32, u32, &)	__lp_vec_def(float32, f32, &)		__lp_vec_def(char32, c32, &)
__lp_vec_def(int64, i64, &)		__lp_vec_def(uint64, u64, &)	__lp_vec_def(float64, f64, &)
#undef __lp_vec_def
#pragma warning( default: 26451 )
#pragma warning( default: 4003 )	//No enough arguments for macro
