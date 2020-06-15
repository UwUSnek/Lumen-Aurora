﻿#pragma once

#ifndef __ENGINE
#define __ENGINE

extern double* __lp_sin;
extern double* __lp_cos;
extern double* __lp_tan;
extern double* __lp_cot;
extern double* __lp_sec;
extern double* __lp_csc;
extern double* __lp_sinh;
extern double* __lp_cosh;
extern double* __lp_tanh;
extern double* __lp_coth;
extern double* __lp_sech;
extern double* __lp_csch;

#define FSin(degree) *(__lp_sin+(degree*1000)) //Fast sin function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FCos(degree) *(__lp_cos+(degree*1000)) //Fast cos function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FTan(degree) *(__lp_tan+(degree*1000)) //Fast tan function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FCot(degree) *(__lp_cot+(degree*1000)) //Fast cot function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FSec(degree) *(__lp_sec+(degree*1000)) //Fast sec function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FCsc(degree) *(__lp_csc+(degree*1000)) //Fast csc function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FSinh(degree) *(__lp_sinh+(degree*1000) //Fast sinh function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FCosh(degree) *(__lp_cosh+(degree*1000) //Fast cosh function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FTanh(degree) *(__lp_tanh+(degree*1000) //Fast tanh function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FCoth(degree) *(__lp_coth+(degree*1000) //Fast coth function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FSech(degree) *(__lp_sech+(degree*1000) //Fast sech function that uses a pre-calculated value to improve performance. 0.001 deg precision
#define FCsch(degree) *(__lp_csch+(degree*1000) //Fast csch function that uses a pre-calculated value to improve performance. 0.001 deg precision






//Disabled useless warnings
#pragma warning( disable : 26812 )			//Prefer enum class to enum
#pragma warning( disable : 26495 )			//Uninitialized variables
#pragma warning( disable : 26451 )			//stb arithmetic overflow

//Dark magic
#include <vulkan/vulkan.h>					//Graphics
#include <GLFW/glfw3.h>						//Window system
#define GLM_FORCE_RADIANS					//Use radiants intead of degrees
#define GLM_FORCE_DEPTH_ZERO_TO_ONE			//0 to 1 depth instead of OpenGL -1 to 1
#include <glm/glm.hpp>						//Shader compatible geometry

//#ifdef LUX_DEBUG //TODO remove

//Default C++
#include <iostream>
#include <fstream>
#include <array>
#include <chrono>
#include <vector>
#include <thread>
#include <set>
//#endif //TODO remove

#include <cmath>
#include <string>

//Junk
#include "LuxEngine/macros.h"				//Useful useless macros
#include <TermColor.hpp>					//Colored console output


//Structures
#include "Graphics/Structs/_VkPhysicalDevice.h"
#include "Graphics/Structs/Vertex.h"
#include "LuxEngine/Object/Object.h"
#include "LuxEngine/Types/Integers/Integers.h"
#include "LuxEngine/Types/Containers/LuxMap.h"
#include "LuxEngine/Types/Containers/LuxArray.h"
#include "LuxEngine/Types/EngineTypes.h"


#include "Input/Input.h"


//Re enable warnings for this header
#pragma warning( default : 26451 )			//Arithmetic overflow








//Structures and macros -------------------------------------------------------------------------------------------------------------//








//It's dark magic, idk why or how it works, but it does
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else return VK_ERROR_EXTENSION_NOT_PRESENT;
}
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) func(instance, debugMessenger, pAllocator);
}

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	LuxArray<VkSurfaceFormatKHR> formats;
	LuxArray<VkPresentModeKHR> presentModes;
};





//Dark magic
#define populateDebugMessengerCreateInfo(createInfo)\
	createInfo = {};\
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;\
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;\
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;\
	createInfo.pfnUserCallback = debugCallback;








// Engine class -----------------------------------------------------------------------------------------------------------------------------//





