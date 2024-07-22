#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client.h>
#include <pxl-buff-ops.h>
#include <renderer.h>
#include <shm.h>
#include <log.h>
#include <xdg-shell-client-protocol.h>


/* structs */
typedef struct _client_buffer {
    int width;
    int height;
    int bytes_per_pxl;
    int stride;
    int size;
    int fd;
    uint32_t last_frame_time;
    uint32_t delta_t;
    struct wl_buffer* wl_buffer;
    void* mem;
} WlClientBuffer;

typedef struct _client_objects {
    /* global objects */
    struct wl_display* wl_display;
    struct wl_registry* wl_registry;
    struct wl_compositor* wl_compositor;
    struct wl_shm* wl_shm;
    struct xdg_wm_base* xdg_wm_base;
    /* local objects */
    struct wl_surface* wl_surface;
    struct xdg_surface* xdg_surface;
    struct xdg_toplevel* xdg_toplevel;
    WlClientBuffer client_buffer;
    WlClientBuffer renderer_buffer;
} WlClientObjects;


/* globals */
static WlClientObjects client_objs;
static const struct wl_buffer_listener wl_buffer_listener;
static const struct wl_callback_listener wl_frame_callback_listener;
static const struct xdg_surface_listener xdg_surface_listener;
static const struct xdg_toplevel_listener xdg_toplevel_listener;
static const struct xdg_wm_base_listener xdg_wm_base_listener;
static void (* app_exit_function)(void);


/* buffer functions */
static void createBuffer(const int width, const int height)
{
    WlClientBuffer* client_buffer = &client_objs.client_buffer;

    int bytes_per_pxl = client_objs.renderer_buffer.bytes_per_pxl;
    int stride = width * bytes_per_pxl;
    int size = width * height * bytes_per_pxl;

    client_buffer->width = width;
    client_buffer->height = height;
    client_buffer->bytes_per_pxl = bytes_per_pxl;
    client_buffer->stride = stride;
    client_buffer->size = size;
    int fd = allocateShm(size);
    if (fd < 0) {
        client_buffer->fd = -1;
        client_buffer->wl_buffer = 0;
    }
    else {
        client_buffer->fd = fd;
        struct wl_shm_pool* pool = wl_shm_create_pool(client_objs.wl_shm, fd, size);
        client_buffer->wl_buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride,
                WL_SHM_FORMAT_XRGB8888);
        wl_buffer_add_listener(client_buffer->wl_buffer, &wl_buffer_listener, client_buffer);
        wl_shm_pool_destroy(pool);
    }
}
static void paintClientSurface(void)
{
    WlClientBuffer* client_buffer = &client_objs.client_buffer;
    WlClientBuffer* renderer_buffer = &client_objs.renderer_buffer;
    void* client_buffer_mem = mmap(0, client_buffer->size, PROT_READ | PROT_WRITE,
            MAP_SHARED, client_buffer->fd, 0);
    bufferCopyStretch(renderer_buffer->mem, renderer_buffer->width, renderer_buffer->height,
            renderer_buffer->stride, client_buffer_mem, client_buffer->width, client_buffer->height,
            client_buffer->stride);
    munmap(client_buffer_mem, client_buffer->size);
    close(client_buffer->fd);
}


/**
 * Internal wayland functions and structs
 *
 * @param data The base data
 * @param xdg_wm_base The base struct
 * @param serial The ping serial
 */
static void xdg_wm_base_ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}
static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping
};

static void wl_registry_global(void* data, struct wl_registry* wl_registry, uint32_t name,
                               const char* interface, const uint32_t version)
{
    WlClientObjects* client_objs = data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        #define WL_COMPOSITOR_VERSION 5
        client_objs->wl_compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface,
                WL_COMPOSITOR_VERSION);
    }
    else if (strcmp(interface, wl_shm_interface.name) == 0) {
        #define WL_SHM_VERSION 1
        client_objs->wl_shm = wl_registry_bind(wl_registry, name, &wl_shm_interface,
               WL_SHM_VERSION);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        #define XDG_WM_BASE_VERSION 2
        client_objs->xdg_wm_base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface,
               XDG_WM_BASE_VERSION);
        xdg_wm_base_add_listener(client_objs->xdg_wm_base, &xdg_wm_base_listener, NULL);
    }
}
static void wl_registry_global_remove(void* data, struct wl_registry* wl_registry, const uint32_t name)
{
    // deliberately left blank
}
static const struct wl_registry_listener wl_registry_listener = {
    .global = wl_registry_global,
    .global_remove = wl_registry_global_remove,
};

static void xdg_surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial)
{
    WlClientObjects* client_objs = data;
    xdg_surface_ack_configure(xdg_surface, serial);
    if (client_objs->client_buffer.wl_buffer) {
        wl_surface_attach(client_objs->wl_surface, client_objs->client_buffer.wl_buffer, 0, 0);
        wl_surface_damage_buffer(client_objs->wl_surface, 0, 0, client_objs->client_buffer.width,
                client_objs->client_buffer.height);
    }
    wl_surface_commit(client_objs->wl_surface);
}
static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure
};

