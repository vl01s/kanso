#include <math.h>
#include <pxl-buff-ops.h>
#include <types.h>

// TODO(vluis): maybe reimplement this using GPU
void
bufferCopyStretch(void* src_mem,
                  const int src_width,
                  const int src_height,
                  const int src_stride,
                  void* dest_mem,
                  const int dest_width,
                  const int dest_height,
                  const int dest_stride)
{
  float width_ratio =
    KANSO_CAST(float, src_width) / KANSO_CAST(float, dest_width);
  float height_ratio =
    KANSO_CAST(float, src_height) / KANSO_CAST(float, dest_height);

  kso_uint8* row = KANSO_CAST(kso_uint8*, dest_mem);

  for (int y = 0; y < dest_height; ++y) {
    kso_uint32* pxl = KANSO_CAST(kso_uint32*, row);

    for (int x = 0; x < dest_width; ++x) {
      *pxl = *(KANSO_CAST(kso_uint32*, src_mem) +
               KANSO_CAST(int, roundf(y * height_ratio) * src_stride / 4) +
               KANSO_CAST(int, roundf(x * width_ratio)));
      ++pxl;
    }

    row += dest_stride;
  }
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
