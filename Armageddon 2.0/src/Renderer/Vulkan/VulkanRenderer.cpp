#include "VulkanRenderer.h"


bool Armageddon::VulkanRenderer::Init(VkInstance instance)
{
    this->instance = instance; 
    Armageddon::Log::GetLogger()->info("Initalization of Vulkan renderer");
    pickPhysicalDevice(); 
    createLogicalDevice();
    InitVkSwapChain();
}




//Other method to check device Suitability
bool isDeviceSuitable(VkPhysicalDevice device) {

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
     return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader; //TODO more constrain...
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

    return true;
}

bool Armageddon::VulkanRenderer::createLogicalDevice()
{
<<<<<<< HEAD
    Armageddon::Log::GetLogger()->info("Creating Logical Device");
=======
    Armageddon::Log::GetLogger()->info("Creating Logical Device ");
>>>>>>> 3a8a62a (src: working on vulkan renderer, selecting devices)
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

<<<<<<< HEAD
    //createInfo.enabledExtensionfCount = 0;
    //TODO: validation layers
    /*bool enableValidationLayers = true;
=======
    createInfo.enabledExtensionCount = 0;


    //TODO: validation layers
   /* bool enableValidationLayers = true;
>>>>>>> 3a8a62a (src: working on vulkan renderer, selecting devices)
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
<<<<<<< HEAD
    }
    */
   return true;
=======
    }*/
>>>>>>> 3a8a62a (src: working on vulkan renderer, selecting devices)
}

bool Armageddon::VulkanRenderer::InitVkSwapChain()
{
    Armageddon::Log::GetLogger()->trace("Init Vulkan Swapchain");
    
    const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    return false;
}

void Armageddon::VulkanRenderer::Cleanup()
{
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
        /*if(indices.isComplete()){
            break;
        }*/
        Armageddon::Log::GetLogger()->info("ICI CA BUG ? ");
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
        }

        Armageddon::Log::GetLogger()->info("okOK OK ");
        i++;
    }
    return indices;
}
