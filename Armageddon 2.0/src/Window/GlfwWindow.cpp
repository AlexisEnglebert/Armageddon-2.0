#include "GlfwWindow.h"

Armageddon::GlfwWindow::GlfwWindow(int width, int height, std::wstring title, std::wstring wclass) 
{
		this->w_height = height;
		this->w_width = width;
		this->w_title = title;
		this->w_class = wclass;

    	glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		this->window = glfwCreateWindow(width,height, "Armageddon Editor", NULL, NULL);
		VkSurfaceKHR surface;
		VkInstance instance;

		//Initialisation of vulkan 
		Armageddon::Log::GetLogger()->trace("Initializaton of Vulkan app");

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; //Define struct type
		appInfo.pApplicationName = "Armageddon 2.0";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Armageddon 2.0";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		//This next struct is not optional and tells the Vulkan driver which global extensions and validation layers we want to use.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // use gltf exetensions for vulkan 
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtensionCount; // Need extension to handle every platforms
		createInfo.ppEnabledExtensionNames = glfwExtensions;// Need extension to handle every platforms
		createInfo.enabledLayerCount = 0; // reste obscur pour l'instant

		Armageddon::Log::GetLogger()->trace("Creating VK instance");

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		if (result != VK_SUCCESS)
		{
			Armageddon::Log::GetLogger()->error("Error while creating Vulkan Instance : {0}", result);
			exit(-1);
		}
		VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		if (deviceCount <= 0)
		{
			Armageddon::Log::GetLogger()->error("No device Found : {0}", deviceCount);
			exit(-1);
		}
		Armageddon::Log::GetLogger()->info("Number of devices : {0}", deviceCount);
		VkPhysicalDeviceProperties deviceProperty;
		vkGetPhysicalDeviceProperties(devices[0], &deviceProperty);
		Armageddon::Log::GetLogger()->info("Device : {0}", deviceProperty.deviceName);
		Armageddon::Log::GetLogger()->info("VendorID : {0}", deviceProperty.vendorID);
		Armageddon::Log::GetLogger()->info("DeviceID : {0}", deviceProperty.deviceID);
		Armageddon::Log::GetLogger()->info("DeviceType : {0}", deviceProperty.deviceType);

		// TODO SHOULD BE FORBIDDEN 
		// vkDestroyInstance(instance, nullptr);
}

bool Armageddon::GlfwWindow::ProcessMessage()
{
	Armageddon::Log::GetLogger()->info("Process Message GLFW");
	if(!glfwWindowShouldClose(this->window)) {
		glfwPollEvents();
	}
    return false;
}
