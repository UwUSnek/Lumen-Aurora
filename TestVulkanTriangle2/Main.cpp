﻿
//^(?!(\s*\*))(?!(\s*\-\-\>))(?!(\s*\<\!\-\-))(?!(\s*\n))(?!(\s*\*\/))(?!(\s*\/\*))(?!(\s*\/\/\/))(?!(\s*\/\/))(?!(\s(using))).*$


/*
	LD 0
		queue 0
			command pool 0
				command buffer 0
					command 0
					command 1
					command 2
					...
				...
			...
		...
	LD 1
		...
	...
*/


#ifndef NDEBUG
#define LUX_DEBUG
#endif

#include "LuxEngine/LuxEngine.h"
#include <filesystem>
#include <iostream>

void hh(LuxArray<uint16> keySequenceCode) { printf("AAAAA"); }
void hh2(LuxArray<uint16> keySequenceCode) { printf("BBBB"); }
void hh3(LuxArray<uint16> keySequenceCode) { printf("CCC"); }
void hh4(LuxArray<uint16> keySequenceCode) { printf("DD"); }


int main() {
	float b;
	//for (uint64_t j = 0; j < 286000; j++) {
	//	for (uint64_t i = 0; i < 350; i++) {
	//		b = FFTan(i);
	//	}
	//}
	for (uint64_t i = 0; i < 100100100; i++) {
		b = FTan((float)i);
	}
	sleep(1000);
	int hhh = 0;
	//LuxCell h;
	//h = __lp_cellCode(8, 2, 2000000);
	//
	//h = __lp_cellCode(6, 2, 500000);
	//h = __lp_cellCode(4, 1, 10000);
	//h = __lp_cellCode(7, 199, 500000);
	////std::string path = "./";
	////for (const auto& entry : std::filesystem::directory_iterator(path))
	////	std::cout << entry.path() << std::endl;

	//LuxInit(false);

	//
	//LuxInputState inputStateTest{
	//	LuxKeySequence{ {LUX_KEY_W | LUX_PRESS}, &hh },
	//	LuxKeySequence{ {LUX_KEY_S | LUX_PRESS, LUX_KEY_S | LUX_RELEASE}, &hh2 } ,
	//	LuxKeySequence{ {LUX_KEY_S | LUX_PRESS, LUX_KEY_A | LUX_PRESS, LUX_KEY_A | LUX_RELEASE}, &hh3 },
	//	LuxKeySequence{ {LUX_KEY_S | LUX_PRESS, LUX_KEY_A | LUX_PRESS, LUX_KEY_D | LUX_PRESS}, &hh4 }
	//};
	//luxInputSetInputState(&inputStateTest);


	//while (engine->running) {
	//	printf("%lf\n", FCos(-573289));
	//	sleep(1);
	//	__noop;
	//}
	return 0;
}