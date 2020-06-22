

#pragma once
#include "LuxEngine/Types/Integers/Integers.h"
#include "LuxEngine/macros.h"
#include "LuxEngine/Types/Containers/LuxArray.h"
#include <vector>



//Lux map
//A 64-bit index map capable of containing billions of elements without losing performances
//The elements are not ordered, so each element has a unique ID. The ID is returned by add functions
//Use the isValid() function to check if an element can be used or not
template<class type>
class LuxMap {
private:
	uint64 chunksDynNum;			//Allocated number of chunks
public:
	uint64 __lp_dynSize;			//Allocated number of elements. Please use size() instead of this variable
	uint64 __lp_freeNum;			//Number of free elements in the map. Please use usedSize() or freeSize() instead of this variable
	type** __lp_data;				//Map elements
	uint64** __lp_tracker;			//Map that stores the state of each element

private:
	uint64 head;					//First free element
	uint64 tail;					//Last free element
	uint64 chunkSize;				//Size of the chunks
	uint64 maxSize;					//Maximum number of elements in the map

	#define __lp_Data(index) __lp_data[index / chunkSize][index % chunkSize]			//Get a data    element using only one index instead of index in the chunk and chunk index
	#define __lp_Tracker(index) __lp_tracker[index / chunkSize][index % chunkSize]		//Get a tracker element using only one index instead of index in the chunk and chunk index
public:




	// Constructor --------------------------------------------------------------------------------------------------------- //



	//Creates the map with the specified chunk and maximum size
	//The number of chunks depends on their size and the maximum size of the map (chunks = maxSize / chunkSize)
	//*   vChunkSize: number of elements allocated when the map grows. Default at 0xFF (256). It must be <= maxSize
	//*   vMaxSize: the maximum size the map can reach. Default at 0xFFFF (65535). Max 0x FFFF FFFF FFFF FFFF
	__vectorcall LuxMap(const uint64 vChunkSize = 0xFF, const uint64 vMaxSize = 0xFFFF) {
		chunkSize = vChunkSize;														//Set size of the chunks
		maxSize = vMaxSize;															//Set maximum size of the map
		__lp_data = (type**)malloc(sizeof(type*) * (maxSize / chunkSize));			//Allocate data map
		__lp_tracker = (uint64**)malloc(sizeof(uint64*) * (maxSize / chunkSize));	//Allocate tracker map
		chunksDynNum = __lp_dynSize = __lp_freeNum = 0;								//Initialize number of chunks, elements and free elements							
		head = tail = -1;															//Initialize head and tail
	}


	//Initializes the array using a container object and converts each element to the array type. The input container must have a begin() and an end() function
	//*   in: a pointer to the container object
	template<class elmType>
	__vectorcall LuxMap(const LuxContainer<elmType>* in) {
		for (int i = 0; i < in->end() - in->begin(); ++i) add((elmType) * (in->begin() + i));
	}





	// Add, remove --------------------------------------------------------------------------------------------------------- //




	//Adds an element at the end of the map, allocating a new chunk if needed
	//Returns the ID of the element
	uint64 __vectorcall append(const type vData) {
		if (__lp_dynSize + 1 > chunksDynNum * chunkSize) {								//If the chunk is full
			__lp_data[chunksDynNum] = (type*)malloc(sizeof(type) * chunkSize);				//Allocate a new data chunk
			__lp_tracker[chunksDynNum] = sc<uint64*>(malloc(sizeof(uint64*) * chunkSize));	//Allocate a new tracker chunk
			chunksDynNum++;																	//Update the number of chunks
		}
		__lp_Data(__lp_dynSize) = vData;												//Assign the data to the new element
		__lp_Tracker(__lp_dynSize) = -1;												//Set the tracker as valid

		__lp_dynSize++;																	//Update the number of elements
		return __lp_dynSize - 1;														//Return the ID
	}




	//Adds an element at the firs free index of the map
	//Returns the ID of the element
	uint64 __vectorcall add(const type vData) {
		uint64 head2 = head;
		if (head == -1) return append(vData);				//If it has no free elements, append it
		else {
			if (head == tail) {								//If it has only one free element
				__lp_Data(head2) = vData;						//Replace it
				head = tail = __lp_Tracker(head2) = -1;			//And reset head and tail
			}
			else {											//If it has more than one
				head = __lp_Tracker(head2);						//Update head
				__lp_Tracker(head2) = -1;						//Update the status of the first
				__lp_Data(head2) = vData;						//Replace it
			}
			__lp_freeNum--;									//Update the number of free elements
			return head2;
		}
	}




	//Adds the elements of an std::vector to the map, placing each element in the first free index
	//Returns a static array containing the IDs of the elements, in the same order as they were in the input
	LuxArray<uint64> __vectorcall add(const std::vector<type>* pVector) {
		LuxArray<uint64> IDs;
		IDs.resize(pVector->size());								//Set the number of IDs
		forEach(*pVector, i) IDs[i] = add((*pVector)[i]);			//Add every element to the map and save its ID
		return IDs;												//Return the IDs
	}




