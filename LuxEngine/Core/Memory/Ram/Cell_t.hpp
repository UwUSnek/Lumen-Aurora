#pragma once
#define LUX_H_CELL_T
#include "LuxEngine/Core/Memory/Ram/Classes.hpp"
#include "LuxEngine/Types/Containers/RaArrayC.hpp"
#include "LuxEngine/Types/Dummy.hpp"
#include <mutex>








namespace lux::ram{
	template<class type> struct ptr;
	//! If you modify those variables change the declarations in Ram.hpp and Ram.cpp too


	struct Cell_t {
		uint16 typeIndex;		//INDEX of the buffer type. -1 for custom size cells		//2
		uint16 owners;			//Number of lux::ram::ptr instances that owns the cell		//2
		uint32 cellIndex;		//Index of the cell in the cells array						//4
		uint32 localIndex;		//Index of the cell in the type allocations					//4
		uint32 cellSize;		//Size of the cell in bytes									//4
		void*  address;			//Address of the cell										//8
		luxDebug(ptr<Dummy>* lastOwner;)													//[8]
		luxDebug(ptr<Dummy>* firstOwner;)													//[8]
	};

	struct alignas(64) Type_t {
		CellClass cellClass;	//Class of the cells
		void** memory;			//Addresses of the buffers
		RaArrayC<bool> cells;	//TODO use optimized uint64 array
		uint32 cellsPerBuff;	//Number of cells in each buffer
		std::mutex m; 			//FIXME REMOVE
	};

	extern Type_t types[];		//Allocated buffers
	extern RaArrayC<Cell_t> cells;
	extern thread_local Cell_t dummyCell;

	extern std::mutex cells_m;
	extern uint32 allocated;	//TODO remove
}