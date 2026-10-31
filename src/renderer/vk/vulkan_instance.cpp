#include "vulkan_instance.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring>

namespace {

// Disable validation layers in Release Build
#ifdef NDEBUG
constexpr bool enable_validation_layers = false;
#else
constexpr bool enable_validation_layers = true;
#endif

// Contains all validation layers we want to use
const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> debug_extensions = {
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

bool check_validation_layer_support() {
    // Load all available layers
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    // Loop through all required layers and confirm they exist in available layers
    for (const char* required_layer : validation_layers) {
        bool found = false;

        for (const auto& layer : available_layers) {
            if (strcmp(required_layer, layer.layerName) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

}

VulkanInstance::VulkanInstance() {
    create_instance();

    if (enable_validation_layers) {
        setup_debug_messenger();
    }
}

VulkanInstance::~VulkanInstance() {
    if (debug_messenger != VK_NULL_HANDLE) {
        auto destroy_debug_messenger = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(handle, "vkDestroyDebugUtilsMessengerEXT"));
    
        if (destroy_debug_messenger) {
            destroy_debug_messenger(handle, debug_messenger, nullptr);
        }
    }

    if (handle != VK_NULL_HANDLE) {
        vkDestroyInstance(handle, nullptr);
    }
}

void VulkanInstance::create_instance() {
    if (enable_validation_layers && !check_validation_layer_support()) {
        throw std::runtime_error("Validation layers requested but are not available.");
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "ShardRenderer";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "ShardRenderer";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 2);
    app_info.apiVersion = VK_API_VERSION_1_0;

    uint32_t extension_count = 0;

    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    if (!glfw_extensions) {
        throw std::runtime_error("Failed to get required Vulkan extensions");
    }

    std::vector<const char*> extensions (glfw_extensions, glfw_extensions + extension_count);

    if (enable_validation_layers) {
        extensions.insert(
            extensions.end(),
            debug_extensions.begin(),
            debug_extensions.end()
        );
    }

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};

    if (enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());

        create_info.ppEnabledLayerNames = validation_layers.data();

        debug_create_info = get_debug_messenger_info();

        create_info.pNext = &debug_create_info;
    }

    VkResult result = vkCreateInstance(&create_info, nullptr, &handle);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance: " + std::to_string(static_cast<int>(result)));
    }

    std::cout << "Vulkan initialized successfully.\n";
}

VkDebugUtilsMessengerCreateInfoEXT VulkanInstance::get_debug_messenger_info() const {
    VkDebugUtilsMessengerCreateInfoEXT create_info{};

    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debug_callback;
    create_info.pUserData = nullptr;

    return create_info;
}

void VulkanInstance::setup_debug_messenger() {
    VkDebugUtilsMessengerCreateInfoEXT create_info = get_debug_messenger_info();
    auto create_debug_messenger = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(handle, "vkCreateDebugUtilsMessengerEXT")
    );

    if (!create_debug_messenger) {
        throw std::runtime_error("Failed to load vkCreateDebugUtilsMessengerEXT.");
    }

    VkResult result = create_debug_messenger(handle, &create_info, nullptr, &debug_messenger);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create debug messenger.");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
    (void)message_type;
    (void)user_data;

    if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cerr << "[Vulkan Validation] " << callback_data->pMessage << '\n';
    }

    return VK_FALSE;
}

VkInstance VulkanInstance::get_handle() const {
    return handle;
}