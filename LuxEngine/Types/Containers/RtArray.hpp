﻿
#pragma once
#define LUX_H_RTARRAY
#include "LuxEngine/Types/Containers/ContainerBase.hpp"
#include "LuxEngine/Math/Algebra/Algebra.hpp"
#include <initializer_list>
#include <cstring>




//TODO a low priority thread reorders the points in the meshes
//TODO If the mesh gets modified, it's sended back to the queue
//TODO if not, the new points are saved and used for rendering in the next frames

//TODO "runtime 3D turbolent flow"


//TODO add .reassign function for containers
//TODO the function simply initializes the container with a pointer without copying the data
//TODO foo.reassign(ptr);
//TODO Array<int> reassign(foo, ptr);
//TODO or maybe it's a bad idea. multiple threads or objects modifying the same data... idk

//TODO add .move function in containers instead of .reassign
//TODO .moved arrays becomes invalid

//TODO add contructor of string from lux containers of chars
//TODO add additional data in errors
namespace lux {
	//"RunTime Array"
	//A dynamic array that uses the global memory pool
	template<class type, class iter = uint32> struct RtArray : public ContainerBase<type, iter> {
		#define Super ContainerBase<type, iter>
	private:
		genInitCheck;
		// ram::Alloc<type> data_;	//Elements of the array

	public:




		// Constructors -------------------------------------------------------------------------------------------------------- //




		// luxDebug(bool checkNeg(iter n) { luxCheckParam(n < 0, n, "Size cannot be negative"); return true; })
		inline RtArray(           ) : Super(      ) { }
		inline RtArray(iter vCount) : Super(vCount) { }

		template<class... types> inline RtArray(types&&... vElms) : Super{ vElms... } { }
		// //Initializes the array using a container object of a compatible type
		// //*   pContainer | The container object to copy elements from
		// //*       The pContainer iterator must be of equal or smaller type than the one of the object you are initializing
		// template<class cIter> inline RtArray(const ContainerBase<type, cIter>& pCont) :
		// 	ContainerBase<type, iter>(pCont.count( )) {
		// 	//FIXME check count and not iterator
		// 	luxCheckParam(sizeof(cIter) > sizeof(iter), pCont, "The iterator of a container must be larger than the one of the container used to initialize it");
		// 	isInit(pCont);
		// 	// ram::cpy(pContainer.begin( ), data_, pContainer.size( ));
		// 	for(int i = 0; i < pCont.count(); ++i){ operator[](i) = *(pCont.begin() + i); }
		// }


		//*Initializes the array using a container object of a compatible type
		//*   pContainer | The container object to copy elements from
		//*       The pContainer iterator must be of equal or smaller type than the one of the object you are initializing

		/**
		 * @brief Initializes the array with a lux::ContainerBase subclass instance by calling operator= on each element
		 * @param pCont The container to copy elements from
		 */
		template<class cType, class cIter> inline RtArray(const ContainerBase<cType, cIter>& pCont) :
			Super(pCont.count( )) {
			//FIXME check count and not iterator
			luxCheckParam(sizeof(cIter) > sizeof(iter), pCont, "The iterator of a container must be larger than the one of the container used to initialize it");
			isInit(pCont);
			// ram::cpy(pContainer.begin( ), data_, pContainer.size( ));
			for(int i = 0; i < pCont.count(); ++i){ operator[](i) = (type)*(pCont.begin() + i); }
		}
		/**
		 * @brief Copy constructor
		 */
		inline RtArray(const RtArray<type, iter>& pCont) : RtArray<type, iter>((Super)pCont) { }


		/**
		 * @brief Move constructor
		 */
		inline RtArray(RtArray<type, iter>&& pCont){
			Super::data = pCont.data;	pCont.data = nullptr;
		}




		// //TODO remove
		// //Initializes the array using a list of elements of the same type
		// inline RtArray(const std::initializer_list<type>& pElements) : data_(sizeof(type) * pElements.size( ), CellClass::AT_LEAST_CLASS_B) {
		// 	//TODO ^ C strings get destroyed when the function returns
		// 	//luxCheckParam(pElements.size( ) > count_, pElements, "%d-elements CtArray initialized with %d-elements container.\nA compile time array cannot be initialized with larger containers", count_, pElements.size( ));

		// 	memcpy(begin( ), pElements.begin( ), ((pElements.size( ) * sizeof(type))));
		// }


		// inline ~RtArray(){
		// 	// data_.~ptr();
		// }





		// Add, remove --------------------------------------------------------------------------------------------------------- //




		//Resizes the array without initializing the new elements
		//*   vNewSize | new count of the array
		//*   Returns  | the new count
		//TODO totally useless. Just don't return
		inline iter resize(const iter vNewSize) {
			checkInit(); luxCheckParam(vNewSize < 0, vNewSize, "The size of a container cannot be negative");
			Super::data.reallocArr(vNewSize, type( ));
			return Super::data.count( );
		}


		//Resets the array to its initial state, freeing the memory and resizing it to 0
		inline void clear( ){
			checkInit();
			Super::data.free();

			//TODO dont call this directly. add construct function
			// this->DynArray::DynArray( );
			//TODO constructor
			Super::data.realloc(0, type(), CellClass::AT_LEAST_CLASS_B);
		}


		//Adds an element to the end of the array
		//*   vElement | the element to add
		//*   Returns  | the index of the element in the array
		inline iter add(const type& vElement) {
			checkInit();
			resize(Super::data.count() + 1);
			*(Super::data.end( ) - 1) = vElement;
			return Super::data.count( ) - 1;
		}




		// Get ----------------------------------------------------------------------------------------------------------------- //




		// inline iter	  count( )	const override { checkInit(); return data_.count( );		  }
		inline uint64 size( ) const { checkInit(); return Super::count( ) * sizeof(type); }
		// inline bool	  empty( )	const override { checkInit(); return !count( );				  }
		// inline type*  begin( )	const override { checkInit(); return data_.begin( );		  }
		// inline type*  end( )	const override { checkInit(); return data_.end( );			  }

		inline type&  operator[](const iter vIndex) const {
			checkInit();
			luxCheckCond(Super::count() == 0,                "This function cannot be called on containers with size 0");
			luxCheckParam(vIndex < 0, vIndex,         "Index cannot be negative");
			luxCheckParam(vIndex >= Super::count( ), vIndex, "Index is out of range");
			return Super::data[vIndex];
		}
		#undef Super
	};
}


//TODO check if non secure C pointers were used. Like const char* strings