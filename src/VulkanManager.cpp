#include "VulkanManager.h"
#include "config.h"


/**
 * @brief Create a vKCreateDebugUtilsMessengerExt extension.
 * 
 * @param instance 
 * @param pCreateInfo 
 * @param pAllocator 
 * @param pCallback 
 * @return VkResult 
 */
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


/**
 * @brief Create vkDestroyDebugUtilsMessengerEXT extension.
 * 
 * @param instance 
 * @param callback 
 * @param pAllocator 
 */
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        func(instance, callback, pAllocator);
    }
}


/**
 * @brief Vulkan callback error.
 * 
 * @param messageSeverity 
 * @param messageType 
 * @param pCallbackData 
 * @param pUserData 
 * @return VKAPI_ATTR 
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    PLOG_ERROR << "validation layer: " << pCallbackData->pMessage;

    return VK_FALSE;
}


/**
 * @brief Constructor.
 */
ugly::VulkanManager::VulkanManager()
{
}


/**
 * @brief Destructor.
 */
ugly::VulkanManager::~VulkanManager()
{
}


/**
 * @brief Initialize.
 * 
 * @return false if error 
 */
bool ugly::VulkanManager::initialize()
{
    LOG_INFO << "--- Initialize vulkan manager";

    if(!createInstance())
    {
        return false;
    }

    if(!setupDebugMessenger())
    {
        return false;
    }

    if(!pickPhysicalDevice())
    {
        return false;
    }

    if(!createLogicalDevice())
    {
        return false;
    }
    
    return true;
}


/**
 * @brief Shutdown.
 */
void ugly::VulkanManager::shutdown()
{
    LOG_INFO << "--- Shutdown vulkan manager";

    vkDestroyDevice(m_device, nullptr);

    if(m_enable_validation_layers)
        DestroyDebugUtilsMessengerEXT(m_instance, m_callback, nullptr);

    vkDestroyInstance(m_instance, nullptr);
}


/**
 * @brief Create the vulkan instance.
 *
 * @return false if error 
 */
bool ugly::VulkanManager::createInstance()
{
    LOG_INFO << "Validation layers enable: " << m_enable_validation_layers;

    if(m_enable_validation_layers && !checkValidationLayerSupport())
    {
        LOG_ERROR << "Validation layers are active but not available";
        return false;
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "UglyEngine application";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "UglyEngine";
    app_info.engineVersion = VK_MAKE_VERSION(version::MAJOR, version::MINOR, version::BUILD);
    app_info.apiVersion = VK_API_VERSION_1_0;   

    auto extensions = getRequiredExtensions();

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
    if(m_enable_validation_layers)
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
        create_info.ppEnabledLayerNames = m_validation_layers.data();
        populateDebugMessengerCreateInfo(debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
    } 
    else 
    {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> extensions_available(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions_available.data());
    LOG_INFO << "Available extensions :";
    for (const auto& extension : extensions_available) 
    {
        LOG_INFO << '\t - ' << extension.extensionName;
    }

    if(vkCreateInstance(&create_info, nullptr, &m_instance))
    {
        LOG_ERROR << "Failed to create vulkan instance";
        return false;
    }

    return true;
}


/**
 * @brief Check the validation layer support.
 * 
 * @return true if validation layer is supported 
 */
bool ugly::VulkanManager::checkValidationLayerSupport()
{
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char* layer_name : m_validation_layers) 
    {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers) 
        {
            if (strcmp(layer_name, layer_properties.layerName) == 0) 
            {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) 
        {
            return false;
        }
    }

    return true;
}


/**
 * @brief Get the required extensions.
 * 
 * @return Required extensions
 */
std::vector<const char*> ugly::VulkanManager::getRequiredExtensions()
{
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if (m_enable_validation_layers) 
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


/**
 * @brief Setup debug messenger for create and destroy instance.
 * 
 * @param _create_info Create info structure
 */
void ugly::VulkanManager::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _create_info) 
{
    _create_info = {};
    _create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    _create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    _create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    _create_info.pfnUserCallback = debugCallback;
}


/**
 * @brief .
 */
bool ugly::VulkanManager::setupDebugMessenger()
{
    if(!m_enable_validation_layers)
        return true;

    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debugCallback;
    create_info.pUserData = nullptr; // Optionnel
    populateDebugMessengerCreateInfo(create_info);
    if (CreateDebugUtilsMessengerEXT(m_instance, &create_info, nullptr, &m_callback) != VK_SUCCESS) 
    {
        PLOG_ERROR << "Failed to create debug messenger";
        return false;
    }

    return true;
}


/**
 * @brief Choose a physical device.
 *        
 * @return false if error 
 */
bool ugly::VulkanManager::pickPhysicalDevice()
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);

    if (device_count == 0) 
    {
        PLOG_ERROR << "No physical device found";
        return false;
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

    for (const auto& device : devices) 
    {
        if (isDeviceSuitable(device)) 
        {
            m_physical_device = device;
            break;
        }
    }

    if (m_physical_device == VK_NULL_HANDLE) 
    {
        PLOG_ERROR << "No suitable physical device found";
        return false;
    }

    return true;
}


/**
 * @brief Check if the device is suitable
 * 
 * @param device Device to test
 * @return true if suitable 
 */
bool ugly::VulkanManager::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
}


/**
 * @brief Get queue families.
 * 
 * @param device Physical device
 * @return QueueFamilyIndices 
 */
ugly::VulkanManager::QueueFamilyIndices ugly::VulkanManager::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    int i = 0;
    for (const auto& queueFamily : queue_families) 
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
        {
            indices.graphicsFamily = i;
        }

        i++;
    }

    return indices;
}


/**
 * @brief Create a logical device.
 * 
 * @return False if error
 */
bool ugly::VulkanManager::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(m_physical_device);

    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = indices.graphicsFamily.value();
    queue_create_info.queueCount = 1;
    float queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.queueCreateInfoCount = 1;

    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = 0;
    if (m_enable_validation_layers) 
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
        create_info.ppEnabledLayerNames = m_validation_layers.data();
    } 
    else 
    {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device) != VK_SUCCESS) 
    {
        LOG_ERROR << "Failed to create logical device";
        return false;
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphics_queue);

    return true;
}
