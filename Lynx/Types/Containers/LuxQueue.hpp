#pragma once
#include "Lynx/Types/Containers/RtArray.hpp"
#include "Lynx/macros.hpp"
#include "Lynx/Types/Integers/Integers.hpp"
#include "Lynx/Tests/StructureInit.hpp"



//TODO automatic back to front and viceversa in queues
//TODO FIX QUEUES

//A queue with dynamic count
//DEPRECATED
namespace lnx {
	template<class tType, class tIndx = uint32> struct Queue {
		genInitCheck;
	private:
	public:
		lnx::RtArray<tType, tIndx> _front, _back;

		inline Queue() : _front(), _back() {}

		inline void clear() {
			_front.clear();
			_back.clear();
		}


		//Adds an element at the back of the queue
		inline void pushBack(const tType& vElement)	{ checkInit(); _back.add(vElement); }
		//Adds an element at the front of the queue
		inline void pushFront(const tType& vElement) { checkInit(); _front.add(vElement); }
		//Removes an element from the back of the queue
		inline void popBack(const tType& vElement)	{ checkInit(); _back.resize(_back.count() - 1); }
		//Removes an element from the front of the queue
		inline void popFront()						{ checkInit(); _front.resize(_front.count() - 1); }


		inline tType& operator[](const tIndx vIndex) { checkInit(); return (vIndex < _back.count()) ? _back[_back.count() - 1 - vIndex] : _front[vIndex - (_back.count() - 1)]; }
		inline tType& front(){ checkInit(); return *_front.end(); }
		inline tType& back()	{ checkInit(); return *_back.end(); }
		inline tIndx size()  { checkInit(); return _front.count() + _back.count(); }
		inline bool empty()	{ checkInit(); return size() == 0; }
	};
}