//         ENGINE CLASS STRUCTURE
//       
//         main()
//         ¦-- run()
//         ¦   │                                                   ___                ___
//         ¦   │-- initWindow()                                       │                  │
//         ¦   │   │-- glfwInit()                                     │                  │
//         ¦   │   │-- set window variable                            │ main             │
//         ¦   │   '-- set window resize callback                     │                  │
//         ¦   │                                                      │                  │
//         ¦   │-- createInstance()                                ___│                  │
//         ¦   │                                                                         │
//         ¦   │                                                   ___                   │
//         ¦   │-- runGraphics()                                      │                  │
//         ¦   │   '-- initVulkan()                                   │                  │
//         ¦   │       │-- createSurface()                            │                  │
//         ¦   │       │-- evalutate physical devices                 │                  │
//         ¦   │       │   │-- save every suitable device data        │                  │
//         ¦   │       │   '-- create logical devices                 │                  │
//         ¦   │       │                                              │                  │ INITIALIZE ENGINE
//         ¦   │       │-- createGraphicsCommandPool()                │                  │
//         ¦   │       │-- createDebugMessenger()                     │ graphics         │
//         ¦   │       │                                              │                  │
//         ¦   │       │-- createTextureImage()                       │                  │
//         ¦   │       │-- createTextureImageView()                   │                  │
//         ¦   │       │-- createTextureSampler()                     │                  │
//         ¦   │       │                                              │                  │
//         ¦   │       │-- createVertexBuffer()                       │                  │
//         ¦   │       │-- createIndexBuffer()                        │                  │
//         ¦   │       │-- ?                                       ___│                  │
//         ¦   │                                                   ___                   │
//         ¦   │-- runCompute()                                       │ compute          │
//         ¦   │   │-- Create image output buffer                     │                  │
//         ¦   │   │-- ?                                           ___│                  │
//         ¦   │                                                   ___                   │
//         ¦   │,- set GLFW keyboard callback                         │ input            │
//         ¦   │'- set GLFW mouse callback                         ___│               ___│
//         ¦   │
//         ¦   │
//         ¦   │  ////////////////////////////////////////////////////////////////////////////////////
//         ¦   │
//         ¦   │                                                   ___                ___
//         ¦   │-- mainLoop()                                         │                  │
//         ¦   │   │-- run fps counter                                │                  │
//         ¦   │   '---every frame                                    │ render loop      │
//         ¦   │   ^   │-- check GLFW events                          │                  │
//         ¦   │   │   │-- render and draw frame to window            │                  │ RUNNING
//         ¦   │   '---'                                           ___│                  │
//         ¦   │                                                                         │
//         '-----> ?                                               ___                   │
//             │                                                   ___│ external      ___│
//             │
//             │
//             │ /////////////////////////////////////////////////////////////////////////////////////
//             │
//             │                                                   ___
//             │,- cleanupGraphics()                                  │ 
//             │'- cleanupCompute()                                   │ 
//             │                                                      │ cleanup
//             │,- destroy instance                                   │
//             │'- destroy window                                     │
//             │                                                      │
//             '-- terminate GLFW                                  ___│



