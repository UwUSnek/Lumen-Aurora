
//####################################################################################
// This file was generated by Tools/Build/Generators/GenInterfaces
// Changes could be overwritten without notice
//####################################################################################

#pragma once
#include "Lynx/Core/Render/Shaders/ShaderInterface_t.hpp"
#include "Lynx/Core/Init.hpp"




namespace lnx::shd::gsi{
	_lnx_init_var_value_dec((InterfaceLayout), Line2_layout);
	_lnx_init_var_value_dec((uint32),          Line2_pipelineIndex);
	struct Line2 : public ShaderInterface_b {




		struct l_data : public ShaderElm_b<eUniform> {
			alwaysInline l_data(const Dummy) : ShaderElm_b() {}
			inline l_data() : ShaderElm_b(256) {}
			inline l_data(const l_data& pData) {
				ShaderElm_b:: data = pData. data;
				ShaderElm_b::vdata = pData.vdata;
			}
			inline l_data& operator=(const l_data& pData) {
				ShaderElm_b:: data = pData. data;
				ShaderElm_b::vdata = pData.vdata;
				return *this;
				//FIXME automatically update render data after calling this function
				//FIXME automatically update render data after calling this function
			}
			inline l_data(const vram::ptr<auto, eVRam, eUniform>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eUniform>)pVPtr;
			}
			inline auto& operator=(const vram::ptr<auto, eVRam, eUniform>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eUniform>)pVPtr;
				return *this;
			}
		private:
			friend struct Line2;
			f32v2* _pvt_elm_fp0 = (f32v2*)(ShaderElm_b::data + 0);
			f32v2* _pvt_elm_fp1 = (f32v2*)(ShaderElm_b::data + 0);
			f32v4* _pvt_elm_col0 = (f32v4*)(ShaderElm_b::data + 16);
			f32v4* _pvt_elm_col1 = (f32v4*)(ShaderElm_b::data + 0);
			f32* _pvt_elm_wd0 = (f32*)(ShaderElm_b::data + 0);
			f32* _pvt_elm_wd1 = (f32*)(ShaderElm_b::data + 0);
			u32* _pvt_elm_ID = (u32*)(ShaderElm_b::data + 0);
		public:
			alwaysInline f32v2& lFp0(){
				return *_pvt_elm_fp0;
			}
			alwaysInline f32v2& lFp1(){
				return *_pvt_elm_fp1;
			}
			alwaysInline f32v4& lCol0(){
				return *_pvt_elm_col0;
			}
			alwaysInline f32v4& lCol1(){
				return *_pvt_elm_col1;
			}
			alwaysInline f32& lWd0(){
				return *_pvt_elm_wd0;
			}
			alwaysInline f32& lWd1(){
				return *_pvt_elm_wd1;
			}
			alwaysInline u32& lID(){
				return *_pvt_elm_ID;
			}
		};
		l_data data;


		struct l_outcol : public ShaderElm_b<eStorage> {
			alwaysInline l_outcol(const Dummy) : ShaderElm_b() {}
			inline l_outcol() : ShaderElm_b(0) {}
			inline l_outcol(const l_outcol& pOutcol) {
				ShaderElm_b:: data = pOutcol. data;
				ShaderElm_b::vdata = pOutcol.vdata;
			}
			inline l_outcol& operator=(const l_outcol& pOutcol) {
				ShaderElm_b:: data = pOutcol. data;
				ShaderElm_b::vdata = pOutcol.vdata;
				return *this;
				//FIXME automatically update render data after calling this function
				//FIXME automatically update render data after calling this function
			}
			inline l_outcol(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
			}
			inline auto& operator=(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
				return *this;
			}
		private:
			friend struct Line2;
			f32v4* _pvt_elm_outcol = nullptr;
			uint64 outcol_tmp_size = 0;
		public:
			alwaysInline f32v4& eOutcol(){
				return *_pvt_elm_outcol;
			}
		};
		l_outcol outcol{ Dummy() };


		struct l_wsize : public ShaderElm_b<eStorage> {
			alwaysInline l_wsize(const Dummy) : ShaderElm_b() {}
			inline l_wsize() : ShaderElm_b(256) {}
			inline l_wsize(const l_wsize& pWsize) {
				ShaderElm_b:: data = pWsize. data;
				ShaderElm_b::vdata = pWsize.vdata;
			}
			inline l_wsize& operator=(const l_wsize& pWsize) {
				ShaderElm_b:: data = pWsize. data;
				ShaderElm_b::vdata = pWsize.vdata;
				return *this;
				//FIXME automatically update render data after calling this function
				//FIXME automatically update render data after calling this function
			}
			inline l_wsize(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
			}
			inline auto& operator=(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
				return *this;
			}
		private:
			friend struct Line2;
			u32v2* _pvt_elm_wsize = nullptr;
		public:
			alwaysInline u32v2& eWsize(){
				return *_pvt_elm_wsize;
			}
		};
		l_wsize wsize{ Dummy() };


		struct l_zbuff : public ShaderElm_b<eStorage> {
			alwaysInline l_zbuff(const Dummy) : ShaderElm_b() {}
			inline l_zbuff() : ShaderElm_b(0) {}
			inline l_zbuff(const l_zbuff& pZbuff) {
				ShaderElm_b:: data = pZbuff. data;
				ShaderElm_b::vdata = pZbuff.vdata;
			}
			inline l_zbuff& operator=(const l_zbuff& pZbuff) {
				ShaderElm_b:: data = pZbuff. data;
				ShaderElm_b::vdata = pZbuff.vdata;
				return *this;
				//FIXME automatically update render data after calling this function
				//FIXME automatically update render data after calling this function
			}
			inline l_zbuff(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
			}
			inline auto& operator=(const vram::ptr<auto, eVRam, eStorage>& pVPtr){
				vdata = (vram::ptr<char, eVRam, eStorage>)pVPtr;
				return *this;
			}
		private:
			friend struct Line2;
			u32* _pvt_elm_zbuff = nullptr;
			uint64 zbuff_tmp_size = 0;
		public:
			alwaysInline u32& eZbuff(){
				return *_pvt_elm_zbuff;
			}
		};
		l_zbuff zbuff{ Dummy() };
		void spawn(
			const l_outcol& pOutcol,
			const l_wsize& pWsize,
			const l_zbuff& pZbuff,
			const u32v3 vGroupCount, core::RenderCore& pRenderCore
		);
		void createDescriptorSets();
		void createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);
		void updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);
		void destroy();
    };



    _lnx_init_fun_dec(LNX_H_LINE2);
}