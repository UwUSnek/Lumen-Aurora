#include "Lynx/Types/Object/2D/Obj2_b.hpp"
#include "Lynx/Types/Object/2D/Border2.hpp"


namespace lnx::obj{
	//In debug mode, setMinLim and setMaxLim functions of non debug objects and update the debug border
	#ifdef LNX_DEBUG
	void Obj2_b::setMinLim(f32v2 vMinLim) {
		minLim = vMinLim;
		if(!debug && debugBorder) {
			debugBorder->data.objData_.ffp() = vMinLim;
			debugBorder->qHierarchy();
		}
	}
	void Obj2_b::setMaxLim(f32v2 vMaxLim) {
		maxLim = vMaxLim;
		if(!debug && debugBorder) {
			debugBorder->data.objData_.fsp() = vMaxLim;
			debugBorder->qHierarchy();
		}
	}
	#endif




	void Obj2_b::onSpawn(Window& pWindow) {
        render.parentWindow = &pWindow;                         //BUG OVER
        for(u32 i = 0; i < children.count(); ++i){              //BUG OVER
            if(children.isValid(i)) children[i]->onSpawn(pWindow); //BUG >IN >OUT >IN >OUT >IN
        }
		#ifdef LNX_DEBUG
        	if(!debug) {
				debugBorder = new Border2();
				debugBorder->debug = true;
				debugBorder->onSpawn(pWindow);
			}
		#endif

		if(doesRedefine(*this, &MouseCallbacks_b::onClick)){
			pWindow.onClick_m.lock();
			pWindow.onClickList.add(this);
			pWindow.onClick_m.unlock();
		}
		if(doesRedefine(*this, &MouseCallbacks_b::onHover)){
			pWindow.onHover_m.lock();
			pWindow.onHoverList.add(this);
			pWindow.onHover_m.unlock();
		}
		if(doesRedefine(*this, &MouseCallbacks_b::onEnter)){
			pWindow.onEnter_m.lock();
			pWindow.onEnterList.add(this);
			pWindow.onEnter_m.unlock();
		}
		if(doesRedefine(*this, &MouseCallbacks_b::onExit)){
			pWindow.onExit_m.lock();
			pWindow.onExitList.add(this);
			pWindow.onExit_m.unlock();
		}
		if(doesRedefine(*this, &MouseCallbacks_b::onAxis)){
			pWindow.onAxis_m.lock();
			pWindow.onAxisList.add(this);
			pWindow.onAxis_m.unlock();
		}
    }
}