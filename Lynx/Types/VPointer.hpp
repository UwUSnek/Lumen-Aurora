#pragma once
#define LNX_H_VPOINTER
#include "Lynx/Core/Memory/VRam/VCell_t.hpp"
#include "Lynx/Core/Render/Buffers.hpp"
#include "Lynx/Core/Devices.hpp"
#include "Lynx/System/SystemMacros.hpp"
#include "Lynx/Tests/StructureInit.hpp"
#include "Lynx/Types/Dummy.hpp"
#include "cstring"








namespace lnx{
	enum bufferType{
		eStorage,
		eUniform
	};
	enum bufferLocation{
		eRam,
		eVRam
	};
}




namespace lnx::vram{
	//ptr base class
	template<class tType> struct Alloc_b {
		genInitCheck;

		uint8 loc;							//Runtime template data
		uint8 btype;						//Runtime template data
		Cell_t2* cell;						//A pointer to a lnx::vram::Cell_t object that contains the cell informations
		tType* mapped _dbg(= nullptr);		//A pointer used to map the memory

		template<class type_> explicit alwaysInline operator Alloc_b<type_>&() const noexcept { return *(Alloc_b<type_>*)(this); }				//Cast to different type
		template<class type_> explicit alwaysInline operator const Alloc_b<type_>&() const noexcept { return *(const Alloc_b<type_>*)(this); }	//Cast to different type
	};




