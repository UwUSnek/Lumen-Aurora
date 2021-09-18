
//####################################################################################
// This file was generated by Lynx/shaders/GlslToCpp.py
// Changes could be overwritten without notice
//####################################################################################

#pragma once
#include "Lynx/Core/Render/Shaders/ShaderInterface_t.hpp"



namespace lnx::shd::gsi{
    struct Volume : public ShaderInterface_b {
        static ShaderInterface_b::Layout layout;
        static uint32 pipelineIndex;


		struct l_outcol : public ShaderElm_b<eStorage, f32v4[30], i32, f32v4[0]> {
			alwaysInline l_outcol(const bool vExt) {}
			inline l_outcol() {
				ShaderElm_b::vdata.realloc(512);
				ShaderElm_b:: data.realloc(512);
			}
			inline l_outcol(const l_outcol& pOutcol) {
				ShaderElm_b:: data = pOutcol. data;
				ShaderElm_b::vdata = pOutcol.vdata;
			}
			inline l_outcol& operator=(const l_outcol& pOutcol) {
				ShaderElm_b:: data = pOutcol. data;
				ShaderElm_b::vdata = pOutcol.vdata;
				return *this;
				//FIXME automatically update render data after calling this function
			}
			inline l_outcol(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
			}
			inline auto& operator=(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
				return *this;
			}
			f32v4& arrayh = *(f32v4*)(ShaderElm_b::data + 0);
			uint64 arrayh_tmp_size = 480;
			i32& valueh = *(i32*)(ShaderElm_b::data + 480);
			f32v4& outcol = *(f32v4*)(ShaderElm_b::data + 496);
			uint64 outcol_tmp_size = 0;
		};
		l_outcol outcol{ true };


		struct l_wsize : public ShaderElm_b<eStorage, u32v2> {
			alwaysInline l_wsize(const bool vExt) {}
			inline l_wsize() {
				ShaderElm_b::vdata.realloc(256);
				ShaderElm_b:: data.realloc(256);
			}
			inline l_wsize(const l_wsize& pWsize) {
				ShaderElm_b:: data = pWsize. data;
				ShaderElm_b::vdata = pWsize.vdata;
			}
			inline l_wsize& operator=(const l_wsize& pWsize) {
				ShaderElm_b:: data = pWsize. data;
				ShaderElm_b::vdata = pWsize.vdata;
				return *this;
				//FIXME automatically update render data after calling this function
			}
			inline l_wsize(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
			}
			inline auto& operator=(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
				return *this;
			}
			u32v2& wsize = *(u32v2*)(ShaderElm_b::data + 0);
		};
		l_wsize wsize{ true };


		struct l__data : public ShaderElm_b<eUniform, f32v3, f32, f32v4> {
			alwaysInline l__data(const bool vExt) {}
			inline l__data() {
				ShaderElm_b::vdata.realloc(256);
				ShaderElm_b:: data.realloc(256);
			}
			inline l__data(const l__data& p_data) {
				ShaderElm_b:: data = p_data. data;
				ShaderElm_b::vdata = p_data.vdata;
			}
			inline l__data& operator=(const l__data& p_data) {
				ShaderElm_b:: data = p_data. data;
				ShaderElm_b::vdata = p_data.vdata;
				return *this;
				//FIXME automatically update render data after calling this function
			}
			inline l__data(const vram::ptr<auto, eVRam, eUniform>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eUniform>)pVPtr;
			}
			inline auto& operator=(const vram::ptr<auto, eVRam, eUniform>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eUniform>)pVPtr;
				return *this;
			}
			f32v3& pos = *(f32v3*)(ShaderElm_b::data + 0);
			f32& r = *(f32*)(ShaderElm_b::data + 16);
			f32v4& col = *(f32v4*)(ShaderElm_b::data + 32);
		};
		l__data _data{ true };


		void create(
			const l_outcol& pOutcol,
			const l_wsize& pWsize,
			const u32v3 vGroupCount, core::RenderCore& pRenderCore
		);
		void createDescriptorSets();
		void createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);
		void updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);
		void destroy();
    };
}