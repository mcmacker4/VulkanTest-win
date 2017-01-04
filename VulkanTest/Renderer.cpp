//
// Created by McMacker4 on 03/01/2017.
//

#include "Renderer.h"

Renderer::Renderer() {
#ifdef _DEBUG
	setupDebugging();
#endif
	createInstance();
	createDevice();
#ifdef _DEBUG
	startDebugging();
#endif
}

Renderer::~Renderer() {
	destroyDevice();

#ifdef _DEBUG
	stopDebugging();
#endif

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
#ifdef _DEBUG
	instanceCreateInfo.enabledLayerCount = instanceLayers.size();
	instanceCreateInfo.ppEnabledLayerNames = instanceLayers.data();
	instanceCreateInfo.enabledExtensionCount = instanceExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instanceCreateInfo.pNext = &debugReportCallbackCreateInfo;
#endif
	std::cout << "[INFO][VkTest] Creating Vulkan instance." << std::endl;
	VkResult err = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
	if (err != VK_SUCCESS)
		throw "[INFO][VkTest] ERROR: Could not create Vulkan instance.";

	std::cout << "[INFO][VkTest] Vulkan Instance created successfully." << std::endl;

}

void Renderer::destroyInstance() {
	std::cout << "[INFO][VkTest] Destroying Vulkan instance." << std::endl;
	vkDestroyInstance(instance, nullptr);
	instance = nullptr;
}

void Renderer::createDevice() {

	VkPhysicalDevice physicalDevice = getPhysicalDevice();
	graphicsQueueFamilyIndex = getQueueFamilyIndex(physicalDevice);

	float priorities[]{ 1.0f };
	VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = priorities;
	deviceQueueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

	std::cout << "[INFO][VkTest] Creating Vulkan device." << std::endl;
	VkResult err = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
	if (err != VK_SUCCESS)
		throw "[INFO][VkTest] ERROR: Could not create Vulkan device.";

	std::cout << "[INFO][VkTest] Vulkan Device created successfully." << std::endl;

}

VkPhysicalDevice Renderer::getPhysicalDevice() {
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	std::cout << "[INFO][VkTest] Found " << physicalDeviceCount << " physical devices." << std::endl;

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
	std::cout << "[INFO][VkTest] Assuming physical device at index 0 to be correct." << std::endl;
	
	return physicalDevices[0];
}

uint32_t Renderer::getQueueFamilyIndex(VkPhysicalDevice physicalDevice) {
	uint32_t queueFamilyPropertiesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);
	std::cout << "[INFO][VkTest] Found " << queueFamilyPropertiesCount << " queue family properties." << std::endl;
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());
	for (uint32_t i = 0; i < queueFamilyPropertiesCount; i++) {
		if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			std::cout << "[INFO][VkTest] Queue family with index " << i << " is compatible." << std::endl;
			return i;
		}
	}
	throw "[INFO][VkTest] ERROR: Could not find compatible queue family.";
}

void Renderer::destroyDevice() {
	std::cout << "[INFO][VkTest] Destroying Vulkan device." << std::endl;
	vkDestroyDevice(device, nullptr);
	device = nullptr;
}

#ifdef _DEBUG

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = nullptr;

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObj,
	size_t location,
	int32_t msgCode,
	const char* layerPrefix,
	const char* msg,
	void* user_data
) {

	std::ostringstream output;
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		output << "[ERROR]";
#ifdef _WIN32
		MessageBox(NULL, msg, "ERROR", 0);
#endif
	} else if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		output << "[WARN]";
	} else if(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		output << "[PERF WARN]";
	} else if(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		output << "[DEBUG]";
	} else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		output << "[INFO]";
	}
	output << "[" << layerPrefix << "] " << msg << std::endl;
	std::cout << output.str();
	return false;
}

void Renderer::setupDebugging() {

	debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugReportCallbackCreateInfo.pfnCallback = VulkanDebugCallback;
	debugReportCallbackCreateInfo.flags =
		VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	/*instanceLayers.push_back("VK_LAYER_LUNARG_threading");
	instanceLayers.push_back("VK_LAYER_LUNARG_draw_state");
	instanceLayers.push_back("VK_LAYER_LUNARG_image");
	instanceLayers.push_back("VK_LAYER_LUNARG_mem_tracker");
	instanceLayers.push_back("VK_LAYER_LUNARG_object_tracker");
	instanceLayers.push_back("VK_LAYER_LUNARG_param_checker");*/

	instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");

	instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
}

void Renderer::startDebugging() {
	fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	if (fvkCreateDebugReportCallbackEXT == nullptr || fvkDestroyDebugReportCallbackEXT == nullptr)
		throw "[INFO][VkTest] ERROR: Failed to fetch extension proc handlers for debugging.";

	fvkCreateDebugReportCallbackEXT(instance, &debugReportCallbackCreateInfo, nullptr, &debugReportCallback);
}

void Renderer::stopDebugging() {
	fvkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);
}

#endif