/*


																																														Frame render
   ↑↓<>-.'_─│¦                                                                                                            GPU MEMORY                                                         ↓
																							   ______________________________________________________________________________________________¦_________________________________________________________
																							  │ .────────────────────────────────────────────────────────────────────────────────────────────¦───────────────────────────────────────────────────────. │
   LUX OBJECT DATA MANAGEMENT                                                                 ││                                                                                             ↓                                                        ││
   This scheme represent a system with a single 1080p monitor and 48GB VRAM                   ││       Custom size allocations for large buffers                                          Shaders                   Output buffer (6.2208MB)          ││
																							  ││      .──────────────────────────.  .──────────────────────────.                  .─────────────────────.           Window 0                          ││
   all the buffers are saved as LuxMap s of buffer cellment index                             ││      | Custom size allocation 2 |  | Custom size allocation 3 >-.                │       Shader 0      <--.       .──────────────────────────.       ││
   and allocated in the GPU's memory.                                                         ││      '─────────↑────────────────'  '─────────↑────────────────' '----------------> 9248141834805313536 │  ¦   .---> Custom size allocation 0 >---.   ││
   by default the buffers are not mapped to avoid multi threading issues        .-------------------------------'     Buffer 10 ↑             ¦     Buffer 11 ↑  .---------------->   20266299256898688 │  ¦   ¦   '──────────────────────────'   ¦   ││
																				¦ .-----------------------------------------------------------'                  ¦                │          ¦          │  ¦   ¦                    Buffer 0 ↑    ¦   ││
   Supported VRAM size: 48GB                                                    ¦ ¦           ││                                                                 ¦                │          ↓          │  ¦   ¦                                  ¦   ││
   50MB per buffer. max 960 buffers                                             ¦ ¦           ││                                                                 ¦                │       Shader 1      <--¦---'    Output buffer (6.2208MB)      ¦   ││
   class 2MB:     25 cells per buffer                                           ¦ ¦           ││       Dynamically allocated buffers                             ¦           .---->   13510803177578784 │  ¦        Window 1                      ¦   ││
   class 500KB:   100 cells per buffer                                          ¦ ¦           ││       cell class 2MB. 25 cells per buffer                 ↑...  ¦           ¦ .-->    9007201402234640 │  ¦       .──────────────────────────.   ¦   ││
   class 5KB:     10k cells per buffer                                          ¦ ¦           ││      .────────────────────────────────────────────────────────. ¦         .-¦ ¦ ->   15763026045542688 │  ¦   - - > Custom size allocation 1 >-. ¦   ││
   class 50B      1Mln cells per buffer                                         ¦ ¦           ││      | cell 24      cell 25      cell 26      ... cell 47     >-'         ¦ ¦ ¦  │          ¦          │  ¦       '──────────────────────────' ¦ ¦   ││
   custom allocation max size: 7FFFFFFF (~2.15GB)                               ¦ ¦           ││      '────────────────────────────────────────────────────────' Buffer 9  ¦ ¦ ¦  │          ↓          │  ¦                        Buffer 1 ↑  ¦ ¦   ││
   larger data structures must be splitted across multiple buffers              ¦ ¦           ││      .────────────────────────────────────────────────────────.           ¦ ¦ ¦  │       Shader 2      <--'                                    ¦ ¦   ││
																				¦ ¦           ││      | cell 00      cell 01      cell 02      ... cell 23     |           ¦---¦ ->   15763026045542688 │           ↓...                        ¦ ¦   ││
																				¦ ¦           ││      '──────↑────────────↑─────────────↓──────────────────────' Buffer 8  ¦ ¦---->   13510803177578784 │                                       ¦ ¦   ││
																				¦ ¦     .--------------------'            ¦             '----------------------------------¦ ¦ ¦ ->   18014402806449280 │                                       ¦ ¦   ││
																				¦ ¦     ¦ .-------------------------------'                                                ¦ ¦ ¦  │          ¦          │                                       ¦ ¦   ││
																				¦ ¦     ¦ ¦   ││                                                                           ¦ ¦ ¦  │          ↓          │                                       ¦ ¦   ││
 extern                            RAM                                          ¦ ¦     ¦ ¦   ││                                                                           ¦ ¦ ¦  │         ...         │                                       ¦ ¦   ││
   ↓     _______________________________________________________                ¦ ¦     ¦ ¦   ││       Dynamically allocated buffers                                       ¦ ¦ ¦  │   ...               │         //TODO shaders >> copy cmd    ¦ ¦   ││
  Add   │ .───────────────────────────────────────────────────. │               ¦ ¦     ¦ ¦   ││       cell class 500KB. 100 cells per buffer             ↑...             ¦ ¦ ¦  │   ...               │                                       ¦ ¦   ││
   ↓    ││    OBJECT ARRAY             LuxObject               ││               ¦ ¦     ¦ ¦   ││      .────────────────────────────────────────────────────────.           ¦ ¦ ¦  :                     :                                       ¦ ¦   ││
   ¦    ││  .──────────────────.      .────────────────────.   ││               ¦ ¦     ¦ ¦   ││      |cell 100     cell 101     cell 102     ... cell 199     >-----------' ¦ ¦  .                     .                                       ¦ ¦   ││
   ¦    ││  │                  |   .--> vert buffer index  >------13800KB-------' ¦     ¦ ¦   ││      '────────────────────────────────────────────────────────' Buffer 7    ¦ ¦                                                                ¦ ¦   ││
   ¦--------> LuxObject 0      |   ¦--> indx buffer index  >------972KB-----------¦ ----' ¦   ││      .────────────────────────────────────────────────────────.             ¦ ¦                                                                ¦ ¦   ││
   ¦    ││  │ 920k v, 81k t    >---'  │ pos, rot, scl      >-----------------.    ¦       ¦   ││      |cell 000     cell 001     cell 002     ... cell 099     |             ¦ ¦               .----------------------------------------------- ¦-'   ││
   ¦    ││  │                  │      '────────────────────'   ││            ¦    ¦       ¦   ││      '────────────────────────────────↓───────────────────────' Buffer 6    ¦ ¦               ¦                              .-----------------'     ││
   ¦--------> LuxObject 1      │      .────────────────────.   ││            ¦    ¦       ¦   ││                                       ¦                                     ¦ ¦   .───────────↓──────────.       .───────────↓──────────.     ...    ││
   ¦    ││  │ 122k v, 5.2Mln t >---.--> vert buffer index  >------1830KB-----¦ ---¦ ------'   ││                                       '-------------------------------------' ¦   │ Swapchain 0          │       │ Swapchain 1          │            ││
   ¦    ││  │                  │   '--> indx buffer index  >------62400KB----¦ ---'           ││                                                                               ¦   │                      │       │                      │            ││
   ¦--------> LuxObject 2      │      │ pos, rot, scl      >---------------. ¦                ││       Dynamically allocated buffers                                           ¦   │ img >-----.          │       │ img  >-----.         │            ││
   :    ││  │ 6 v, 71 t        >---.  '────────────────────'   ││          ¦ ¦                ││       cell class 5KB. 10k cells per buffer               ↑...                 ¦   │ img >-----¦          │       │ img  >-----¦         │            ││
   .    ││  │ ....             │   ¦  .────────────────────.   ││          ¦ ¦                ││      .────────────────────────────────────────────────────────.               ¦   │ img >-----¦          │       │ img  >-----¦         │            ││
		││  :                  :   ¦--> vert buffer index  >------90B------¦ ¦ -----.         ││      | cell 10000   cell 10001   cell 10002   ... cell 19999  |               ¦   │ ...       ¦          │       │ ...        ¦         │            ││
		││  .                  .   '--> indx buffer index  >------852B-----¦ ¦ ---. ¦         ││      '────────────────────────────────────────────────────────' Buffer 5      ¦   '───────────¦──────────'       '────────────¦─────────'            ││
		││                            │ pos, rot, scl      >-------------. ¦ ¦    ¦ ¦         ││      .────────────────────────────────────────────────────────.               ¦               ¦                               ¦                      ││
		││                            '────────────────────'   ││        ¦ ¦ ¦    ¦ ¦         ││      | cell 00000   cell 00001   cell 00002   ... cell 09999  |               ¦         .---- ¦ ----------------------------- ¦ ----.                ││
		││                                                     ││        ¦ ¦ ¦    ¦ ¦         ││      '──────↑────────────↑──↓─────────────────────────────────' Buffer 4      ¦         ¦     ¦                               ¦     ¦                ││
		│'.___________________________________________________.'│        ¦ ¦ ¦    ¦ '------------------------'            ¦  ¦                                                 ¦         ¦                                           ¦                ││
		'───────────────────────────────────────────────────────'        ¦ ¦ ¦    '---------------------------------------'  '-------------------------------------------------'         ¦                                           ¦                ││
																		 ¦ ¦ ¦                ││                                                                                         ¦                                           ¦                ││
																		 ¦ ¦ ¦                ││                                                                                         ¦            D a r k    M a g i c           ¦                ││
																		 ¦ ¦ ¦                ││       Dynamically allocated buffers                                                     ¦                                           ¦                ││
	   A            A            A                                       ¦ ¦ ¦                ││       cell class 50B. 1Mln cells per buffer               ↑...                          ¦                                           ¦                ││
	   ¦            ¦        B---¦ ->B                                   ¦ ¦ ¦                ││      .────────────────────────────────────────────────────────.                         ¦                                           ¦                ││
   B---¦ ->B    B-->¦-->AB       ¦-->A                                   ¦ ¦ ¦                ││      | cell 1000000 cell 1000001 cell 1000002 ... cell 1999999|                         ¦     ¦                               ¦     ¦                ││
	   ↓            ↓            ↓                                       ¦ ¦ ¦                ││      '────────────────────────────────────────────────────────' Buffer 3                '---- ¦ ----------------------------- ¦ ----'                ││
	   A            AB           A                                       ¦ ¦ ¦                ││      .────────────────────────────────────────────────────────.                               ¦                               ¦                      ││
																		 ¦ ¦ ¦                ││      | cell 0000000 cell 0000001 cell 0000002 ... cell 0999999|                               ¦                               ¦                      ││
																		 ¦ ¦ ¦                ││      '──────↑────────────↑────────────↑───────────────────────' Buffer 2                      ¦                               ¦                      ││
																		 ¦ ¦ ¦                │'.____________¦____________¦____________¦_______________________________________________________¦_______________________________¦_____________________.'│
																		 ¦ ¦ ¦                '──────────────¦────────────¦────────────¦───────────────────────────────────────────────────────¦───────────────────────────────¦───────────────────────'
																		 ¦ ¦ '------36B----------------------'            ¦            ¦                                                       ↓                               ↓
																		 ¦ '--------36B-----------------------------------'            ¦                                                 Color output                    Color output
																		 '----------36B------------------------------------------------'
*/









