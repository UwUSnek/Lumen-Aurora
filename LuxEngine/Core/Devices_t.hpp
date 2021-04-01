#pragma once
#define LUX_H_DEVICES_T
#include <vulkan/vulkan.hpp>
#include "LuxEngine/Types/Integers/Integers.hpp"
#include "LuxEngine/Types/Containers/RtArray.hpp"








//Available queue families of a physical device
struct QueueFamilyIndices {
	uint32 graphicsFamily = (uint32)-1;
	uint32 presentFamily  = (uint32)-1;
	lux::RtArray<uint32> computeFamilies;

	QueueFamilyIndices()							: graphicsFamily{ (uint32)-1 },       presentFamily{ (uint32)-1 },      computeFamilies() {}
	QueueFamilyIndices(const QueueFamilyIndices& e) : graphicsFamily{ e.graphicsFamily }, presentFamily{ e.presentFamily }, computeFamilies(e.computeFamilies) {}

	inline bool isGraphicsComplete() { return (graphicsFamily != (uint32)-1 && presentFamily != (uint32)-1); }
	void operator=(const QueueFamilyIndices& i){
		graphicsFamily = i.graphicsFamily;
		presentFamily = i.presentFamily;
		computeFamilies = i.computeFamilies;
	}
	void operator=(const QueueFamilyIndices&& i){
		graphicsFamily = std::move(i.graphicsFamily);
		presentFamily = std::move(i.presentFamily);
		computeFamilies = std::move(i.computeFamilies);
	}
};




//Structure containing all the useful data of a physical device
struct _VkPhysicalDevice {
	_VkPhysicalDevice(const vk::PhysicalDevice vDevice, const vk::PhysicalDeviceProperties& vProperties, const vk::PhysicalDeviceFeatures& vFeatures, const QueueFamilyIndices& vIndices) :
						  device{ vDevice }, properties{ vProperties }, features{ vFeatures }, indices{ vIndices }, score{ 0 } {}
	_VkPhysicalDevice() : device{ nullptr }, properties{             }, features{           }, indices{          }, score{ 0 } {}

	vk::PhysicalDevice				device;		//Actual vk::PhysicalDevice structure
	vk::PhysicalDeviceProperties	properties;	//Physical device properties
	vk::PhysicalDeviceFeatures		features;	//Physical device features
	QueueFamilyIndices				indices;	//Indices of the queue families
	uint32							score;		//Device performances evalutation

	//TODO REMOVE
	auto& operator=(const _VkPhysicalDevice& d){
		device = d.device;
		properties = d.properties;
		features = d.features;
		indices = d.indices;
		score = d.score;
		return *this;
	}
};





struct graphicsDevice {
	graphicsDevice() : PD(), LD{ nullptr }, graphicsQueue{ nullptr }, presentQueue{ nullptr } {}

	_VkPhysicalDevice PD;		//Main physical device for graphics
	vk::Device		  LD;		//Main logical device for graphics
	vk::Queue graphicsQueue;	//Main graphics queue. Runs on graphicsLD
	vk::Queue presentQueue;		//Main graphics queue. Runs on graphicsLD
};




struct computeDevice {
	computeDevice() : PD(), LD{ nullptr }, computeQueues() {}

	_VkPhysicalDevice PD;					//Main physical device for computing
	vk::Device        LD;					//Main logical device for computing
	lux::RtArray<vk::Queue> computeQueues;	//Main compute queues. Run on computeLD
};