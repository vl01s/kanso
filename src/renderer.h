#ifndef KANSO_RENDERER_H
#define KANSO_RENDERER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <vulkan/vulkan.h>

#ifndef STD_WIDTH
#define STD_WIDTH 1920
#endif // !STD_WIDTH
#ifndef STD_HEIGHT
#define STD_HEIGHT 1080
#endif // !STD_HEIGHT
#ifndef UNINITIALIZED_BUFFER
#define UNINITIALIZED_BUFFER -1
#endif // !UNINITIALIZED_BUFFER

static void createVkInstance(void);
void pickVkPhysicalDevice(void);
void initVk(void);
void cleanup(void);
bool rendererSafeToRead(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !KANSO_RENDERER_H

// vim:ts=4:sts=4:sw=4:et:
