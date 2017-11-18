#include <stdbool.h>
#include <string.h>
#include "sway/commands.h"
#include "sway/container.h"
#include "sway/focus.h"
#include "sway/ipc-server.h"
#include "sway/layout.h"

struct cmd_results *cmd_fullscreen(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if (config->reading) return cmd_results_new(CMD_FAILURE, "fullscreen", "Can't be used in config file.");
	if (!config->active) return cmd_results_new(CMD_FAILURE, "fullscreen", "Can only be used when sway is running.");
	if ((error = checkarg(argc, "fullscreen", EXPECTED_AT_LEAST, 0))) {
		return error;
	}
	swayc_t *container = current_container;
	if(container->type != C_VIEW){
		return cmd_results_new(CMD_INVALID, "fullscreen", "Only views can fullscreen");
	}
	swayc_t *workspace = swayc_parent_by_type(container, C_WORKSPACE);
	bool current = swayc_is_fullscreen(container);
	// TODO WLR
	//wlc_view_set_state(container->handle, WLC_BIT_FULLSCREEN, !current);

	if (container->is_floating) {
		if (current) {
			// set dimensions back to what they were before we fullscreened this
			container->x = container->cached_geometry.x;
			container->y = container->cached_geometry.y;
			container->width = container->cached_geometry.width;
			container->height = container->cached_geometry.height;
		} else {
			// cache dimensions so we can reset them after we "unfullscreen" this
			/* TODO WLR
			struct wlc_geometry geo = {
				.origin = {
					.x = container->x,
					.y = container->y
				},
				.size = {
					.w = container->width,
					.h = container->height
				}
			};
			container->cached_geometry = geo;
			*/
		}
	}

	// Resize workspace if going from  fullscreen -> notfullscreen
	// otherwise just resize container
	if (!current) {
		arrange_windows(workspace, -1, -1);
		workspace->fullscreen = container;
	} else {
		arrange_windows(container, -1, -1);
		workspace->fullscreen = NULL;
	}
	ipc_event_window(container, "fullscreen_mode");

	return cmd_results_new(CMD_SUCCESS, NULL, NULL);
}
