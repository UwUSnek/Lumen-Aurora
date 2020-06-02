
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



#include "LuxEngine/LuxEngine.h"


int main() {
	//luxInit(false);

	//Frame{
	//	Sleep(1);
	//}

	LuxArray<float> a{ 0, 5, 2 };

	LuxArray<uint32> b(&a);


	printf("%d", b[1]);
	return 0;
}