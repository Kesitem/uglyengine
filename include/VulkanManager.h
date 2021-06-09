#pragma once

#include "Core.h"

namespace ugly
{
    /**
     * @brief Vulkan manager.
     */
    class VulkanManager
    {
    public:

        /**
         * @brief Constructor.
         */
        VulkanManager();

        /**
         * @brief Destructor.
         */
        virtual ~VulkanManager();

        /**
         * @brief Initialize.
         * 
         * @return false if error 
         */
        bool initialize();

        /**
         * @brief Shutdown.
         */
        void shutdown();

    private:

        /**
         * @brief Create the vulkan instance.
         * 
         * @return false if error
         */
        bool createInstance();

        /**
         * @brief Check the validation layer support.
         * 
         * @return true if validation layer is supported 
         */
        bool checkValidationLayerSupport();

        /**
         * @brief Get the required extensions.
         * 
         * @return Required extensions
         */
        std::vector<const char*> getRequiredExtensions();

        /**
         * @brief Setup debug messenger for create and destroy instance.
         * 
         * @param _create_info Create info structure
         */
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _create_info);

        /**
         * @brief Setup debug messenger.
         * 
         * @return false if error 
         */
        bool setupDebugMessenger();

        /**
         * @brief Choose a physical device.
         *        
         * @return false if error 
         */
        bool pickPhysicalDevice();

        /**
         * @brief Check if the device is suitable
         * 
         * @param device Device to test
         * @return true if suitable 
         */
        bool isDeviceSuitable(VkPhysicalDevice device);

        /**
         * @brief Indices family
         */
        struct QueueFamilyIndices 
        {
            std::optional<uint32_t> graphicsFamily;

            bool isComplete() 
            {
                return graphicsFamily.has_value();
            }
        };

        /**
         * @brief Get queue families.
         * 
         * @param device Physical device
         * @return QueueFamilyIndices 
         */
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        /**
         * @brief Create a logical device.
         * 
         * * @return False if error
         */
        bool createLogicalDevice();

    private:

        /*! Selected validation layers */
        const std::vector<const char*> m_validation_layers = {"VK_LAYER_KHRONOS_validation"};

        /*! validation layers usage flag */
        #ifdef NDEBUG
        const bool m_enable_validation_layers = false;
        #else
        const bool m_enable_validation_layers = true;
        #endif

        /*! Vulkan instance */
        VkInstance m_instance;

        /*! Debug callback */
        VkDebugUtilsMessengerEXT m_callback;

        /*! Physical device */
        VkPhysicalDevice m_physical_device {VK_NULL_HANDLE};

        /*! Logical device */ 
        VkDevice m_device;

        /*! Graphic queue */
        VkQueue m_graphics_queue;
    };
}