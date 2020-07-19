#pragma once

template<class t>
struct vec3_t {
	t x = 0;
	t y = 0;
	t z = 0;


	//Constructors
	inline vec3_t( ) {}
	inline vec3_t(const t& _x, const t& _y, const t& _z) { x = _x; y = _y; z = _z; }
	template<class ta, class tb, class tc> inline vec3_t(const ta& _x, const tb& _y, const tc& _z) { x = (t)_x; y = (t)_y; z = (t)_z; }


	//Assignment operators and constructors
	/**/			   inline vec3_t(const vec3_t<t>& v) { *this = v; }
	/**/			   inline vec3_t(const t& v) { *this = v; }
	/**/			   inline void __vectorcall operator = (const vec3_t<t>& v) { x = v.x; y = v.y; z = v.z; }
	/**/			   inline void __vectorcall operator = (const t& n) { x = y = z = n; }
	template<class vt> inline vec3_t(const vec3_t<vt>& v) { *this = v; }
	template<class vt> inline vec3_t(const vt& v) { *this = v; }
	template<class vt> inline void __vectorcall operator = (const vec3_t<vt>& v) { x = (t)(v.x); y = (t)(v.y); z = (t)(v.z); }
	template<class vt> inline void __vectorcall operator = (const vt& n) { x = y = z = (t)n; }


	//Add, subtract, multiply and divide operators with vectors
	template<class vt> inline vec3_t<t> __vectorcall operator + (const vec3_t<vt>& v) const { return { x + v.x, y + v.y, z + v.z }; }
	template<class vt> inline vec3_t<t> __vectorcall operator * (const vec3_t<vt>& v) const { return { x * v.x, y * v.y, z * v.z }; }
	template<class vt> inline vec3_t<t> __vectorcall operator - (const vec3_t<vt>& v) const { return { x - v.x, y - v.y, z - v.z }; }
	template<class vt> inline vec3_t<t> __vectorcall operator / (const vec3_t<vt>& v) const { return { x / v.x, y / v.y, z / v.z }; }
	template<class vt> inline void __vectorcall operator += (const vec3_t<vt>& v) { x += v.x; y += v.y; z += v.z; }
	template<class vt> inline void __vectorcall operator *= (const vec3_t<vt>& v) { x *= v.x; y *= v.y; z *= v.z; }
	template<class vt> inline void __vectorcall operator -= (const vec3_t<vt>& v) { x -= v.x; y -= v.y; z -= v.z; }
	template<class vt> inline void __vectorcall operator /= (const vec3_t<vt>& v) { x /= v.x; y /= v.y; z /= v.z; }


	//Add, subtract, multiply and divide operators with values
	template<class vt> inline vec3_t<t> __vectorcall operator + (const vt& n) const { return { x + n, y + n, z + n }; }
	template<class vt> inline vec3_t<t> __vectorcall operator * (const vt& n) const { return { x * n, y * n, z * n }; }
	template<class vt> inline vec3_t<t> __vectorcall operator - (const vt& n) const { return { x - n, y - n, z - n }; }
	template<class vt> inline vec3_t<t> __vectorcall operator / (const vt& n) const { return { x / n, y / n, z / n }; }
	template<class vt> inline void __vectorcall operator += (const vt& n) { x += n; y += n; z += n; }
	template<class vt> inline void __vectorcall operator *= (const vt& n) { x *= n; y *= n; z *= n; }
	template<class vt> inline void __vectorcall operator -= (const vt& n) { x -= n; y -= n; z -= n; }
	template<class vt> inline void __vectorcall operator /= (const vt& n) { x /= n; y /= n; z /= n; }


private:
	inline void __vectorcall init(const t _x, const t _y, const t _z) { x = _x; y = _y; z = _z; }
};
