

//regex per contare le linee di codice non vuote
//^(?!(\s*\*))(?!(\s*\-\-\>))(?!(\s*\<\!\-\-))(?!(\s*\n))(?!(\s*\*\/))(?!(\s*\/\*))(?!(\s*\/\/\/))(?!(\s*\/\/))(?!(\s(using))).*$


//TODO add memory pool

#include <time.h>

#include "LuxEngine/LuxEngine.h"
#include "LuxEngine/Engine/Input/Input.h"


//namespace lux::obj {
//	using h = Line2D;
//}


int h(const char* b, int hh) {
	printf("\n%s %d \n",b, hh);
	return 0;
}


void hg(lux::Array<uint16>){
	printf("\nKEYYYY");
}


int main() {
	LuxInit(false);
	//sleep(1000);


	vec2i32 hhh = {10, 9 };
	hhh *= 0.5;


	{
		lux::obj::Line2D lineTest;
		lineTest.col0 = vec4f32{ 1.0f, 0.1f, 0.0f, 1.0f };
		lineTest.col1 = vec4f32{ 0.0f, 0.2f, 1.0f, 0.0f };
		lineTest.wd0 = 100;
		lineTest.wd1 = 200;
		lineTest.p0 = vec2i32{ 2000, 500 };
		lineTest.p1 = vec2i32{ 1700, 800 };

		lux::obj::Line2D lineTest2;
		lineTest2.col0 = vec4f32{ 1.0f, 0.1f, 0.0f, 1.0f };
		lineTest2.col1 = vec4f32{ 0.0f, 0.2f, 1.0f, 0.0f };
		lineTest2.wd0 = 100;
		lineTest2.wd1 = 200;
		lineTest2.p0 = vec2i32{ 2000, 500 };
		lineTest2.p1 = vec2i32{ 1700, 800 };


		lux::obj::RenderSpace2D renderSpace(lux::obj::AlignmentType::FixedHorizontal, 2);
		renderSpace.addChild(&lineTest);
		renderSpace.addChild(&lineTest2);

		//TODO
		Engine& engine_ = lux::getEngine( );
		engine_.cshaderNew(lux::Array<LuxCell>{ engine_.gpuCellWindowOutput, engine_.gpuCellWindowSize, lineTest.gpuCell }, (engine_.shaderPath + lineTest.shaderName + ".comp.spv").begin( ), 4, 1, 1);
		engine_.cshaderNew(lux::Array<LuxCell>{ engine_.gpuCellWindowOutput, engine_.gpuCellWindowSize, lineTest2.gpuCell }, (engine_.shaderPath + lineTest.shaderName + ".comp.spv").begin( ), 4, 1, 1);
		lux::obj::addRenderSpace(&renderSpace);
	}


	lux::input::InputState is({
		lux::input::KeySequence{ LUX_KEY_W | lux::input::LuxKeyState::LUX_PRESS, hg },
		lux::input::KeySequence{ LUX_KEY_A | lux::input::LuxKeyState::LUX_PRESS, hg }
		});
	lux::input::setInputState(&is);


	srand(time(NULL));
	lux::thr::sendToExecQueue(h, lux::thr::LUX_PRIORITY_MAX, "UwU EKUSUPUROSION", 12345);
	lux::thr::sendToExecQueue(h, lux::thr::LUX_PRIORITY_MAX, "UwU EKUSUPUROSION", 12345);
	while(lux::getEngine( ).running) {
		//lux::thr::sendToExecQueue(h, lux::thr::LUX_PRIORITY_MAX, "UwU EKUSUPUROSION", 12345);
		//sleep(rand( ) % 20);
		sleep(10);
	}
	return 0;
}


//TODO ! la memoria pre allocata dei buffer della GPU non distingue memoria locale e condivisa