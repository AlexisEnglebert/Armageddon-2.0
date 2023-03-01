#pragma once

#include <map>
#include <optional>
#include <vector>

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

    class VulkanRenderer
    {
        public:
            VulkanRenderer(){};
            ~VulkanRenderer(){Cleanup();};
            bool Init(VkInstance instance); 
            bool CreateVkInstance();
            bool pickPhysicalDevice();
            bool createLogicalDevice();
            bool InitVkSwapChain();
            void Cleanup();

            // Utilities
            Armageddon::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        private:  
        	VkInstance instance;
            VkPhysicalDevice physicalDevice;
            VkDevice logical_device;


    };
}