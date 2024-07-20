#ifndef KANSO_RENDERER_H
#define KANSO_RENDERER_H

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <vulkan/vulkan.h>
#include <log.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef STD_WIDTH
#define STD_WIDTH 1920
#endif // !STD_WIDTH
#ifndef STD_HEIGHT
#define STD_HEIGHT 1080
#endif // !STD_HEIGHT
#ifndef UNINITIALIZED_BUFFER
#define UNINITIALIZED_BUFFER -1
#endif // !UNINITIALIZED_BUFFER

static VkInstance instance;
static VkPhysicalDevice physical_device;

static void createVkInstance(void);
void pickVkPhysicalDevice(void);
void initVk(void);
void cleanup(void);
bool rendererSafeToRead(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !KANSO_RENDERER_H