	/**
	 * @brief A pointer that allocates memory accessible from the GPU
	 * @tparam tType The type of data pointed by the pointer (int, float, etc...)
	 * @tparam loc Where to allocate the memory. Can be Ram or VRam. VRam allocations are not accessible from the CPU
	 * @tparam btype //TODO
	 */
	template<class tType, bufferLocation loc, bufferType btype> struct ptr : public Alloc_b<tType> {
	private:
		using Super = Alloc_b<tType>;


		static alwaysInline void checkAllocSize(uint64 size_, VCellClass _class) { _dbg(
			if((_class) != VCellClass::e0 && _class != VCellClass::eAuto) {
				dbg::checkCond(size_ > 0xFFFFffff, "Allocation size cannot exceed 0xFFFFFFFF bytes. The given size was %llu", size_);
				dbg::checkCond((uint64)_class < size_, "%lu-bytes class specified for %llu-bytes allocation. The cell class must be large enought to contain the bytes. %s", (uint64)_class, size_, "Use lnx::VCellClass::AUTO to automatically choose it");
			}
		)}
		alwaysInline void checkMapped() const { _dbg(
			dbg::checkCond(!Super::mapped, "Unable to call this function on unmapped memory blocks");
		)}


		constexpr static void evaluateCellClass(const uint64 vSize, VCellClass& pClass) noexcept {
			if(pClass == VCellClass::eAuto) { [[likely]]
				     if(vSize <= (uint32)VCellClass::eA) [[likely]]   pClass = VCellClass::eA;
				else if(vSize <= (uint32)VCellClass::eB) [[likely]]   pClass = VCellClass::eB;
				else if(vSize <= (uint32)VCellClass::eC) [[likely]]   pClass = VCellClass::eC;
				else if(vSize <= (uint32)VCellClass::eD) [[likely]]   pClass = VCellClass::eD;
				else if(vSize <= (uint32)VCellClass::eQ) [[unlikely]] pClass = VCellClass::eQ;
				else if(vSize <= (uint32)VCellClass::eL) [[unlikely]] pClass = VCellClass::eL;
				else										 			   pClass = VCellClass::e0;
			}
		}




	public:
		// Constructors ------------------------------------------------------------------------------------------------------------------//




		/**
		 * @brief Creates a nullptr ptr.
		 *		The pointer will need to be initialized with the .realloc function before accessing its data
		 */
		alwaysInline ptr() { Super::loc = loc; Super::btype = btype; Super::cell = nullptr; }
		alwaysInline ptr(const std::nullptr_t) : ptr() {}



		/**
		 * @brief Copy constructor. This function only copies the pointer structure. The 2 pointers will share the same memory
		 * @param pPtr The pointer to copy
		 */
		alwaysInline ptr(const ptr<tType, loc, btype>& vAlloc) :
			ptr(vAlloc, Dummy{}) {
		}

		/**
		 * @brief Creates a pointer by copying another pointer's address.
		 *		This function only copies the pointer structure. The 2 pointers will share the same memory
		 * @param pPtr The pointer to copy
		 */
		template<class aType> explicit inline ptr(const ptr<aType, loc, btype>& vAlloc, const Dummy vDummy = Dummy{}){
			isInit(vAlloc);
			Super::loc = loc; Super::btype = btype;
			Super::cell = vAlloc.cell;
		}




		/**
		 * @brief Move constructor
		 */
		inline ptr(ptr<tType, loc, btype>&& vAlloc) {
			isInit(vAlloc);
			Super::loc = loc; Super::btype = btype;
			Super::cell = vAlloc.cell; //vAlloc.cell = &dummyCell;
			//!                          ^ Don't reset the vAlloc cell. It's required to decrement the owners count in vAlloc destructor
		}




		/**
		 * @brief Allocates a block of memory without initializing it
		 * @param vSize  Size of the block in bytes. It must be less than 0xFFFFFFFF
		 * @param vClass Class of the allocation. Default: AUTO
		 */
		inline ptr(const uint64 vSize, VCellClass vClass = VCellClass::eAuto) {
			Super::loc = loc; Super::btype = btype;
			evaluateCellClass(vSize, vClass); checkAllocSize(vSize, vClass);
			alloc_(vSize, vClass);
		}




		// Assignment --------------------------------------------------------------------------------------------------------------------//




		//Move assignment
		alwaysInline void operator=(ptr<tType, loc, btype>&& vAlloc) {
			operator=((ptr<tType, loc, btype>&)vAlloc);
		}


		//Copy assignment
		alwaysInline void operator=(const ptr<tType, loc, btype>& vAlloc) {
			checkInit(); isInit(vAlloc);
			Super::cell = vAlloc.cell; //vAlloc.cell = nullptr;
			//!                          ^ //TODO
		}


		alwaysInline void operator=(const std::nullptr_t) { Super::cell = nullptr; }




		// Get element -------------------------------------------------------------------------------------------------------------------//




		//Returns a reference to the element at index vIndex. This function can only be called on mapped pointers
		alwaysInline tType& operator[](const uint64 vIndex) const {
			checkInit(); checkMapped();
			dbg::checkIndex(vIndex, 0, count() - 1, "vIndex");
			return ((tType*)(Super::mapped))[vIndex];
		}

		//Returns a reference to the first element. This function can only be called on mapped pointers
		alwaysInline tType& operator*()  const { checkMapped(); checkInit(); return *((tType*)(Super::mapped)); }
		alwaysInline tType* operator->() const { checkMapped(); checkInit(); return   (tType*)(Super::mapped);  }


		/**
		 * @brief Returns the first address of the allocated memory block.
		 *		This function can only be called on mapped pointers
		 */
		alwaysInline tType* begin() const {
			checkInit(); checkMapped();
            return (tType*)Super::mapped;
		}

		/**
		 * @brief Returns the address of the object past the last object in the memory block.
		 *		Dereferencing the pointer is undefined behaviour.
		 *		This function can only be called on mapped pointers
		 */
		alwaysInline tType* end() const {
			checkInit(); checkMapped();
			return (tType*)((int8*)Super::mapped + count() * sizeof(tType));
		}




		// Count and size ----------------------------------------------------------------------------------------------------------------//




		//Returns the size in BYTES of the allocate memory. use count() to get the number of elements
		alwaysInline uint64 size()  const noexcept { return Super::cell->cellSize; }
		//Returns the number of complete elements in the allocated memory
		alwaysInline uint64 count() const noexcept { return Super::cell->cellSize / sizeof(tType); }




		// Reallocation ------------------------------------------------------------------------------------------------------------------//








	private:
		static consteval vk::BufferUsageFlags _usage() {
			if(btype == eUniform) return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer;
			else                 return vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc;
		}
		static consteval vk::MemoryPropertyFlags _prop() {
			// if(loc == Ram) return VK_MEMORY_PROPERTY_HOST_ACHED_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			if(loc == eRam) return vk::MemoryPropertyFlagBits::eHostVisible;
			else           return vk::MemoryPropertyFlagBits::eDeviceLocal; //FIXME IDK
		}


		void alloc_(const uint64 vSize, const VCellClass vClass) {
			using namespace lnx::__pvt;
			//FIXME WRITE USER INTERFACE

			cells_m.lock();
			const auto cellIndex = cells.add(Cell_t2{});					//Save cell index
			cells_m.unlock();
			Super::cell = &cells[cellIndex];								//Update cell pointer
			uint16 typeIndex = (vClass == VCellClass::e0) ? (uint16)-1 : ((__pvt::classIndexFromEnum(vClass) << 2) | (loc << 1) | btype);
			*Super::cell = Cell_t2{											//Update cell data
				.typeIndex = typeIndex,											//Set cell type index
				.cellIndex  = cellIndex,										//Set cell index
				.cellSize = (uint32)vSize										//Set size specified in function call
			};


			if((uint32)vClass) {											//For fixed class cells
				auto& type_ = types[typeIndex];									//Cache buffer type
				type_.m.lock();
				const auto localIndex = type_.cells.add(true);					//Create a new allocation and save its index
				type_.m.unlock();
				Super::cell->localIndex = localIndex;							//Save local index in cell object
				Super::cell->localOffset = (vClass != VCellClass::e0) * localIndex * (uint32)vClass;

				const uint32 buffIndex = localIndex / type_.cellsPerBuff;		//Cache buffer index and allocate a new buffer, if necessary
				if(!type_.memory[buffIndex].memory) {
					//!                     ^ Vulkan structures, but they are set to nullptr and treated as pointers, when not used
					lnx::core::buffers::createBuffer(
						&type_.memory[buffIndex].buffer, _usage(),
						__pvt::buffSize,
						&type_.memory[buffIndex].memory, _prop(),
						core::dvc::graphics.ld
					);
				}
				Super::cell->csc.buffer = type_.memory[buffIndex].buffer;
				Super::cell->csc.memory = type_.memory[buffIndex].memory;
			}
			else {															//For custom size cells
				uint64 size = (vSize / __pvt::incSize + 1) * __pvt::incSize;	//Calculate the new size and allocate a new buffer
				//FIXME USE ARBITRARY RANGE FOR COMPATIBILITY
				dbg::checkParam(btype == bufferType::eUniform && core::dvc::graphics.pd.properties.limits.maxUniformBufferRange >= vSize, "vSize", "Allocation is too large to be a uniform buffer");
				lnx::core::buffers::createBuffer(
					&Super::cell->csc.buffer, _usage(),
					size,
					&Super::cell->csc.memory, _prop(),
					core::dvc::graphics.ld
				);
				_dbg(Super::cell->localIndex = 0;)
			}
		}
		//TODO CHECK MEMORY FULL
	public:







		void realloc(const uint64 vSize, const bool vCopyOldData = true, VCellClass vClass = VCellClass::eAuto){
			// static std::mutex test_m;
			// test_m.lock();
			checkInit(); checkAllocSize(vSize, vClass);
			evaluateCellClass(vSize, vClass);

			if(!Super::cell) alloc_(vSize, vClass);
			else{
				dbg::checkCond(Super::mapped, "realloc() can only be called on unmapped memory");
				free();
				alloc_(vSize, vClass);
				//FIXME write a proper realloc function
			}
			// test_m.unlock();
		}
		// /**
		//  * @brief Reallocates the pointer to a block of memory of vSize bytes without initializing it
		//  * @param vSize  Size of the block in bytes. It must be positive and less than 0xFFFFFFFF
		//  * @param vCopyOldData If true, copies the old data when the memory block is changed
		//  * @param vClass Class of the allocation. It must be a valid lnx::VCellClass value. Default: AUTO
		//  */
		// void realloc(const uint64 vSize, const bool vCopyOldData = true, VCellClass vClass = VCellClass::AUTO) {
		// 	using namespace lnx::__pvt;
		// 	checkInit(); checkAllocSize(vSize, vClass);
		// 	evaluateCellClass(vSize, vClass);


		// 	// if(!cell->address) { [[unlikely]]									//If the memory is unallocated
		// 		// alloc_(vSize, vClass);												//Allocate it
		// 		// //!^ Ok. Owners count is set back to 1 if the address was nullptr. nullptr allocations are not shared
		// 	// }
		// 	// else { [[likely]]														//If it's allocated
		// 		if(																	//And the new size is smaller or equal to the maximum cell size
		// 			((uint32)vClass && vSize <= (int64)vClass) || (!(uint32)vClass && vSize <= (vSize / incSize) * incSize)) { //FIXME USE DIFFERENT INCREMENT FOR GPU CELLS
		// 			//! ^ Not (vSize / incSize + 1)
		// 			[[unlikely]] cell->cellSize = vSize;								//change the cellSize variable and return //FIXME move to fixed size cell
		// 		}
		// 		else { [[likely]]												//If it's larger than the maximum cell size //TODO check realloc and free returns
		// 			if(cell->typeIndex != (uint16)-1) {								//If the cell is a fixed size cell
		// 				type* oldAddr = (type*)cell->address;							//Save the old address
		// 				types[cell->typeIndex].m.lock();
		// 				types[cell->typeIndex].cells.remove(cell->localIndex);			//Remove old allocation
		// 				types[cell->typeIndex].m.unlock();
		// 				//! ^ this doesn't remove or invalidate the cell object but only the buffer's cell tracker. This is to allow other pointers to use the same cell even after a reallocation
		// 				if((uint32)vClass) {											//Fixed size --> fixed
		// 					auto& type_ = types[cell->typeIndex];							//Cache buffer type
		// 					type_.m.lock();
		// 					cell->localIndex = type_.cells.add(true);						//Create a new allocation and save its index. Then set the new address
		// 					type_.m.unlock();
		// 					cell->address = (char*)type_.memory[cell->localIndex / type_.cellsPerBuff] + (uint64)type_.VCellClass * cell->localIndex;
		// 				}
		// 				else {															//Fixed size --> custom
		// 					uint64 size_ = (vSize / incSize + 1) * incSize;			//Calculate the new size and allocate the new memory
		// 					cell->address = _wds(_aligned_malloc(size_, memAlign)) _lnx(aligned_alloc(memOffset, size_));
		// 				}
		// 				// if(vCopyOldData) memcpy(cell->address, oldAddr, cell->cellSize);//Copy old data in the new memory //FIXME COPY
		// 				//! ^ The cell still has the same size as before, so it's ok to use it to copy the old data
		// 				cell->typeIndex = classIndexFromEnum(vClass);					//Set the new type index
		// 			}
		// 			else {																//Custom size --> custom
		// 				//FIXME use normal malloc if the data doesnt need to be copied
		// 				cell->address = std::realloc(cell->address, vSize);					//Just reallocate the pointer
		// 			}
		// 			cell->cellSize = vSize;											//Set the new cell size
		// 			//! Custom size --> fixed is managed in smaller size case
		// 		}
		// 	// }
		// }




		// /**
		//  * @brief Reallocates the pointer to a block of memory containing vCount elements without initializing them
		//  * @param vCount Number of elements
		//  * @param vCopyOldData If true, copies the old data when the memory block is changed
		//  * @param vClass Class of the allocation. It must be a valid lnx::VCellClass value. Default: AUTO
		//  */
		// alwaysInline void reallocArr(const uint64 vCount, const bool vCopyOldData = true, const VCellClass vClass = VCellClass::AUTO) {
		// 	checkInit(); checkAllocSize(sizeof(type) * vCount, vClass);
		// 	realloc(sizeof(type) * vCount, vCopyOldData, vClass);
		// }




		// Free, map and unmap memory ----------------------------------------------------------------------------------------------------//




		//TODO add free function to erase memory contents
		/**
		 * @brief Frees the memory block owned by the pointer.
		 *		The memory will become invalid and unaccessible for any other pointer currently using it
		 */
		inline void free() {
			dbg::checkCond(Super::mapped, "free() can only be called on unmapped memory");
            if(Super::cell->typeIndex != (uint16)-1) {										//For fixed  size cells
                types[Super::cell->typeIndex].m.lock();
                types[Super::cell->typeIndex].cells.remove(Super::cell->localIndex);			//free the allocation object
                types[Super::cell->typeIndex].m.unlock();
				//FIXME FREE BUFFERS
            }
			else {																			//For custom size cells
				core::dvc::graphics.ld.freeMemory   (Super::cell->csc.memory, nullptr);			//Free the memory
				core::dvc::graphics.ld.destroyBuffer(Super::cell->csc.buffer, nullptr);			//Destroy the vulkan buffer object
			}

            cells_m.lock();
            cells.remove(Super::cell->cellIndex);											//Free the cell object
            cells_m.unlock();
		}




		//FIXME SPECIALIZE RAM ALLOCATIONS
		/**
		 * @brief Maps the memory block to a RAM pointer, allowing the CPU to access its data through operator[], operator->, operator*, begin() and end() functions.
		 *		The pointer MUST be unmapped by the same thread before the memory is freed.
		 *		This function will automatically invalidate the host cache and flush the data from the GPU
		 */
		void map(){
			dbg::checkCond(Super::mapped, "Memory block mapped twice");
			dbg::checkCond(!size(), "Cannot map memory blocks with size 0");
			auto memory = Super::cell->csc.memory;
			auto offset = Super::cell->localOffset;
			auto size   = Super::cell->cellSize;
			if(size % __pvt::incSize) size = (size / __pvt::memOffset + 1) * __pvt::memOffset;

			core::dvc::graphics.ld.mapMemory(memory, offset, size, 0, (void**)&(Super::mapped));
			const auto range = vk::MappedMemoryRange()
				.setMemory (memory)
				.setOffset (offset)
				.setSize   (size)
			;
			core::dvc::graphics.ld.invalidateMappedMemoryRanges(1, &range); //TODO this seems useless
		}


		/**
		 * @brief Unmaps the memory block and flushes the data to the GPU.
		 *		This function can only be called on mapped blocks
		 */
		void unmap(){
			dbg::checkCond(!Super::mapped, "unmap() called on unmapped memory");
			auto memory = Super::cell->csc.memory;
			auto offset = Super::cell->localOffset;
			auto size   = Super::cell->cellSize;
			if(size % __pvt::incSize) size = (size / __pvt::memOffset + 1) * __pvt::memOffset;

			const auto range = vk::MappedMemoryRange()
				.setMemory (memory)
				.setOffset (offset)
				.setSize   (size)
			;
			core::dvc::graphics.ld.flushMappedMemoryRanges(1, &range); //TODO this seems useless
			core::dvc::graphics.ld.unmapMemory(memory);
			_dbg(Super::mapped = nullptr);
		}




		// Comparison and conversion operators -------------------------------------------------------------------------------------------//




		alwaysInline operator tType*() const { checkInit(); return (tType*)Super::cell; }
		alwaysInline operator bool()  const { checkInit(); return !!Super::cell;      }

		alwaysInline bool operator==(ram::ptr<tType> vPtr) { return vPtr.cell == Super::cell; }
		alwaysInline bool operator!=(ram::ptr<tType> vPtr) { return vPtr.cell != Super::cell; }
		//! If they have the same cell, they also have he same address. No need to access it
	};
}