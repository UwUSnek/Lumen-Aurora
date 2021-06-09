#pragma once
#define LNX_H_LNX_OBJECT

#include "Lynx/Types/Vectors/Vectors.hpp"
#include "Lynx/Types/Containers/String.hpp"
#include "Lynx/Core/Render/Shaders/Shader.hpp"
#include "Lynx/Types/VPointer.hpp"
//BUG REPLACE C STYLE CASTS WITH C++ CASTS







namespace lnx{
	//This namespace contains all the structures and functions of render objects
	namespace obj {
		struct Border2;


		enum class limitAlignment : int16{
			eTop,
			eBottom,
			eRight,
			eLeft,
			eCenter
		};



		enum UpdateBits : uint32 {
			eNone    = 0,
			eSpawn   = 0b0001,
			eLimit   = 0b0100,
			eUpdateg = 0b0010
		};
		alwaysInline UpdateBits operator~ (UpdateBits  a){ return (UpdateBits)((u32)~ (u32)a); }
		alwaysInline UpdateBits operator| (UpdateBits  a, UpdateBits b){ return (UpdateBits )((u32 )a |  (u32)b); }
		alwaysInline UpdateBits operator& (UpdateBits  a, UpdateBits b){ return (UpdateBits )((u32 )a &  (u32)b); }
		alwaysInline UpdateBits operator^ (UpdateBits  a, UpdateBits b){ return (UpdateBits )((u32 )a ^  (u32)b); }
		alwaysInline UpdateBits operator|=(UpdateBits& a, UpdateBits b){ return (UpdateBits&)((u32&)a |= (u32)b); }
		alwaysInline UpdateBits operator&=(UpdateBits& a, UpdateBits b){ return (UpdateBits&)((u32&)a &= (u32)b); }
		alwaysInline UpdateBits operator^=(UpdateBits& a, UpdateBits b){ return (UpdateBits&)((u32&)a ^= (u32)b); }


                struct Structural : virtual public Obj_bb {
                    virtual ram::ptr<char>       getShData() override { dbg::printError("Unable to call getShData on structural object"; return nullptr; }
                    virtual vram::Alloc_b<char> getShVData() override { dbg::printError("Unable to call getShVData on structural object"; return vram::Alloc_b<char>(); }
                };
                struct Opaque {};


		/**
		 * @brief Members common to any Obj_bt instantiation
		 */
		struct Obj_bb { //
			_dbg(const char* dbgName;)
			struct Common{
				static std::atomic<uint64> lastID;							//#LLID LOS000 the last assigned ID of a Lynx object
				uint64 ID{ ++lastID };							//A unique ID that indentifies the object
				uint32 childIndex{ (uint32)-1 };				//The index of the object in the parent's children list
			} common;
			virtual void setChildLimits(const uint32 vChildIndex) const = 0;
			virtual ram::ptr<char>       getShData() = 0;
			virtual vram::Alloc_b<char> getShVData() = 0;

			virtual Obj_bb* getChildren(uint32 vIndex) = 0;			//FIXME UNIFY CHILDREN ACCESS FUNCTIONS
			virtual uint32  getChildrenCount() = 0;					//FIXME UNIFY CHILDREN ACCESS FUNCTIONS
			virtual bool    getChildrenIsValid(uint32 vIndex) = 0; 	//FIXME UNIFY CHILDREN ACCESS FUNCTIONS

			struct Render{									//Structure containing rendering helper members
				// _dbg(bool isDbgObj = false;)					//True if the object is used for graphical debugging
				std::atomic<UpdateBits> updates;				//Update requests sent to the render thread //FIXME MAKE NON ATOMIC
				Window* parentWindow = nullptr;					//Parent window object that contains the render thread and the window data
			} render;

			virtual void onSpawn(Window& pWindow){
				dbg::checkCond(render.parentWindow && thr::self::thr() != render.parentWindow->renderCore.t.thr, "This function can only be called by the render thread.");
			}
			virtual void onLimit(){
				dbg::checkCond(render.parentWindow && thr::self::thr() != render.parentWindow->renderCore.t.thr, "This function can only be called by the render thread.");
			}
			virtual void onUpdateg(vk::CommandBuffer pCB){
				dbg::checkCond(render.parentWindow && thr::self::thr() != render.parentWindow->renderCore.t.thr, "This function can only be called by the render thread.");
			}

			//TODO comment
			void queue(UpdateBits vUpdates){
				UpdateBits old = render.updates;						//Save old updates bits
				if(render.parentWindow) { 								//If the object has a binded window
					render.parentWindow->renderCore.requests_m.lock();					//Lock requests mutex
						render.updates = render.updates | vUpdates;			//Update updates bits
						if(!old) render.parentWindow->renderCore.requests.add(this);	//If it isn't already in it, add the object to the update queue
					render.parentWindow->renderCore.requests_m.unlock();				//Unlock requests mutex
				}														//If not
				else render.updates = render.updates | vUpdates;			//Update updates bits
			}
		};




		/**
		 * @brief Base class of any render object of any dimension
		 * @tparam chType Type of the children objects. Can be any subclass of Obj_b
		 */
		template<class chType, class tType> struct Obj_bt : virtual public Obj_bb, public tType {
                        static_assert(std::is_same<tType, Opaque> || std::is_same<tType, Structural>, "Object type can only be \"Structural\" or \"Opaque\"");
			RaArray<chType*, uint32> children;
			virtual Obj_bb* getChildren(uint32 vIndex) override { return static_cast<Obj_bb*>(children[vIndex]); } 	//FIXME UNIFY CHILDREN ACCESS FUNCTIONS
			virtual uint32  getChildrenCount() override { return children.count(); } 								//FIXME UNIFY CHILDREN ACCESS FUNCTIONS
			virtual bool    getChildrenIsValid(uint32 vIndex) override { return children.isValid(vIndex); } 		//FIXME UNIFY CHILDREN ACCESS FUNCTIONS
		};
	}
}
