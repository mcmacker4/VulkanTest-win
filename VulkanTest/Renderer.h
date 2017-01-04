//
// Created by McMacker4 on 03/01/2017.
//

#ifndef VULKANTEST_RENDERER_H
#define VULKANTEST_RENDERER_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <vulkan\vulkan.h>

class Renderer {
public:
	Renderer();
	virtual ~Renderer();
private:
	void createInstance();
	void destroyInstance();

	void createDevice();
	void destroyDevice();

	VkPhysicalDevice getPhysicalDevice();
	uint32_t getQueueFamilyIndex(VkPhysicalDevice);

	VkInstance instance;
	VkDevice device;
};

#endif //VULKANTEST_RENDERER_H
