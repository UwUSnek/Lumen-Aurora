#pragma once
#include "LuxEngine/Types/Integers/Integers.h"
#include "LuxEngine/Core/Devices.h"
#include "LuxEngine/Types/Containers/LuxMap.h"
#include "LuxEngine/Core/Compute/CBuffers.h"
#include <intrin.h>





namespace lux::rem{
	//TODO check for alllocation number limit
	//TODO use multiple devices

	//Bytes to allocate for each cell
	//Buffer classes and addresses are 32-byte aligned to allow the use of AVX2 and match the GPU minimum offsets
	enum CellClass : uint32 {
		LUX_CELL_CLASS_A = 32,						//32 bytes. The minimum size of a cell
		LUX_CELL_CLASS_B = LUX_CELL_CLASS_A * 16,	//16x LUX_CELL_CLASS_A. 512 B per cell (~0.5KB)
		LUX_CELL_CLASS_C = LUX_CELL_CLASS_A * 32,	//32x LUX_CELL_CLASS_A. 1024 B per cell (~1KB)
		LUX_CELL_CLASS_D = LUX_CELL_CLASS_A * 64,	//64x LUX_CELL_CLASS_A. 2048 B per cell (~2KB)
		LUX_CELL_CLASS_Q = LUX_CELL_CLASS_D * 64,	//64x LUX_CELL_CLASS_D. 131072 B per cell (~130KB)
		LUX_CELL_CLASS_L = LUX_CELL_CLASS_Q * 64,	//64x LUX_CELL_CLASS_Q. 8388608 B per cell (~8.4MB)
		LUX_CELL_CLASS_0 = 0,						//Dedicated buffer for cells larger than LUX_CELL_CLASS_L
		LUX_CELL_CLASS_AUTO = (uint32)-1,			//Choose a class large enough to contain the cell
	};
	const uint32 bufferSize = LUX_CELL_CLASS_L * 6;	//Size of each buffer. 50331648 B (~50MB)
	enum CellClassIndex : uint32 {
		LUX_CELL_CLASS_INDEX_A = 0b000,
		LUX_CELL_CLASS_INDEX_B = 0b001,
		LUX_CELL_CLASS_INDEX_C = 0b010,
		LUX_CELL_CLASS_INDEX_D = 0b011,
		LUX_CELL_CLASS_INDEX_Q = 0b100,
		LUX_CELL_CLASS_INDEX_L = 0b101,
		LUX_CELL_CLASS_INDEX_0 = 0b110,
		LUX_CELL_CLASS_NUM 							//Number of LUX_CELL_CLASS values
	};
	enum AllocType : uint32 {
		LUX_ALLOC_TYPE_DEDICATED_STORAGE = 0b00,	//Storage buffer in dedicated GPU memory
		LUX_ALLOC_TYPE_DEDICATED_UNIFORM = 0b01,	//Uniform buffer in dedicated GPU memory
		LUX_ALLOC_TYPE_SHARED_STORAGE =/**/0b10,	//Storage buffer in shared RAM memory
		LUX_ALLOC_TYPE_SHARED_UNIFORM =/**/0b11,	//Uniform buffer in shared RAM memory
		LUX_ALLOC_TYPE_NUM							//Number of LUX_ALLOC_TYPE values
	};

	struct MemBuffer {
		VkBuffer buffer;				//The actual Vulkan buffer
		VkDeviceMemory memory;			//The memory of the buffer
		Map<bool, uint32> cells;		//The cells in the buffer
	};
	struct MemBufferType {
		CellClass cellClass;			//The class of the cells
		AllocType allocType;			//The buffer allocation type
		Map<MemBuffer, uint32> buffers;	//Buffers containing the cells
	};

	// |--------------------------------- VRAM -------------------------------|
	// |------- Buffer0 ------||------- Buffer1 ------||------- Buffer2 ------|
	// |-cell-||-cell-||-cell-|
	//A video memory cell
	//A cell is a fixed-size partition of memory inside an allocated GPU buffer
	//Allocate a cell with the lux::rem::alloc function
	struct Cell {
		uint64 cellSize;			//Size of the cell in bytes
		//void* address;			//Address of the cell. The same as you would get with malloc
		MemBufferType* bufferType;	//Type of buffer allocation
		MemBuffer* buffer;			//Index of the buffer where the cell is allocated
		uint32 cellIndex;			//Index of the cell in the buffer
		//uint32 __padding__;

		//void operator = (Cell&& pCell){ _mm256_stream_si256((__m256i*)this, _mm256_stream_load_si256((__m256i*) & pCell)); }
		//TODO shared pointer
	};



	static constexpr inline bool isUniform(const AllocType vAllocType) {
		return (vAllocType & 0b1);
	}
	static constexpr inline bool isShared(const AllocType vAllocType) { return ((vAllocType >> 1) & 0b1); }
	//Returns an index based on the cell class
	#define _case(n) case LUX_CELL_CLASS_##n: return LUX_CELL_CLASS_INDEX_##n;
	static constexpr inline uint32 getCellClassIndex(const CellClass vClass){ switch(vClass){ _case(A) _case(B) _case(C) _case(D) _case(Q) _case(L) _case(0) default: return (uint32)-1; } }
	#define _case2(n) case LUX_CELL_CLASS_INDEX_##n: return LUX_CELL_CLASS_##n;
	static constexpr inline CellClass getCellClassValue(const uint32 vClass){ switch(vClass){ _case2(A) _case2(B) _case2(C) _case2(D) _case2(Q) _case2(L) _case2(0) default: return (CellClass)-1; } }




	extern uint32 maxAlloc;						//The maximum number of allocated buffers. Depends on the gpu properties
	extern Array<MemBufferType> buffers;		//VRAM allocated buffers


	static inline void init( ){
		//Set max allocation count and resize buffer types array
		maxAlloc = lux::core::dvc::compute.PD.properties.limits.maxMemoryAllocationCount;
		buffers.resize(LUX_CELL_CLASS_NUM * LUX_ALLOC_TYPE_NUM);

		//Init buffer types
		uint32 index;
		for(uint32 i = 0; i < LUX_CELL_CLASS_NUM; ++i){
			for(uint32 j = 0; j < LUX_ALLOC_TYPE_NUM; ++j){
				index = (i << 2) | j;
				buffers[index].cellClass = (CellClass)getCellClassValue(i);
				buffers[index].allocType = (AllocType)j;
				//32 buffers per chunk, max 4096 buffers (max allocation limit in GPUs)
				buffers[index].buffers = Map<MemBuffer, uint32>(32, 4096);
			}
		}
	}


	Cell alloc(const uint64 vSize, const CellClass vCellClass, const AllocType vAllocType);
	void* map(Cell& pCell);
	void free(Cell& pCell);

	//Generates the index of a buffer from the cell class and allocation type
	// 1 0 1 | 0 1
	// class | type
	constexpr uint32 genBufferTypeIndex(const CellClass vClass, const AllocType vAllocType){
		return (getCellClassIndex(vClass) << 2) | vAllocType;
	}
	static inline CellClass getCellClass(const Cell& pCell){ return pCell.bufferType->cellClass; }
	static inline uint32 getCellOffset(const Cell& pCell){ return getCellClass(pCell) * pCell.cellIndex; }
	static inline VkDeviceMemory getCellMemory(const Cell& pCell){ return pCell.buffer->memory; }
}