class Engine;
static Engine* engine;


class Engine {
public:
	double FPS = 0;
	float FOV;
	bool running;
	bool useVSync;
	bool __lp_initialized = false;
	uint32 frame = 0;


	void run(bool _useVSync = true, float FOV = 45.0f);

private:
	//Main
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	//Window
	GLFWwindow* window;								//Main engine's window
	const uint32 WIDTH = 1920, HEIGHT = 1080;		//Default size in windowed mode
	//const uint32 WIDTH = 800, HEIGHT = 600;		//Default size in windowed mode

	//Devices and queues
	struct graphicsDevice {
		_VkPhysicalDevice PD;						//Main physical device for graphics
		VkDevice LD;								//Main logical device for graphics
		VkQueue graphicsQueue;						//Main graphics queue. Runs on graphicsLD
		VkQueue presentQueue;						//Main graphics queue. Runs on graphicsLD
	}graphics;

	struct computeDevice {
		_VkPhysicalDevice PD;						//Main physical device for computing
		VkDevice LD;								//Main logical device for computing
		LuxMap<VkQueue> computeQueues;				//Main compute queues. Run on computeLD
	}compute;

	LuxArray<computeDevice> secondary;				//Secondary devices and queues for computation



	//Main >> this
	void mainLoop();		void FPSCounter();     void render();
	void initWindow();		void createInstance();

