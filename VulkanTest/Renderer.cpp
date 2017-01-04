//
// Created by McMacker4 on 03/01/2017.
//

#include "Renderer.h"

Renderer::Renderer() {
	createInstance();
	createDevice();
}

Renderer::~Renderer() {
	destroyDevice();
	destroyInstance();
}

void Renderer::createInstance() {

	VkApplicationInfo applicationInfo{};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 3);
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	applicationInfo.pApplicationName = "Vulkan Test";
	
	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	std::cout << "Vk: Creating Vulkan instance." << std::endl;
	VkResult err = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
	if (err != VK_SUCCESS)
		throw "Vk: ERROR: Could not create Vulkan instance.";

	std::cout << "Vk: Vulkan Instance created successfully." << std::endl;

}

void Renderer::destroyInstance() {
	std::cout << "Vk: Destroying Vulkan instance." << std::endl;
	vkDestroyInstance(instance, nullptr);
	instance = nullptr;
}

void Renderer::createDevice() {

	VkPhysicalDevice physicalDevice = getPhysicalDevice();
	uint32_t familyIndex = getQueueFamilyIndex(physicalDevice);

	float priorities[]{ 1.0f };
	VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = priorities;
	deviceQueueCreateInfo.queueFamilyIndex = familyIndex;

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

	std::cout << "Vk: Creating Vulkan device." << std::endl;
	VkResult err = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
	if (err != VK_SUCCESS)
		throw "Vk: ERROR: Could not create Vulkan device.";

	std::cout << "Vk: Vulkan Device created successfully." << std::endl;

}

VkPhysicalDevice Renderer::getPhysicalDevice() {
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	std::cout << "Vk: Found " << physicalDeviceCount << " physical devices." << std::endl;

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
	std::cout << "Vk: Assuming physical device at index 0 to be correct." << std::endl;
	
	return physicalDevices[0];
}

uint32_t Renderer::getQueueFamilyIndex(VkPhysicalDevice physicalDevice) {
	uint32_t queueFamilyPropertiesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);
	std::cout << "Vk: Found " << queueFamilyPropertiesCount << " queue family properties." << std::endl;
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());
	for (uint32_t i = 0; i < queueFamilyPropertiesCount; i++) {
		if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			std::cout << "Vk: Queue family with index " << i << " is compatible." << std::endl;
			return i;
		}
	}
	throw "Vk: ERROR: Could not find compatible queue family.";
}

void Renderer::destroyDevice() {
	std::cout << "Vk: Destroying Vulkan device." << std::endl;
	vkDestroyDevice(device, nullptr);
	device = nullptr;
}
