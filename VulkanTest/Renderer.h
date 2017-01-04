//
// Created by McMacker4 on 03/01/2017.
//

#ifndef VULKANTEST_RENDERER_H
#define VULKANTEST_RENDERER_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <vulkan\vulkan.h>
#include <sstream>

#ifdef _WIN32
	#include <Windows.h>
#endif

class Renderer {
public:
	Renderer();
	virtual ~Renderer();

	uint32_t getGraphicsQueueFamilyIndex() { return graphicsQueueFamilyIndex; }
	VkDevice *getDevice() { return &device; }

private:
	void createInstance();
	void destroyInstance();

	void createDevice();
	void destroyDevice();

	VkPhysicalDevice getPhysicalDevice();
	uint32_t getQueueFamilyIndex(VkPhysicalDevice);

	VkInstance instance;
	VkDevice device;

	uint32_t graphicsQueueFamilyIndex;

#ifdef _DEBUG
	//Debugging Stuff
	void setupDebugging();
	void startDebugging();
	void stopDebugging();

	std::vector<const char*> instanceLayers;
	std::vector<const char*> instanceExtensions;

	VkDebugReportCallbackEXT debugReportCallback = nullptr;
	VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = {};
#endif

};

#endif //VULKANTEST_RENDERER_H
