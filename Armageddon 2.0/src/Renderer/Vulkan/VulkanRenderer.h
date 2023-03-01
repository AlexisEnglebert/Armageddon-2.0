#pragma once

#include <map>
#include <optional>
#include <vector>
#include <set>  

#include <vulkan/vulkan.h>


#include "../../Log.h"

namespace Armageddon
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };
    //TODO move ce qu'on a vraiment besoin dans des classes 
    // Car là le projet est brodélique lol des classes partout 

    class VulkanRenderer
    {
        public:
            VulkanRenderer(){};
            ~VulkanRenderer(){Cleanup();};
            bool Init(VkInstance instance); 
            bool CreateVkInstance();
            bool pickPhysicalDevice();
            bool isDeviceSuitable(VkPhysicalDevice device);
            bool checkDeviceExtensionSupport(VkPhysicalDevice device);  
            bool createLogicalDevice();
            bool InitVkSwapChain();
            void Cleanup();

            //! Utilities
            //TODO: Maybe create VulkanUtils class ? 
            Armageddon::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


            //! THIS MAY BE IN Vulkan INTERFACE IDK
            //! SAME AS VALIDATION LAYERS ETC...; 
            //! COMME POUR DIRECTx
            const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        private:  
        	VkInstance instance;
            VkPhysicalDevice physicalDevice;
            VkDevice logical_device;


    };
}