static void xdg_toplevel_configure(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width,
                                   int32_t height, struct wl_array* states)
{
    WlClientObjects* client_objs = data;
    // WlClientBuffer* client_buffer = &client_objs->client_buffer;
    WlClientBuffer* renderer_buffer = &client_objs->renderer_buffer;
    if (width == 0 && height == 0) {
        width = renderer_buffer->width;
        height = renderer_buffer->height;
    }
    createBuffer(width, height);

    if (rendererSafeToRead()) {
        paintClientSurface();
    }
}
static void xdg_toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel)
{
    // TODO(vluis): Send a message to the user to confirm exit
    app_exit_function();
}
static void xdg_toplevel_configure_bounds(void* data, struct xdg_toplevel* xdg_toplevel,
        int32_t width, int32_t height)
{
    // NOTE(DrKJeff16): deliberately left blank
}
static void xdg_toplevel_wm_capabilities(void* data, struct xdg_toplevel* xdg_toplevel,
        struct wl_array* capabilities)
{
    // NOTE(DrKJeff16): deliberately left blank
    // char* wm_capabilities_names[] = { "", "window_menu", "maximize", "fullscreen", "minimize" };
}
static const struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close,
    .configure_bounds = xdg_toplevel_configure_bounds,
    .wm_capabilities = xdg_toplevel_wm_capabilities
};

static void wl_buffer_release(void* data, struct wl_buffer* wl_buffer)
{
    WlClientBuffer* buffer = data;
    wl_buffer_destroy(wl_buffer);
    // TODO(vluis): Check wether the following cleanup is necessary
    buffer->fd = -1;
    buffer->wl_buffer = 0;
}
static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release
};

static void new_frame(void* data, struct wl_callback* wl_callback, uint32_t time)
{
    WlClientObjects* client_objs = data;
    WlClientBuffer* client_buffer = &client_objs->client_buffer;
    wl_callback_destroy(wl_callback);

    if (rendererSafeToRead()) {
        createBuffer(client_buffer->width, client_buffer->height);
        paintClientSurface();
        wl_surface_attach(client_objs->wl_surface, client_objs->client_buffer.wl_buffer, 0, 0);
        wl_surface_damage_buffer(client_objs->wl_surface, 0, 0, client_objs->client_buffer.width,
                client_objs->client_buffer.height);
    }

    struct wl_callback* frame_callback = wl_surface_frame(client_objs->wl_surface);
    wl_callback_add_listener(frame_callback, &wl_frame_callback_listener, client_objs);

    wl_surface_commit(client_objs->wl_surface);
}
static const struct wl_callback_listener wl_frame_callback_listener = {
    .done = new_frame
};


/* external wayland functions */
void wlSetRendererBuffer(void* mem, int width, int height)
{
    WlClientBuffer* buffer = &client_objs.renderer_buffer;
    buffer->width = width;
    buffer->height = height;
    buffer->mem = mem;
    buffer->bytes_per_pxl = 4;
    buffer->stride = width * buffer->bytes_per_pxl;
    buffer->size = buffer->stride * buffer->height;
}

void wlInitializeWindow(char* name, void* mem, const int width, const int height)
{
    client_objs.wl_display = wl_display_connect(NULL);
    // TODO(vluis): Check if we got a valid pointer to the wl_display and log it if otherwise

    if (client_objs.wl_display == NULL) {
        err_msg("Invalid pointer to wl_display");
    }

    client_objs.wl_registry = wl_display_get_registry(client_objs.wl_display);
    // TODO(vluis): Check if we got a valid pointer to the wl_registry and log it if otherwise
    if (client_objs.wl_registry == NULL) {
        err_msg("Invalid pointer to wl_registry");
    }

    wl_registry_add_listener(client_objs.wl_registry, &wl_registry_listener, &client_objs);
    wl_display_roundtrip(client_objs.wl_display);

    client_objs.wl_surface = wl_compositor_create_surface(client_objs.wl_compositor);
    // TODO(vluis): maybe bind a listener to wl_surface to catch errors
    client_objs.xdg_surface = xdg_wm_base_get_xdg_surface(client_objs.xdg_wm_base,
            client_objs.wl_surface);
    xdg_surface_add_listener(client_objs.xdg_surface, &xdg_surface_listener, &client_objs);
    client_objs.xdg_toplevel = xdg_surface_get_toplevel(client_objs.xdg_surface);
    xdg_toplevel_add_listener(client_objs.xdg_toplevel, &xdg_toplevel_listener, &client_objs);
    // xdg_toplevel_set_fullscreen(client_objs.xdg_toplevel, NULL);
    xdg_toplevel_set_title(client_objs.xdg_toplevel, name);

    struct wl_callback* frame_callback = wl_surface_frame(client_objs.wl_surface);
    wl_callback_add_listener(frame_callback, &wl_frame_callback_listener, &client_objs);

    wl_surface_commit(client_objs.wl_surface);

    wlSetRendererBuffer(mem, width, height);
}

void wlUpdate(void)
{
    wl_display_dispatch(client_objs.wl_display);
    wl_display_flush(client_objs.wl_display);
}

void wlSetExitCallback(void (* callback_function)(void))
{
    app_exit_function = callback_function;
}

// vim:ts=4:sts=4:sw=4:et:
