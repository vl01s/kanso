#ifndef KANSO_PXL_BUFF_OPS_H
#define KANSO_PXL_BUFF_OPS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void bufferCopyStretch(void *src_mem, const int src_width, const int src_height,
                       const int src_stride, void *dest_mem,
                       const int dest_width, const int dest_height,
                       const int dest_stride);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !KANSO_PXL_BUFF_OPS_H */

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:noet:
