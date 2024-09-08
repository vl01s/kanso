/* #include <math.h> */
#include <types.h>
#include <log.h>
#include <renderer.h>

static VkInstance instance;
static VkPhysicalDevice physical_device;

static void createVkInstance(void)
{
    VkApplicationInfo appinfo = {0};
    appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appinfo.pApplicationName = "OneDollarLiveVisuals";
    appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appinfo.pEngineName = "NoEngine";
    appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appinfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &appinfo;

    kso_uint32 property_count;
    vkEnumerateInstanceExtensionProperties(NULL, &property_count, NULL);
    property_count *= 2;
    VkExtensionProperties extension_properties[property_count];
    vkEnumerateInstanceExtensionProperties(NULL, &property_count,
            extension_properties);
    /* 24 Available Instance Extension Properties
     * VK_KHR_device_group_creation
     * VK_KHR_display
     * VK_KHR_external_fence_capabilities
     * VK_KHR_external_memory_capabilities
     * VK_KHR_external_semaphore_capabilities
     * VK_KHR_get_display_properties2
     * VK_KHR_get_physical_device_properties2
     * VK_KHR_get_surface_capabilities2
     * VK_KHR_surface
     * VK_KHR_surface_protected_capabilities
     * VK_KHR_wayland_surface
     * VK_KHR_xcb_surface
     * VK_KHR_xlib_surface
     * VK_EXT_acquire_drm_display
     * VK_EXT_acquire_xlib_display
     * VK_EXT_debug_report
     * VK_EXT_debug_utils
     * VK_EXT_direct_mode_display
     * VK_EXT_display_surface_counter
     * VK_EXT_headless_surface
     * VK_EXT_surface_maintenance1
     * VK_EXT_swapchain_colorspace
     * VK_KHR_portability_enumeration
     * VK_LUNARG_direct_driver_loading
     */
    create_info.enabledExtensionCount = 2;
    const char *const enabled_extensions[2] = {"VK_KHR_wayland_surface",
        "VK_KHR_surface"};
    create_info.ppEnabledExtensionNames = enabled_extensions;

    vkEnumerateInstanceLayerProperties(&property_count, NULL);
    property_count *= 2;
    VkLayerProperties layer_properties[property_count];
    vkEnumerateInstanceLayerProperties(&property_count, layer_properties);
    /* 9 Available Instance Layer Properties
     * VK_LAYER_VALVE_steam_overlay_32
     * VK_LAYER_VALVE_steam_overlay_64
     * VK_LAYER_VALVE_steam_fossilize_64
     * VK_LAYER_VALVE_steam_fossilize_32
     * VK_LAYER_NV_optimus
     * VK_LAYER_LUNARG_monitor
     * VK_LAYER_LUNARG_screenshot
     * VK_LAYER_LUNARG_api_dump
     * VK_LAYER_KHRONOS_validation
     */
    // TODO(vluis): Only enable validation layers if a flag is set (compile time)
    create_info.enabledLayerCount = 1;
    const char *const enabled_layers[1] = {"VK_LAYER_KHRONOS_validation"};
    create_info.ppEnabledLayerNames = enabled_layers;

    VkResult status = vkCreateInstance(&create_info, NULL, &instance);
    if (status != VK_SUCCESS) {
        verr("(createVkInstance): %s (VKResult == %d)\n",
                "Unable to create vk instance", status);
        return;
    }
}

void pickVkPhysicalDevice(void)
{
    // TODO(vluis): Implement a function that chooses the best possible physical
    // device
    kso_uint32 device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);
    device_count *= 2;
    VkPhysicalDevice devices[device_count];
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    physical_device = devices[0];

    // VkPhysicalDeviceProperties device_properties;
    // vkGetPhysicalDeviceProperties(physical_device, &device_properties);
    // printf("%s\n", device_properties.deviceName);
    // VkPhysicalDeviceFeatures device_features;
    // vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    kso_uint32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
            NULL);
    queue_family_count *= 2;
    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
            queue_families);
    for (kso_uint32 i = 0; i < queue_family_count; ++i) {
    }
}

void initVk(void)
{
    createVkInstance();
    /* setupVkDebugMessenger(); */
    pickVkPhysicalDevice();
}

void cleanup(void)
{
    vkDestroyInstance(instance, NULL);
}

bool rendererSafeToRead(void)
{
    return true;
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
