#pragma once


#include "LuxEngine/Types/LuxObject/LuxObject.h"
#include <vulkan/vulkan.h>




//A bidimensional line with interpolated color and width
//Lines with the same color or width have better performance
//Perfectly vertical or horyzontal lines also have better performance
//Lines with size 0 or alpha 0 are not rendered
struct LuxDynamic_LuxObjectLineCCT : public LuxObject2D_base1 {
	LuxDynamic_LuxObjectLineCCT() { objectType = LUX_OBJECT_TYPE_2D_LINE; }

	void initPtrs() final override {
		p0 = (vec2i32*)(((int32*)cellPtr) + 0);
		p1 = (vec2i32*)(((int32*)cellPtr) + 2);

		col0 = (vec4f32*)(((int32*)cellPtr) + 4);
		col1 = (vec4f32*)(((int32*)cellPtr) + 8);

		wd0 = ((float32*)cellPtr) + 12;
		wd1 = ((float32*)cellPtr) + 13;
	}

	vec2i32* p0{ nullptr };		//First point of the line
	vec2i32* p1{ nullptr };		//Second point of the line

	vec4f32* col0{ nullptr };	//Color of the first point
	vec4f32* col1{ nullptr };	//Color of the second point

	float32* wd0{ nullptr };	//Width of the first point
	float32* wd1{ nullptr };	//Width of the second point
};


//TODO dual camera