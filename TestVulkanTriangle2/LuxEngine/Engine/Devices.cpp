﻿#include "LuxEngine/Engine/Engine.h"

//Compares 2 _VkPhysicalDevice objects
#define sameDevice(a,b) ((a).properties.deviceID == (b).properties.deviceID)







//Rates a physical device based on its properties and features
//*   pDevice: a pointer to the device structure where its infos are stored
//*   Returns the rating of the physical device
int32 Engine::ratePhysicalDevice(const _VkPhysicalDevice* pDevice) {
	uint32 score = 0;																			//Device performance evalutation
	if (pDevice->properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000000;	//Discrete GPUs have performance advantage
	pDevice->properties.limits.maxComputeSharedMemorySize; //TODO non superare il limite della shader
	score += pDevice->properties.limits.maxImageDimension2D;										//Maximum possible size of textures affects graphics quality
	if (pDevice->features.geometryShader) score += 1;												//Geometry shaders needed
	return score;
}




//TODO check graphics only for main graphics devices
//Checks if a device has the required extensions and properties to run vulkan
//*   vDevice: the physical device to check
//*   pErrorText: a pointer to a LuxString where to store the error in case the device is not suitable
//*   Returns true if the device is suitable, false if not
bool Engine::isDeviceSuitable(const VkPhysicalDevice vDevice, std::string* pErrorText) {
	//Check extensions
	if (!checkDeviceExtensionSupport(vDevice)) {
		*pErrorText = "Missing required extensions";
		return false;
	}

	//Check swapchain support
	else {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vDevice);
		if (swapChainSupport.formats.size() == 0 || swapChainSupport.presentModes.size() == 0) {
			*pErrorText = "Unsupported swapchain";
			return false;
		}
	}
	return true;
}




//Returns true if the device supports the extensions, false if not
bool Engine::checkDeviceExtensionSupport(const VkPhysicalDevice vDevice) {
	uint32 extensionCount;
	vkEnumerateDeviceExtensionProperties(vDevice, nullptr, &extensionCount, nullptr);					//Get extension count
	LuxArray<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(vDevice, nullptr, &extensionCount, availableExtensions.data());	//Get extensions

	//TODO use LuxMap
	std::set<const char*> requiredExtensions(requiredDeviceExtensions.begin(), requiredDeviceExtensions.end());
	for (const auto& extension : availableExtensions) requiredExtensions.erase(extension.extensionName);//Search for required extensions
	return requiredExtensions.empty();
}




//Finds the queue families of a physical device
QueueFamilyIndices Engine::findQueueFamilies(const VkPhysicalDevice vDevice) {
	uint32 queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(vDevice, &queueFamilyCount, nullptr);						//Enumerate queue families
	LuxArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vDevice, &queueFamilyCount, queueFamilies.data());			//Save queue families

	//Set families
	QueueFamilyIndices indices;
	for (int i = 0; i < queueFamilies.size(); i++) {													//For every queue family
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily = i;				//Set graphics family
		if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) indices.computeFamilies.add(i);				//Add compute families
		VkBool32 hasPresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(vDevice, i, surface, &hasPresentSupport);						//Set present family
		if (hasPresentSupport) indices.presentFamily = i;
	}
	return indices;
}








//-----------------------------------------------------------------------------------------------------------------------------//








