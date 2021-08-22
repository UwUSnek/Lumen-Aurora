
//####################################################################################
// This file was generated by Lynx/shaders/GlslToCpp.py
// Changes could be overwritten without notice
//####################################################################################

#pragma once
#include "Lynx/Core/Render/Shaders/Shader_t.hpp"



namespace lnx::shd{
	struct _3DTest : public Shader_b {
		static Shader_b::Layout layout;
		static uint32 pipelineIndex;


		struct outcol_t : public ShaderElm_b<eStorage> {
			outcol_t() {
				ShaderElm_b::bind = 0;
			}
		} outcol;


		struct wsize_t : public ShaderElm_b<eStorage> {
			wsize_t() {
				ShaderElm_b::bind = 1;
			}
		} wsize;


		struct _data_t : public ShaderElm_b<eUniform> {
			_data_t() {
				ShaderElm_b::vdata.realloc(256);
				ShaderElm_b::data.realloc(256);
				ShaderElm_b::bind = 2;
			}
			alwaysInline f32v3& pos() { return *(f32v3*)ShaderElm_b::data; }
			alwaysInline f32& r() { return *(f32*)(ShaderElm_b::data + 16); }
			alwaysInline f32v4& col() { return *(f32v4*)(ShaderElm_b::data + 32); }
		} _data;


		void create(vram::ptr<f32v4, eVRam, eStorage> pOutcol, vram::ptr<u32v2, eVRam, eStorage> pWsize, const u32v3 vGroupCount, core::RenderCore& pRenderCore);
		void createDescriptorSets();
		void createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);
		void updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);
		void destroy();
	};
}//TODO remove local data in external bindings