	//Adds the elements of a LuxContainer to the map, skipping the invalid ones and placing the others in the first free index
	//Returns a static array containing the IDs of the elements, in the same order as they were in the input (invalid indices have -1 as ID)
	LuxArray<uint64> __vectorcall add(const LuxMap<type>* vContainer) {
		LuxArray<uint64> IDs;
		IDs.resize(vContainer->size());								//Set the number of IDs
		forEach(*vContainer, i) {									//For every element of the input map
			if (vContainer->isValid(i)) IDs[i] = add((*vContainer)[i]);	//If it's valid, add it to the map and save its ID
			else IDs[i] = -1;											//If not, save -1 as ID
		}
		return IDs;													//Return the IDs
	}




	//Signs an index of the map as free, without deleting it. Returns a char for performance reasons
	//*   vIndex: the index to sign
	//*   freeElm: whether to free the element or keep it in memory. Keeping it saves performances but increases memory usage
	//*   Returns 0 if the operation succeeded, status(index) if not
	signed char __vectorcall remove(const uint64 vIndex, const bool vFreeElm = false) {
		signed char r = status(vIndex);

		if (r != 0) return r;								//If the index is not valid, return the error code
		else {												//If it is,
			__lp_Tracker(vIndex) = -1;							//Set the index as free
			if (vFreeElm) free(&__lp_Data(vIndex));				//Free the element if necessary
			if (head == sc<uint64>(-1)) head = tail = vIndex;		//If it has no free elements, initialize head and tail.
			else tail = __lp_Tracker(tail) = vIndex;			//If it has free elements, set the new tail and update the last free index
			__lp_freeNum++;										//Update the number of free elements
		}
		return 0;
	}




	//Sets the size of the map to 0, deleting all the elements and resetting it to the initial state
	inline void __vectorcall clear() {
		for (int32 i = 0; i < chunksDynNum; ++i) free(__lp_data[i]); free(__lp_data);		//Free data map
		for (int32 i = 0; i < chunksDynNum; ++i) free(__lp_tracker[i]); free(__lp_tracker);	//Free tracker map
		__lp_data = (type**)malloc(sizeof(type*) * (maxSize / chunkSize));					//Allocate data map
		__lp_tracker = sc<uint64**>(malloc(sizeof(uint64*) * (maxSize / chunkSize)));		//Allocate tracker map
		chunksDynNum = __lp_dynSize = __lp_freeNum = 0;										//Reset number of chunk, number of elements, number of free elements
		head = tail = -1;																	//Reset head and tail
	}




	// Status -------------------------------------------------------------------------------------------------------------- //




	//Returns 0 if the index is used, 1 if the index is free, -1 if the index is invalid or there is an error, -2 if the index is out of range
	inline signed char __vectorcall status(const uint64 vIndex) const {
		if (vIndex == (int64)-1)return -1;							//Invalid index
		else if (vIndex >= __lp_dynSize) return -2;					//Index out of range
		else if (__lp_Tracker(vIndex) == sc<uint64>(-1)) return 0;	//Ok
		else if (__lp_Tracker(vIndex) >= 0)return 1;				//Invalid element
		else return -1;												//Unknown error
	}


	//Returns true if the index is used, 1 if it's free or invalid (use the 'status' function for more details)
	inline bool __vectorcall isValid(const uint64 vIndex) const { return (__lp_dynSize > vIndex && __lp_Tracker(vIndex) == sc<uint64>(-1)); }




	// Get ----------------------------------------------------------------------------------------------------------------- //




	//Use the isValid() function to check if the element can be used
	inline type& __vectorcall operator[](const uint64 vIndex) const { return __lp_Data(vIndex); }
	//Returns a pointer to the first element of a chunk. The elements are guaranteed to be in contiguous order
	inline type* __vectorcall data(const uint64 vChunkIndex) const { return &__lp_data[vChunkIndex][0]; }

	//Returns a pointer to a new map that contains all the elements in the chunks, without the invalid ones
	//This operation can be really slow, try to avoid using it
	type* data() const {
		type* arr = (type*)malloc(sizeof(type) * usedSize());
		int32 new_i = 0;
		for (int32 i = 0; i < usedSize(); ++i) {
			if (isValid(i)) {
				arr[new_i] = __lp_Data(i);
				++new_i;
			}
			else ++i;
		}
		return arr;
	}




	// Size ---------------------------------------------------------------------------------------------------------------- //




	//Returns the number of elements in the map, including the free ones
	inline uint64 const size() const { return __lp_dynSize; }
	//Returns the number of used elements
	inline uint64 __vectorcall usedSize() const { return __lp_dynSize - __lp_freeNum; }
	//Returns the number of free elements
	inline uint64 freeSize() const { return __lp_freeNum; }
};

#undef __lp_Data
#undef __lp_Tracker
