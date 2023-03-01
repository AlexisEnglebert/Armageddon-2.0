#include "VulkanRenderer.h"


bool Armageddon::VulkanRenderer::Init(VkInstance instance)
{
    this->instance = instance; 
    Armageddon::Log::GetLogger()->info("Initalization of Vulkan renderer");
    if(!pickPhysicalDevice()) return false;
    if(!createLogicalDevice()) return false;
    if(!InitVkSwapChain()) return false;
    
    return true;

}



bool Armageddon::VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device){
   uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
    std::set<std::string> requiredExtensions(this->deviceExtensions.begin(), this->deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool Armageddon::VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device) {

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    QueueFamilyIndices indices = findQueueFamilies(device);
     bool extensionsSupported = checkDeviceExtensionSupport(device);

     return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader && indices.isComplete() && extensionsSupported; //TODO more constrain...
}



int rateDeviceSuitability(VkPhysicalDevice device) {
    int score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader) {
        return 0;
    }

    return score;
}
bool Armageddon::VulkanRenderer::pickPhysicalDevice()
{
    Armageddon::Log::GetLogger()->info("Picking Physical device");
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    if (deviceCount <= 0){
        Armageddon::Log::GetLogger()->error("No device Found : {0}", deviceCount);
        return false;
    }
    
    Armageddon::Log::GetLogger()->info("Found {0} device",deviceCount);
    
    //Todo beter device selection (for now use the first)
    
    std::multimap<int, VkPhysicalDevice> candidates;
    for(const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    VkPhysicalDeviceProperties deviceProperty;
    vkGetPhysicalDeviceProperties(devices[0], &deviceProperty);
    Armageddon::Log::GetLogger()->info("Device : {0}", deviceProperty.deviceName);
    Armageddon::Log::GetLogger()->info("VendorID : {0}", deviceProperty.vendorID);
    Armageddon::Log::GetLogger()->info("DeviceID : {0}", deviceProperty.deviceID);
    Armageddon::Log::GetLogger()->info("DeviceType : {0}", deviceProperty.deviceType);
    return true;
}

bool Armageddon::VulkanRenderer::createLogicalDevice()
{
    Armageddon::Log::GetLogger()->trace("Creating Logical Device");
    // Check for device queue family
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;


    // Create logical device
    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    //TODO: validation layers

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logical_device) != VK_SUCCESS) {
        Armageddon::Log::GetLogger()->error("Failed creating Logical device :( ");
        return false;
    }

   

    Armageddon::Log::GetLogger()->info("Finished creating Logical Device ");
    return true;

}

bool Armageddon::VulkanRenderer::InitVkSwapChain()
{
    Armageddon::Log::GetLogger()->trace("Init Vulkan Swapchain");
    
    //TODO check for swapchain support 

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    return true;
}

void Armageddon::VulkanRenderer::Cleanup()
{
    Armageddon::Log::GetLogger()->trace("Destroying Vulkan's structure");
    vkDestroyDevice(logical_device, nullptr);
    vkDestroyInstance(instance, nullptr);

}

Armageddon::QueueFamilyIndices Armageddon::VulkanRenderer::findQueueFamilies(VkPhysicalDevice device)
{
    Armageddon::Log::GetLogger()->info("Finding Queue family");
    
    Armageddon::QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    for(const auto& queueFamily : queueFamilies){
        if(indices.isComplete()){
            break;
        }
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
        }
        Armageddon::Log::GetLogger()->info("FamilyIdx has values : {0}", indices.graphicsFamily.has_value());
        i++;
    }
    return indices;
}