	//Devices >> Devices.cpp
	void getPhysicalDevices();		void createLogicalDevice(_VkPhysicalDevice* PD, VkDevice* LD, VkQueue* graphicsQueue, VkQueue* presentQueue, LuxMap<VkQueue>* computeQueues);
	bool isDeviceSuitable(VkPhysicalDevice device, std::string errorText);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	//Shared functions >> this
	uint32* readShaderFromFile(uint32* length, const char* filename);
	VkShaderModule createShaderModule(VkDevice device, uint32* code, uint32* size);
	void createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);




	// Graphics ---------------------------------------------------------------------------------------------------------------------------------//




	bool framebufferResized = false;				//Updates the swapchain when the window is resized	
	const char* VERT_PATH = "LuxEngine/Contents/Shaders/vert.spv";
	const char* FRAG_PATH = "LuxEngine/Contents/Shaders/frag.spv";

	//Swapchain
	VkSwapchainKHR swapChain;
	LuxArray<VkImage> swapChainImages;
	LuxArray<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	LuxArray<VkFramebuffer> swapChainFramebuffers;

	//Render
	VkRenderPass renderPass;
	const int32 MAX_FRAMES_IN_FLIGHT = 16; //Default:2


private:
	//Commands
	VkCommandPool graphicsCommandPool;
	LuxArray<VkCommandBuffer> commandBuffers;

	//Render semaphores and frame
	LuxArray<VkSemaphore> imageAvailableSemaphores;
	LuxArray<VkSemaphore> renderFinishedSemaphores;
	LuxArray<VkFence> inFlightFences;
	LuxArray<VkFence> imagesInFlight;
	int64 currentFrame = 0;

	//debug and validation layers data
	LuxArray<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	LuxArray<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };



	//Graphics >> Graphics/GGraphics.cpp
	void runGraphics(bool _useVSync = true, float FOV = 45.0f);
	void initVulkan();
	void createSurface();
	void createSyncObjects();
	void createDebugMessenger();
	void drawFrame();
	static void framebufferResizeCallback(GLFWwindow* window, int32 width, int32 height);
	void cleanupGraphics();


	//Graphics swapchain >> Graphics/GSwapchain.cpp
	void createSwapChain();			void recreateSwapChain();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(LuxArray<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(LuxArray<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	void createImageViews();
	void cleanupSwapChain();


	//Graphics textures and images >> Graphics/GImages.cpp
	void createImage(uint32 width, uint32 height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32 width, uint32 height);


	//Graphics commands >> Graphics/GCommands.cpp
	void createGraphicsCommandPool();
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);


	//Graphics render >> Graphics/GPipeline.cpp
	void createRenderPass();
	void createFramebuffers();


	//Graphics other >> Graphics/Graphics.cpp
	VkFormat findSupportedFormat(LuxArray<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	uint32 findMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);




	// Compute ----------------------------------------------------------------------------------------------------------------------------------//




	//COMPUTE 
	const int32 COMPUTE_WIDTH = WIDTH;
	const int32 COMPUTE_HEIGHT = HEIGHT;
	const int32 WORKGROUP_SIZE = 32; // Workgroup size in compute shader.

	struct Pixel { unsigned char r, g, b, a; };


	VkDebugReportCallbackEXT debugReportCallback;


	//shaders
	struct LuxCShader {
		//Descriptors
		VkDescriptorPool descriptorPool;
		VkDescriptorSet descriptorSet;
		VkDescriptorSetLayout descriptorSetLayout;
		//Pipeline
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
		VkShaderModule shaderModule;
		//Commands
		VkCommandPool commandPool;
		LuxArray <VkCommandBuffer> commandBuffers;
	};
	LuxMap<LuxCShader> CShaders;


	//Buffers
	//This structure groups the components of a Vulkan buffer
	struct _LuxCell {
		uint64 ID;					//A unique id, different for each buffer
		uint32 size;				//The size in bytes of the buffer
		VkBuffer buffer;			//The actual Vulkan buffer
		VkDeviceMemory memory;		//The memory of the buffer
		bool isMapped = false;		//Whether the buffer is mapped or not
		//uint64 fragmentSize = 0;	//The size of the fragments. 0 means that the buffer is not divided
	};
	//This function maps a buffer to a void pointer. Mapping a buffer allows the CPU to access its data
	//Mapping an already mapped buffer will overwrite the old mapping
	//*   buffer: a pointer to a _LuxCell object. It's the buffer that will be mapped
	//*   returns the void pointer that maps the buffer
	void* mapGpuBuffer(_LuxCell* buffer) {
		if (buffer->isMapped) vkUnmapMemory(compute.LD, buffer->memory);
		else buffer->isMapped = true;
		void* data;
		vkMapMemory(compute.LD, buffer->memory, 0, buffer->size, 0, &data);
		return data;
	}
	LuxMap<_LuxCell> CGpuBuffers;

	//Compute >> Compute/Compute.cpp
	void runCompute();
	void cleanupCompute();
	LuxCell createGpuBuffer(uint64 size);
	int32 newCShader(LuxArray<LuxCell> buffers, const char* shaderPath);

	//Compute pipeline and descriptors >> Compute/CPipeline.cpp
	void CShader_create_descriptorSetLayouts(LuxArray<LuxCell> bufferIndices, LuxShader CShader);
	void CShader_create_descriptorSets(LuxArray<LuxCell> bufferIndices, LuxShader CShader);
	void CShader_create_CPipeline(const char* shaderPath, LuxShader CShader);

	//Compute command buffers >> Compute/CCommands.cpp
	void CShader_create_commandBuffers(LuxShader CShader);
	void runCommandBuffer(LuxShader CShader, uint32 imgIndex);

};








// Init -------------------------------------------------------------------------------------------------------------------------------------//








#define Frame while(engine->running)


//This function is used by the engine. You shouldn't call it
static void __lp_lux_init_run_thr(bool useVSync) {
	engine->run(useVSync, 45);
}



//This function is used by the engine. You shouldn't call it
static void __lp_luxInit(Engine* _engine, bool useVSync) {
	engine = _engine;
	std::thread t(__lp_lux_init_run_thr, useVSync);
	t.detach();
	engine->running = true;
}



#endif