//Finds all the suitable physical devices, choosing the main and secondary devices according to their capabilities,
//Then saves them in the class members
void Engine::getPhysicalDevices() {
	uint32 deviceCount = 0;
	LuxMap<std::string> discardedPhysicalDevices(0xFFFF, 0xFFFF);
	LuxMap<_VkPhysicalDevice*> physicalDevices(0xFFFF, 0xFFFF);


	//Get physical devices
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) Exit("Failed to find GPUs with Vulkan support")
	else {
		//Get physical devices
		LuxArray<VkPhysicalDevice> physDevices(deviceCount);								//Get physical device count
		vkEnumeratePhysicalDevices(instance, &deviceCount, physDevices.data());					//Get physical devices

		forEach(physDevices, i) {																//For every physical device, create and save a _VkPhysicalDevice stucture
			VkPhysicalDeviceProperties properties;	vkGetPhysicalDeviceProperties(physDevices[i], &properties);
			VkPhysicalDeviceFeatures features;		vkGetPhysicalDeviceFeatures(physDevices[i], &features);
			//TODO use LuxString
			std::string errorText;
			if (isDeviceSuitable(physDevices[i], &errorText)) {										//If it's suitable
				physicalDevices.add(new _VkPhysicalDevice(physDevices[i], properties, features, *new QueueFamilyIndices)); //Add it to the physical devices vector
			}
			else {																						//If not
				discardedPhysicalDevices.add(properties.deviceName);									//Add it to the discarded devices vector
				discardedPhysicalDevices.add(errorText);												//And save the reason of its unsuitability
			}
		}
	}


	//If there are discarded devices, print their names
	if (discardedPhysicalDevices.size() > 0) {
		Failure printf("    Discarded devices:");
		for (int32 i = 0; i < discardedPhysicalDevices.size(); i += 2) {
			Failure printf("        %s\t|  %s", discardedPhysicalDevices[i].data(), discardedPhysicalDevices[(int64)i + 1].data());
		}
	}

	//TODO different score for graphics and compute
	#define physDev (*physicalDevices[i])
	if (physicalDevices.size() > 0) {								//If there are suitable devices
		graphics.PD = *physicalDevices[0];								//set graphics device at default value
		compute.PD = *physicalDevices[0];								//set compute  device at default value
		forEach(physicalDevices, i) {									//For every physical device
			physDev.indices = findQueueFamilies(physDev.device);			//Get its queue families
			physDev.score = ratePhysicalDevice(&physDev);					//And its score. Then check if it has the necessary queues and set it as the main graphics and or compute physical device
			if (physDev.score > graphics.PD.score || physDev.indices.graphicsFamily != -1) graphics.PD = physDev;
			if (physDev.score > compute.PD.score || physDev.indices.computeFamilies.size() > 0) compute.PD = physDev;
		}
		forEach(physicalDevices, i) {									//For every physical device that isn't the main graphics or compute device
			if (!sameDevice(physDev, graphics.PD) && !sameDevice(physDev, compute.PD)) {
				secondary.resize(secondary.size() + 1);
				secondary[secondary.size() - 1].PD = physDev;				//Add it to the secondary devices vector (it'll be used as a compute device with less priority. T.T poor gpu)
			}
		}

		//Print the devices names, IDs, scores and tasks
		Success printf("    Found %lld suitable device%s:", physicalDevices.size(), (physicalDevices.size() == 1) ? "" : "s");
		forEach(physicalDevices, i) {
			if (sameDevice(physDev, graphics.PD) || sameDevice(physDev, compute.PD)) Main else Normal;
			printf("        %s  |  ID: %d  |  %d", physDev.properties.deviceName, physDev.properties.deviceID, physDev.score);
			if (sameDevice(physDev, graphics.PD)) printf("  |  Main graphics");
			if (sameDevice(physDev, compute.PD)) printf("  |  Main compute");
		}
	}
	else Exit("Failed to find a suitable GPU");
	#undef physDev



	//Logical devices
	//Create a logical device for graphics, one for computation and one for every secondary device
	createLogicalDevice(&graphics.PD, &graphics.LD, nullptr);
	createLogicalDevice(&compute.PD, &compute.LD, &compute.computeQueues);
	for (int32 i = 0; i < secondary.size(); i++) {
		createLogicalDevice(&secondary[i].PD, &secondary[i].LD, &secondary[i].computeQueues);
	}

	//Output created logical devices and queues
	Success printf("    Created %lld logical devices:", 2 + secondary.size());
	Main	printf("        Main graphics  |  graphics queues: 1  |  present queues:  1");
	Main	printf("        Main compute   |  compute queues:  %lld", compute.computeQueues.size());
	Normal	printf("        %lld secondary devices",/*  |  secondary compute queues: %lld", secondary.size, */secondary.size());
}













