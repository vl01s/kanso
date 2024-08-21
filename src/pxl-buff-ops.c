#include <math.h>
#include <pxl-buff-ops.h>
#include <stdint.h>

// TODO(vluis): maybe reimplement this using GPU
void bufferCopyStretch(void *src_mem, const int src_width, const int src_height,
                       const int src_stride, void *dest_mem,
                       const int dest_width, const int dest_height,
                       const int dest_stride)
{
    float width_ratio = (float)src_width / (float)dest_width;
    float height_ratio = (float)src_height / (float)dest_height;

    uint8_t *row = (uint8_t *)dest_mem;

    for (int y = 0; y < dest_height; ++y) {
        uint32_t *pxl = (uint32_t *)row;

        for (int x = 0; x < dest_width; ++x) {
            *pxl = *((uint32_t *)src_mem +
                    (int)(roundf(y * height_ratio) * src_stride / 4) +
                    (int)roundf(x * width_ratio));
            ++pxl;
        }

        row += dest_stride;
    }
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
