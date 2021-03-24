#pragma once
#include "LuxEngine/Core/Render/Shaders/Shader.hpp"
#include "LuxEngine/Types/LuxObject/2D/Obj2_b.hpp"
#include "LuxEngine/Contents/shaders/Border2D.hpp"



namespace lux::obj{
	struct Border2D : public Obj2_b{
		shd::Border2D data;


		virtual ram::ptr<char> getShData(){ return data.objData_.data; }
		virtual vram::Alloc_b<char> getShVData(){ return data.objData_.vdata; }


		Border2D() {
			luxInitObject(2, BORDER);

			data.objData_.ffp() = { .0f, .0f };
			data.objData_.fsp() = { .0f, .0f };
			data.objData_.ID() = (uint32)common.ID;
		}
		Border2D(const f32v2& vFp, const f32v2& vSp) {
			luxInitObject(2, BORDER);

			data.objData_.ffp() = vFp;
			data.objData_.fsp() = vSp;
			data.objData_.ID() = (uint32)common.ID;
		}

		/**
		 * @brief Initializes the GPU data that allows the window to render the object
		 */
		virtual void init(Window& pWindow) override;
	};
}