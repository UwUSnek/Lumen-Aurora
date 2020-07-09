#pragma once

#ifndef  __INPUT	//This is useless, but it doesn't work without it
#define __INPUT
#include "InputState.h"
#include "GLFW/glfw3.h"                           // for GLFWwindow, GLFW_REPEAT
#include "LuxEngine/macros.h"                     // for scast
#include "LuxEngine/Types/Containers/LuxArray.h"  // for LuxArray
#include "LuxEngine/Types/Integers/Integers.h"    // for uint16
#include "stdio.h"                                // for printf
#include "LuxEngine/Engine/Engine.h"
#include "KeyboardInput.h"


extern LuxInputState* __lp_input_states;		//The current input state
inline static void luxInputSetInputState(LuxInputState* inputState) { __lp_input_states = inputState; __lp_input_states->sort(); }







static void __lp_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).p0 = vec2int32{ (int32)x, (int32)y };
	//printf("mouse");
}




static void __lp_mouseWheelCallback(GLFWwindow* window, double x, double y) {
	*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).wd0 -= y * 10;
	*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).wd1 -= y * 10;
}




static void __lp_mouseCursorPosCallback(GLFWwindow* window, double x, double y) {
	////renderFence.wait(1);
	*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).p1 = vec2int32{ (int32)x, (int32)y };
	////renderFence.set(0);
}














//TODO add key seqeuence tree


//This function manages the input from the keyboard and calls the functions binded to the input state key bindings
static void __lp_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == LUX_KEY_KP_SUBTRACT) (*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col0).w = max((*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col0).w - 0.05, 0);
	if (key == LUX_KEY_KP_SUBTRACT) (*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col1).w = max((*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col1).w - 0.05, 0);
	if (key == LUX_KEY_KP_ADD) if ((*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col0).w < 1) *(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col0 += vec4float32(0, 0, 0, 0.05);
	if (key == LUX_KEY_KP_ADD) if ((*(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col1).w < 1) *(*(rcast<LuxDynamic_LuxObjectLineCCT*>(engine.objs[0]))).col1 += vec4float32(0, 0, 0, 0.05);




	//FULL SCREEN
	static int fsstate = 0;
	static int wmx, wmy, wmw, wmh;
	if (key == LUX_KEY_F11 && action == GLFW_PRESS) {
		int mx, my, mw, mh;
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &mx, &my, &mw, &mh);
		if (fsstate == 0) {
			glfwGetWindowSize(window, &wmw, &wmh);
			glfwGetWindowPos(window, &wmx, &wmy);
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), mx, my, mw, mh, GLFW_DONT_CARE);
			fsstate = 1;
		}
		else if (fsstate == 1) {
			glfwSetWindowMonitor(window, NULL, wmx, wmy, wmw, wmh, GLFW_DONT_CARE);
			glfwRestoreWindow(window);
			fsstate = 0;
		}
	}




	// 37th hour spent debugging this function
	static uint16 yMin = 0, yMax = scast<uint16>(__lp_input_states->sequences.size() - 1), x = 0;
	#define __lp_to_lux_act(glfwAction) ((uint16)1 << (16 - glfwAction - 1))

	if (action != GLFW_REPEAT) {										//If the action is not repeat
		uint16 keyCode = (key | __lp_to_lux_act(action));					//Calculate the key code
		int i = yMin;														//Set the loop index as the minimum y
		while (keyCode != __lp_input_states->sequences[i].sequence[x]) {	//Find the new minimum y
			++i;																//Increase the counter until the input key is equal to the key of the input state sequence 
			if (i > yMax) {														//If there are no equal keys
				x = 0;																//Reset the key counter
				return;																//Exit the function
			}
		}
		yMin = i;															//If there is an equal key, set the minimum y

		while (true) {														//Now find the maximum y 
			if (keyCode == __lp_input_states->sequences[i].sequence[x]) {		//if the input key is NOT equal to the key of the input state sequence
				++i;																//Increase the counter
				if (i >= __lp_input_states->sequences.size()) {						//If there are no more different keys
					yMax = scast<uint16>(__lp_input_states->sequences.size() - 1);		//Set the maximum y as the maximum index of the sequences
					break;																//Exit the loop
				}
			}
			else {																	//If there are different keys
				yMax = i - 1;															//Set the maximum y
				break;																	//Exit the loop
			}
		}
																												// UwU
		if (yMin == yMax && x == scast<LuxArray<uint16>>(__lp_input_states->sequences[yMax].sequence).size() - 1) {	//If the maximum and minimum y are the same
			__lp_input_states->sequences[yMax].bindedFunction(__lp_input_states->sequences[yMax].sequence);			//Call the binded function
			yMin = 0;																								//Reset the minimum y
			yMax = scast<uint16>(__lp_input_states->sequences.size() - 1);												//Reset the maximum y
			x = 0;																									//Reset the x
			return;																									//Exit the function
		}
		x++;																									//Update the input key index
	}
}




#endif //! __INPUT
