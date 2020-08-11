
#include "LuxEngine/Core/Core.h"
#include "LuxEngine/Core/Compute/CShader.h"
#include "LuxEngine/Types/LuxObject/LuxObject.h"
#include "LuxEngine/Types/LuxObject/2D/2DBorder.h"






//Adds a render space with no parent to the engine
namespace lux::obj{
	uint64 Base::Common::lastID = 0;	//#LLID LOS000 initialize the last object ID


	void addRenderSpace(RenderSpace2D* pRenderSpace){
		core::c::shaders::CRenderSpaces.add(pRenderSpace);
	}


	//This function allocates the object data in the shared memory of the GPU, eventually initializing the engine
	//An object needs to be allocated before being read or written
	void Base::allocate( ){
		if(common.objectType >= 3000 || common.objectType < 2000) return;
		//TODO add initialization for 1d, 2.5d and 3d non base objects
		core::init(false);											//Initialize the engine
		//TODO use GPU local memory
		//render.data = core::c::buffers::gpuCellCreate(getCellSize( ), true);		//Create the cell taht contains the object data
		render.data = malloc(getCellSize( ));		//Create the cell taht contains the object data
		//render.dataPtr = core::c::buffers::gpuCellMap(render.data);						//Map the cell pointer to the cell
		//TODO
		render.localData = core::c::buffers::gpuCellCreate(getCellSize( ), false, true);		//Create the cell taht contains the object data
	}


	void Base::updateBase( ){
		if(common.objectType == ObjectType::LUX_OBJECT_TYPE_2D_RENDER_SPACE || common.objectType == ObjectType::LUX_OBJECT_TYPE_3D_RENDER_SPACE) return;
		core::g::pendingObjectUpdatesFence.startSecond( );
		if(render.updated){
			render.updated = false;
			core::g::objUpdates2D.add(this);
		}
		core::g::pendingObjectUpdatesFence.endSecond( );
	}

	#ifdef LUX_DEBUG
	void __vectorcall Base2D::setMinLim(vec2f32 vMinLim){
		minLim = vMinLim;
		if(!debug){
			if(debugBorder == nullptr) {
				debugBorder = new Border2D( );
				debugBorder->debug = true;
			}
			//debugBorder->fp_tmp = vMinLim;
			*debugBorder->fp = vMinLim;
			//Updated from parent update() function
			//debugBorder->update( );
		}
	}
	void __vectorcall Base2D::setMaxLim(vec2f32 vMaxLim){
		maxLim = vMaxLim;
		if(!debug){
			if(debugBorder == nullptr) {
				debugBorder = new Border2D( );
				debugBorder->debug = true;
			}
			//debugBorder->sp_tmp = vMaxLim;
			*debugBorder->sp = vMaxLim;
			//Updated from parent update() function
			//debugBorder->update( );
		}
	}
	#endif




	Base2D::Base2D( ){
		common.objectType = LUX_OBJECT_TYPE_2D__BASE;
	}
}