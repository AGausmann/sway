#ifndef _SWAY_SERVER_H
#define _SWAY_SERVER_H
#include <stdbool.h>
#include <wayland-server.h>
#include <wlr/backend.h>
#include <wlr/backend/session.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device_manager.h>
#include <wlr/types/wlr_xdg_shell_v6.h>
#include <wlr/render.h>
// TODO WLR: make Xwayland optional
#include <wlr/xwayland.h>

struct sway_server {
	struct wl_display *wl_display;
	struct wl_event_loop *wl_event_loop;
	const char *socket;

	struct wlr_backend *backend;
	struct wlr_renderer *renderer;

	struct wlr_compositor *compositor;
	struct wlr_data_device_manager *data_device_manager;

	struct sway_input *input;

	struct wl_listener output_add;
	struct wl_listener output_remove;
	struct wl_listener output_frame;

	struct wlr_xdg_shell_v6 *xdg_shell_v6;
	struct wl_listener xdg_shell_v6_surface;
};

struct sway_server server;

bool server_init(struct sway_server *server);
void server_fini(struct sway_server *server);
void server_run(struct sway_server *server);

void output_add_notify(struct wl_listener *listener, void *data);
void output_remove_notify(struct wl_listener *listener, void *data);

void handle_xdg_shell_v6_surface(struct wl_listener *listener, void *data);

#endif