//Create a logical device from a physical one
//*   pPD: a pointer to the physical device structure containing its infos
//*   pLD: a pointer to the logical device where to store the created device
//*   pComputeQueues: a pointer to an array of compute queues
//*       This is used to know if the physical device is for graphics, computation or is secondary
void Engine::createLogicalDevice(const _VkPhysicalDevice* pPD, VkDevice* pLD, LuxMap<VkQueue>* pComputeQueues) {
	//List unique device's queues
	std::set<int32> uniqueQueueFamilyIndices;
	if (sameDevice(*pPD, graphics.PD)) {												//If it's the main device for graphics,
		uniqueQueueFamilyIndices.insert(pPD->indices.graphicsFamily);					//Add his graphics family
		uniqueQueueFamilyIndices.insert(pPD->indices.presentFamily);						//And his present family
	}
	forEach(pPD->indices.computeFamilies, i) {											//And then add every compute family, graphics ones included
		uniqueQueueFamilyIndices.insert(pPD->indices.computeFamilies[i]);
	}



	//Queue infos
	LuxMap<VkDeviceQueueCreateInfo> queueCreateInfos;								//Create a queue create info array
	for (auto queueFamilyIndex : uniqueQueueFamilyIndices) {							//For every device queue family index found
		VkDeviceQueueCreateInfo queueCreateInfo{};											//Create a queue create info struct
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;					//Set structure type
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;								//Set index
		queueCreateInfo.queueCount = 1;														//Set count		// ↓ Set priority. 1 for main devices, 0.5 for secondary ones
		queueCreateInfo.pQueuePriorities = new float((sameDevice(*pPD, graphics.PD) || sameDevice(*pPD, compute.PD)) ? 1.0f : 0.5f);
		queueCreateInfos.add(queueCreateInfo);											//Add it to the queue create info array
	}

	//Required extensions
	VkPhysicalDeviceFeatures enabledDeviceFeatures{};								//Set enabled features
	enabledDeviceFeatures.samplerAnisotropy = VK_FALSE;									//No anistropy filter
	enabledDeviceFeatures.multiViewport = VK_FALSE;										//No multiple viewports
	enabledDeviceFeatures.fillModeNonSolid = VK_FALSE;									//No point32 and line render, since we don't use meshes

	//Fill deviceCreateInfo
	VkDeviceCreateInfo deviceCreateInfo{};											//Create deviceCreateInfo structure for logical device creation
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;						//Set structure type
	deviceCreateInfo.queueCreateInfoCount = (int32)queueCreateInfos.size();				//Set queue infos count
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();						//Set queue infos
	deviceCreateInfo.enabledExtensionCount = (int32)requiredDeviceExtensions.size();	//Set required extentions count
	deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();			//Set required extensions
	deviceCreateInfo.pEnabledFeatures = &enabledDeviceFeatures;							//Set physical device enabled features

	luxDebug(deviceCreateInfo.enabledLayerCount = (int32)validationLayers.size();)		//Set validation layers count if in debug mode
	luxDebug(deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();)			//Set validation layers if in debug mode
	luxRelease(deviceCreateInfo.enabledLayerCount = 0;)									//Disable validation layers if in release mode



	//Create the logical device and save its queues, exit if an error occurs
	VkDevice _logicalDevice;
	if (vkCreateDevice(pPD->device, &deviceCreateInfo, nullptr, &_logicalDevice) == VK_SUCCESS) {
		if (sameDevice(*pPD, graphics.PD) || sameDevice(*pPD, compute.PD)) {
			if (sameDevice(*pPD, graphics.PD)) {														//If it's the main graphics device
				graphics.LD = _logicalDevice;																//Set it as the main graphics logical device
				vkGetDeviceQueue(_logicalDevice, pPD->indices.graphicsFamily, 0, &graphics.graphicsQueue);	//Set graphics queue
				vkGetDeviceQueue(_logicalDevice, pPD->indices.presentFamily, 0, &graphics.presentQueue);	//Set present queue
			}
			if (pComputeQueues != nullptr) {																//If it's the main compute device and the function was called to create his logical device
				compute.LD = _logicalDevice;																//Set it as the main compute logical device
				forEach(pPD->indices.computeFamilies, i) {													//Add every compute queue to the main compute queue list
					VkQueue computeQueue;
					vkGetDeviceQueue(_logicalDevice, pPD->indices.computeFamilies[i], 0, &computeQueue);
					compute.computeQueues.add(computeQueue);
				}
			}
		}
		else {																							//If it's none of them
			*pLD = _logicalDevice;																			//Add it to the list of secondary logical devices
			forEach(pPD->indices.computeFamilies, i) {														//Add every compute queue to the secondary compute queues
				VkQueue computeQueue;
				vkGetDeviceQueue(_logicalDevice, pPD->indices.computeFamilies[i], 0, &computeQueue);
				pComputeQueues->add(computeQueue);
			}
		}
	}
	else Exit("Failed to create logical device");
}
