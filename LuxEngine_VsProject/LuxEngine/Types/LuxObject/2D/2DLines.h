#pragma once


#include "LuxEngine/Types/LuxObject/LuxObject.h"
#include "LuxEngine/Core/Compute/CBuffers.h"
#include "LuxEngine/Core/Devices.h"
#include <vulkan/vulkan.h>



namespace lux::obj {
	//A bidimensional line with interpolated color and width
	//Lines with width 0 or alpha 0 are not rendered
	struct Line2D : public Base2D {
		void init( );


		//Creates a Line2D object
		//Automatically initializes the object type and the GPU memory
		Line2D( ) {
			init( );

			*fp = { 0, 0 };
			*sp = { 0, 0, };
			*fc = { 0, 0, 0, 0 };
			*sc = { 0, 0, 0, 0 };
			*fw = { 0 };
			*sw = { 0 };
		}

		//Creates a Line2D object with the specified points, colors and widths
		//Automatically initializes the object type and the GPU memory
		//Colors and widths are interpolated
		//*   pFp | first point of the line
		//*   pSp | second point of the line
		//*   pFc | color of the first point
		//*   pSc | color of the second point
		//*   vFw | width of the first point
		//*   vSw | width of the second point
		Line2D(const vec2i32& pFp, const vec2i32& pSp, const vec4f32& pFc, const vec4f32& pSc, const float32 vFw, const float32 vSw) {
			init( );
			//TODO use full 64bit int or just 32 for all objects

			setFp(pFp);
			setFp(pSp);
			*fc = pFc;
			*sc = pSc;
			*fw = vFw;
			*sw = vSw;
		}

		//TODO automatically calculate the offset of the variables with device's minStorageBufferOffset and minUniformBufferOffset
		//TODO convert to variable. or convert the variable to function. idk
		inline int32 getCellSize( ) const final override { return 60; }


		//Updates the object data in the GPU memory
		void update( ) final override {
			//TODO USE IN-SHADER MATRIX TRANSFORM

			//if(parent != nullptr) parent->setChildLimits(childIndex);
			//vec2i32 pMinLim = vec2i32(1920 * 2, 1080) * minLim;
			//vec2i32 pMaxLim = vec2i32(1920 * 2, 1080) * maxLim;


			////TODO fix
			//*fp = (*fp - pMinLim) * dist2D(minLim, maxLim) + pMinLim;		//    8    |    0  - 7     |    0 +
			//*sp = (*sp - pMinLim) * dist2D(minLim, maxLim) + pMinLim;		//    8    |    8  - 15    |    0
			//*(vec2i32*)((int8*)cellPtr + 0) = (fp - pMinLim) * dist2D(minLim, maxLim) + pMinLim;		//    8    |    0  - 7     |    0 +
			//*(vec2i32*)((int8*)cellPtr + 8) = (sp - pMinLim) * dist2D(minLim, maxLim) + pMinLim;		//    8    |    8  - 15    |    0
			//*(vec4f32*)((int8*)cellPtr + 16) = fc;														//    16   |    16 - 31    |    1 +
			//*(vec4f32*)((int8*)cellPtr + 32) = sc;														//    16   |    32 - 47    |    2 +
			//*(float32*)((int8*)cellPtr + 48) = fw;														//    4    |    48 - 51    |    3 +
			//*(float32*)((int8*)cellPtr + 52) = sw;														//    4    |    52 - 55    |    3
			////TODO move to one-time init
			////TODO use full 64bit int or just 32 for all objects
			//*(uint32*)((int8*)cellPtr + 56) = (uint32)ID;												//    4    |    56 - 59    |    3
			////4x trailing padding																		//    4    |    60 - 63

			////TODO recalculate limits for children

			vec2i32 pMinLim = vec2i32(1920 * 2, 1080) * minLim;
			*fp = (fp_temp - pMinLim) * dist2D(minLim, maxLim) + pMinLim;
			*sp = (sp_temp - pMinLim) * dist2D(minLim, maxLim) + pMinLim;

			void* h = core::c::buffers::gpuCellMap(gpuCell);
			void* h2 = core::c::buffers::gpuCellMap(localCell);
			memcpy(h2, h, getCellSize( ));
			//if(core::c::buffers::CBuffers[getBufferIndex(gpuCell)].isMapped) vkUnmapMemory(core::dvc::compute.LD, core::c::buffers::CBuffers[getBufferIndex(gpuCell)].memory);
			//if(core::c::buffers::CBuffers[getBufferIndex(gpuCell)].isMapped) vkUnmapMemory(core::dvc::compute.LD, core::c::buffers::CBuffers[getBufferIndex(localCell)].memory);
		}

		inline void setFp(const vec2i32& vFp){ fp_temp = vFp; update(); }
		inline void setSp(const vec2i32& vSp){ sp_temp = vSp; update(); }



	private:
		vec2i32* fp;		//First point of the line
		vec2i32* sp;		//Second point of the line
		vec2i32 fp_temp;		//TODO remove. Temporary workaround for the absence of a shader matrix transform
		vec2i32 sp_temp;		//TODO remove. Temporary workaround for the absence of a shader matrix transform
	public:

		vec4f32* fc;		//Color of the first point
		vec4f32* sc;		//Color of the second point
		float32* fw;		//Width of the first point
		float32* sw;		//Width of the second point
	};
}

//TODO dual camera