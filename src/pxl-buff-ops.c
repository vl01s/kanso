#include <math.h>
#include <stdint.h>
#include "pxl-buff-ops.h"

// TODO(vluis): maybe reimplement this using GPU
void bufferCopyStretch(void *src_mem, int src_width, int src_height, int src_stride,
    void *dest_mem, int dest_width, int dest_height, int dest_stride)
{
    float width_ratio = (float)src_width/(float)dest_width;
    float height_ratio = (float)src_height/(float)dest_height;

    uint8_t *row = dest_mem;
    for (int y = 0; y < dest_height; ++y) {
        uint32_t *pxl = (uint32_t *)row;
        for (int x = 0; x < dest_width; ++x) {
            *pxl = *((uint32_t *)src_mem + (int)(roundf(y * height_ratio) * src_stride / 4) +
                    (int)roundf(x * width_ratio));
            ++pxl;
        }
        row += dest_stride;
    }
}
