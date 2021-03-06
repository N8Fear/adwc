/*
 * Copyright © 2011 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <xcb/xcb.h>
#include <xcb/xfixes.h>
#include <xcb/xcb_icccm.h>

#include <wayland-server.h>

#include "adwc.h"
#include "xserver-server-protocol.h"
#include "hash.h"


struct weston_wm {
	xcb_connection_t *conn;
	const xcb_query_extension_reply_t *xfixes;
	struct wl_event_source *source;
	xcb_screen_t *screen;
	struct hash_table *window_hash;
	struct weston_xserver *server;
	xcb_window_t wm_window;

	xcb_window_t selection_window;
	int incr;
	int data_source_fd;
	struct wl_event_source *property_source;
	xcb_get_property_reply_t *property_reply;
	int property_start;
	struct wl_array source_data;
	xcb_selection_request_event_t selection_request;
	xcb_atom_t selection_target;
	xcb_timestamp_t selection_timestamp;
	int selection_property_set;
	int flush_property_on_delete;
	struct wl_listener selection_listener;

	struct {
		xcb_atom_t		 wm_protocols;
		xcb_atom_t		 wm_take_focus;
		xcb_atom_t		 wm_delete_window;
		xcb_atom_t		 wm_normal_hints;
		xcb_atom_t		 wm_size_hints;
		xcb_atom_t		 net_wm_name;
		xcb_atom_t		 net_wm_icon;
		xcb_atom_t		 net_wm_state;
		xcb_atom_t		 net_wm_state_fullscreen;
		xcb_atom_t		 net_wm_user_time;
		xcb_atom_t		 net_wm_icon_name;
		xcb_atom_t		 net_wm_window_type;
		xcb_atom_t		 net_wm_window_type_popup_menu;
		xcb_atom_t		 net_wm_moveresize;
		xcb_atom_t		 net_supporting_wm_check;
		xcb_atom_t		 net_supported;
		xcb_atom_t		 clipboard;
		xcb_atom_t		 targets;
		xcb_atom_t		 utf8_string;
		xcb_atom_t		 wl_selection;
		xcb_atom_t		 incr;
		xcb_atom_t		 timestamp;
		xcb_atom_t		 multiple;
		xcb_atom_t		 compound_text;
		xcb_atom_t		 text;
		xcb_atom_t		 string;
		xcb_atom_t		 text_plain_utf8;
		xcb_atom_t		 text_plain;
	} atom;
};

struct weston_wm_window {
	xcb_window_t id;
	tSurf *surface;
	struct shell_surface *shsurf;
	struct wl_listener surface_destroy_listener;
	char *class;
	char *name;
	struct weston_wm_window *transient_for;
	uint32_t protocols;
	xcb_atom_t type;
	xcb_size_hints_t size_hints;
};


static struct weston_wm_window *
get_wm_window(tSurf *surface);



WL_EXPORT  bool		weston_wm_window_resize			(struct weston_wm* wm, tSurf* es, int32_t x, int32_t y, int32_t width, int32_t height, bool hints)
{
	struct weston_wm_window* win = get_wm_window(es);
	
	if (!win)
		return win;
	
	/* Real client geometry, please keep it contained to C code at the very least. */
//	if (!c->fullscreen)
//		geometry_internal = titlebar_geometry_remove(c->titlebar, c->border_width, geometry);
//	else
//		geometry_internal = geometry;
	
//	if(hints && !c->fullscreen)
//		geometry_internal = client_geometry_hints(c, geometry_internal);
	
//	if(geometry_internal.width == 0 || geometry_internal.height == 0)
//		return false;
	
	/* Also let client hints propagate to the "official" geometry. */
//	if (!c->fullscreen)
//		geometry = titlebar_geometry_add(c->titlebar, c->border_width, geometry_internal);
//	else
//		geometry = geometry_internal;
	
	if (1/*c->geometries.internal.x != geometry_internal.x
		|| c->geometries.internal.y != geometry_internal.y
		|| c->geometries.internal.width != geometry_internal.width
		|| c->geometries.internal.height != geometry_internal.height*/)
	{
	//	screen_t *new_screen = screen_getbycoord(c->screen,
	//	geometry_internal.x, geometry_internal.y);

		/* Values to configure a window is an array where values are
		* stored according to 'value_mask' */
		uint32_t values[4];

		values[0] = x;
		values[1] = y;
		values[2] = width;
		values[3] = height;
		
		/* Also store geometry including border and titlebar. */
	//	c->geometry = geometry;
		
	//	titlebar_update_geometry(c);
		
		/* Ignore all spurious enter/leave notify events */
	//	client_ignore_enterleave_events();
		
		xcb_configure_window(wm->conn, win->id,
			XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y
			| XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
			values);
		
	//	client_restore_enterleave_events();
	//	screen_client_moveto(c, new_screen, false);
		/* execute hook */
	/*	hook_property(c, "geometry");

		luaA_object_push(globalconf.L, c);
		luaA_object_emit_signal(globalconf.L, -1, "property::geometry", 0);
		luaA_object_emit_signal(globalconf.L, -1, "property::x", 0);
		luaA_object_emit_signal(globalconf.L, -1, "property::y", 0);
		luaA_object_emit_signal(globalconf.L, -1, "property::width", 0);
		luaA_object_emit_signal(globalconf.L, -1, "property::height", 0);
		lua_pop(globalconf.L, 1);
		/**/
		return true;
	}
	return false;
}

static void
buf_print(uint8_t* buf, uint32_t size)
{
	fprintf (stderr, "\nbuf %lx\n", buf);
	int i, j;
	for (i = 0; i < size; ) {
		for (j = i; j < size && j < i+4; ++j) {
			fprintf (stderr, "%.2hhx:", buf[j]);
		}
		i = j;
		fprintf (stderr, "\n");
	}
	fprintf (stderr, "\n");
}


static const char *
get_atom_name(xcb_connection_t *c, xcb_atom_t atom)
{
	return "SkipIt";
	xcb_get_atom_name_cookie_t cookie;
	xcb_get_atom_name_reply_t *reply;
	xcb_generic_error_t *e;
	static char buffer[64];

	if (atom == XCB_ATOM_NONE)
		return "None";

	cookie = xcb_get_atom_name (c, atom);
	reply = xcb_get_atom_name_reply (c, cookie, &e);
	snprintf(buffer, sizeof buffer, "%.*s",
		 xcb_get_atom_name_name_length (reply),
		 xcb_get_atom_name_name (reply));
	free(reply);

	return buffer;
}

static void
dump_property(struct weston_wm *wm, xcb_atom_t property,
	      xcb_get_property_reply_t *reply)
{
	return;
	int32_t *incr_value;
	const char *text_value, *name;
	xcb_atom_t *atom_value;
	int width, len;
	uint32_t i;

	width = fprintf(stderr, "  %s: ", get_atom_name(wm->conn, property));
	if (reply == NULL) {
		fprintf(stderr, "(no reply)\n");
		return;
	}

	width += fprintf(stderr,
			 "type %s, format %d, length %d (value_len %d): ",
			 get_atom_name(wm->conn, reply->type),
			 reply->format,
			 xcb_get_property_value_length(reply),
			 reply->value_len);

	if (reply->type == wm->atom.incr) {
		incr_value = xcb_get_property_value(reply);
		fprintf(stderr, "%d\n", *incr_value);
	} else if (reply->type == wm->atom.utf8_string ||
	      reply->type == wm->atom.string) {
		text_value = xcb_get_property_value(reply);
		if (reply->value_len > 40)
			len = 40;
		else
			len = reply->value_len;
		fprintf(stderr, "\"%.*s\"\n", len, text_value);
	} else if (reply->type == XCB_ATOM_ATOM) {
		atom_value = xcb_get_property_value(reply);
		for (i = 0; i < reply->value_len; i++) {
			name = get_atom_name(wm->conn, atom_value[i]);
			if (width + strlen(name) + 2 > 78) {
				fprintf(stderr, "\n    ");
				width = 4;
			} else if (i > 0) {
				width += fprintf(stderr, ", ");
			}

			width += fprintf(stderr, "%s", name);
		}
		fprintf(stderr, "\n");
	} else {
		fprintf(stderr, "huh?\n");
	}
}

static void
dump_window_properties(struct weston_wm *wm, xcb_window_t window)
{
	return;
	xcb_list_properties_cookie_t list_cookie;
	xcb_list_properties_reply_t *list_reply;
	xcb_get_property_cookie_t property_cookie;
	xcb_get_property_reply_t *property_reply;
	xcb_atom_t *atoms;
	int i, length;

	list_cookie = xcb_list_properties(wm->conn, window);
	list_reply = xcb_list_properties_reply(wm->conn, list_cookie, NULL);
	if (!list_reply)
		/* Bad window, typically */
		return;

	length = xcb_list_properties_atoms_length(list_reply);
	atoms = xcb_list_properties_atoms(list_reply);

	for (i = 0; i < length; i++) {
		property_cookie =
			xcb_get_property(wm->conn,
					 0, /* delete */
					 window,
					 atoms[i],
					 XCB_ATOM_ANY,
					 0, 2048);

		property_reply = xcb_get_property_reply(wm->conn,
							property_cookie, NULL);
		dump_property(wm, atoms[i], property_reply);

		free(property_reply);
	}

	free(list_reply);
}

static void
data_offer_accept(struct wl_client *client, struct wl_resource *resource,
		  uint32_t time, const char *mime_type)
{
}

static void
data_offer_receive(struct wl_client *client, struct wl_resource *resource,
		   const char *mime_type, int32_t fd)
{
	struct wl_data_offer *offer = resource->data;
	struct weston_wm *wm = offer->source->resource.data;

	if (strcmp(mime_type, "text/plain;charset=utf-8") == 0) {
		/* Get data for the utf8_string target */
		xcb_convert_selection(wm->conn,
				      wm->selection_window,
				      wm->atom.clipboard,
				      wm->atom.utf8_string,
				      wm->atom.wl_selection,
				      XCB_TIME_CURRENT_TIME);

		xcb_flush(wm->conn);

		fcntl(fd, F_SETFL, O_WRONLY | O_NONBLOCK);
		wm->data_source_fd = fd;
	} else {
		close(fd);
	}
}

static void
data_offer_destroy(struct wl_client *client, struct wl_resource *resource)
{
	wl_resource_destroy(resource);
}

static const struct wl_data_offer_interface data_offer_interface = {
	data_offer_accept,
	data_offer_receive,
	data_offer_destroy,
};

static void
data_source_cancel(struct wl_data_source *source)
{
}

static void
weston_wm_get_selection_targets(struct weston_wm *wm)
{
	struct wl_data_source *source;
	struct weston_compositor *compositor;
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;
	xcb_atom_t *value;
	char **p;
	uint32_t i;

	cookie = xcb_get_property(wm->conn,
				  1, /* delete */
				  wm->selection_window,
				  wm->atom.wl_selection,
				  XCB_GET_PROPERTY_TYPE_ANY,
				  0, /* offset */
				  4096 /* length */);

	reply = xcb_get_property_reply(wm->conn, cookie, NULL);

	dump_property(wm, wm->atom.wl_selection, reply);

	if (reply->type != XCB_ATOM_ATOM) {
		free(reply);
		return;
	}

	source = malloc(sizeof *source);
	if (source == NULL)
		return;

	wl_signal_init(&source->resource.destroy_signal);
	source->offer_interface = &data_offer_interface;
	source->cancel = data_source_cancel;
	source->resource.data = wm;

	wl_array_init(&source->mime_types);
	value = xcb_get_property_value(reply);
	for (i = 0; i < reply->value_len; i++) {
		if (value[i] == wm->atom.utf8_string) {
			p = wl_array_add(&source->mime_types, sizeof *p);
			if (p)
				*p = strdup("text/plain;charset=utf-8");
		}
	}

	compositor = wm->server->compositor;
	wl_input_device_set_selection(compositor->input_device, source,
				      wl_display_next_serial(compositor->wl_display));

	free(reply);
}

static int
weston_wm_write_property(int fd, uint32_t mask, void *data)
{
	struct weston_wm *wm = data;
	unsigned char *property;
	int len, remainder;

	property = xcb_get_property_value(wm->property_reply);
	remainder = xcb_get_property_value_length(wm->property_reply) -
		wm->property_start;

	len = write(fd, property + wm->property_start, remainder);
	if (len == -1) {
		free(wm->property_reply);
		wl_event_source_remove(wm->property_source);
		close(fd);
		fprintf(stderr, "write error to target fd: %m\n");
		return 1;
	}

	fprintf(stderr, "wrote %d (chunk size %d) of %d bytes\n",
		wm->property_start + len,
		len, xcb_get_property_value_length(wm->property_reply));

	wm->property_start += len;
	if (len == remainder) {
		free(wm->property_reply);
		wl_event_source_remove(wm->property_source);

		if (wm->incr) {
			xcb_delete_property(wm->conn,
					    wm->selection_window,
					    wm->atom.wl_selection);
		} else {
			fprintf(stderr, "transfer complete\n");
			close(fd);
		}
	}

	return 1;
}

static void
weston_wm_get_selection_data(struct weston_wm *wm)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;

	cookie = xcb_get_property(wm->conn,
				  1, /* delete */
				  wm->selection_window,
				  wm->atom.wl_selection,
				  XCB_GET_PROPERTY_TYPE_ANY,
				  0, /* offset */
				  0x1fffffff /* length */);

	reply = xcb_get_property_reply(wm->conn, cookie, NULL);

	if (reply->type == wm->atom.incr) {
		dump_property(wm, wm->atom.wl_selection, reply);
		wm->incr = 1;
		free(reply);
	} else {
		dump_property(wm, wm->atom.wl_selection, reply);
		wm->incr = 0;
		wm->property_start = 0;
		wm->property_source =
			wl_event_loop_add_fd(wm->server->loop,
					     wm->data_source_fd,
					     WL_EVENT_WRITABLE,
					     weston_wm_write_property,
					     wm);
		wm->property_reply = reply;
	}
}

static void
weston_wm_get_incr_chunk(struct weston_wm *wm)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;

	cookie = xcb_get_property(wm->conn,
				  0, /* delete */
				  wm->selection_window,
				  wm->atom.wl_selection,
				  XCB_GET_PROPERTY_TYPE_ANY,
				  0, /* offset */
				  0x1fffffff /* length */);

	reply = xcb_get_property_reply(wm->conn, cookie, NULL);

	dump_property(wm, wm->atom.wl_selection, reply);

	if (xcb_get_property_value_length(reply) > 0) {
		wm->property_start = 0;
		wm->property_source =
			wl_event_loop_add_fd(wm->server->loop,
					     wm->data_source_fd,
					     WL_EVENT_WRITABLE,
					     weston_wm_write_property,
					     wm);
		wm->property_reply = reply;
	} else {
		fprintf(stderr, "transfer complete\n");
		close(wm->data_source_fd);
		free(reply);
	}
}

static void
weston_wm_set_selection(struct wl_listener *listener, void *data)
{
	struct wl_input_device *device = data;
	struct weston_wm *wm =
		container_of(listener, struct weston_wm, selection_listener);
	struct wl_data_source *source = device->selection_data_source;
	const char **p, **end;
	int has_text_plain = 0;

	if (source->offer_interface == &data_offer_interface)
		return;

	fprintf(stderr, "set selection\n");

	p = source->mime_types.data;
	end = (const char **)
		((char *) source->mime_types.data + source->mime_types.size);
	while (p < end) {
		fprintf(stderr, "  %s\n", *p);
		if (strcmp(*p, "text/plain") == 0 ||
		    strcmp(*p, "text/plain;charset=utf-8") == 0)
			has_text_plain = 1;
		p++;
	}

	if (has_text_plain) {
		xcb_set_selection_owner(wm->conn,
					wm->selection_window,
					wm->atom.clipboard,
					XCB_TIME_CURRENT_TIME);
	} else {
		xcb_set_selection_owner(wm->conn,
					XCB_ATOM_NONE,
					wm->atom.clipboard,
					XCB_TIME_CURRENT_TIME);
	}
}

static void
weston_wm_handle_configure_request(struct weston_wm *wm, xcb_generic_event_t *event)
{
	xcb_configure_request_event_t *configure_request = 
		(xcb_configure_request_event_t *) event;
	uint32_t values[16];
	int i = 0;
	
	fprintf(stderr, "XCB_CONFIGURE_REQUEST (window %d) %d,%d @ %dx%d\n",
		configure_request->window,
		configure_request->x, configure_request->y,
		configure_request->width, configure_request->height);
	
	if (configure_request->value_mask & XCB_CONFIG_WINDOW_X)
		values[i++] = configure_request->x;
	if (configure_request->value_mask & XCB_CONFIG_WINDOW_Y)
		values[i++] = configure_request->y;
	if (configure_request->value_mask & XCB_CONFIG_WINDOW_WIDTH)
		values[i++] = configure_request->width;
	if (configure_request->value_mask & XCB_CONFIG_WINDOW_HEIGHT)
		values[i++] = configure_request->height;
	if (configure_request->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH)
		values[i++] = configure_request->border_width;
	if (configure_request->value_mask & XCB_CONFIG_WINDOW_SIBLING)
		values[i++] = configure_request->sibling;
	if (configure_request->value_mask & XCB_CONFIG_WINDOW_STACK_MODE)
		values[i++] = configure_request->stack_mode;
	
	xcb_configure_window(wm->conn,
			     configure_request->window,
			     configure_request->value_mask, values);
}

static void
weston_wm_handle_configure_notify(struct weston_wm *wm, xcb_generic_event_t *event)
{
	xcb_configure_notify_event_t *configure_notify = (xcb_configure_notify_event_t *) event;

	fprintf(stderr, "XCB_CONFIGURE_NOTIFY (window %d) %d,%d @ %dx%d\n",
		configure_notify->window,
		configure_notify->x, configure_notify->y,
		configure_notify->width, configure_notify->height);
	
	struct weston_wm_window *window = hash_table_lookup(wm->window_hash, configure_notify->window);
	
	if (window && window->surface) {
		weston_surface_configure (window->surface, window->surface->geometry.x, window->surface->geometry.y, configure_notify->width, configure_notify->height);
		Surf_input_regionSet (window->surface, 0, 0, configure_notify->width, configure_notify->height);
	}
}

static void
weston_wm_activate(struct weston_wm *wm,
		 struct weston_wm_window *window, xcb_timestamp_t time)
{
	xcb_client_message_event_t client_message;

	client_message.response_type = XCB_CLIENT_MESSAGE;
	client_message.format = 32;
	client_message.window = window->id;
	client_message.type = wm->atom.wm_protocols;
	client_message.data.data32[0] = wm->atom.wm_take_focus;
	client_message.data.data32[1] = XCB_TIME_CURRENT_TIME;

	xcb_send_event(wm->conn, 0, window->id, 
		       XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
		       (char *) &client_message);

	xcb_set_input_focus (wm->conn,
			     XCB_INPUT_FOCUS_POINTER_ROOT, window->id, time);
}

static void
weston_xserver_surface_activate(struct wl_listener *listener, void *data)
{
	tSurf *surface = data;
	struct weston_wm_window *window = get_wm_window(surface);
	struct weston_xserver *wxs =
		container_of(listener,
			     struct weston_xserver, activate_listener);

	if (window)
		weston_wm_activate(wxs->wm, window, XCB_TIME_CURRENT_TIME);
	else if (wxs && wxs->wm)
		xcb_set_input_focus (wxs->wm->conn,
				     XCB_INPUT_FOCUS_POINTER_ROOT,
				     XCB_NONE,
				     XCB_TIME_CURRENT_TIME);
}

static void
weston_wm_handle_map_request(struct weston_wm *wm, xcb_generic_event_t *event)
{
	xcb_map_request_event_t *map_request =
		(xcb_map_request_event_t *) event;

	fprintf(stderr, "XCB_MAP_REQUEST (window %d)\n", map_request->window);

	xcb_map_window(wm->conn, map_request->window);
}

/* We reuse some predefined, but otherwise useles atoms */
#define TYPE_WM_PROTOCOLS XCB_ATOM_CUT_BUFFER0



static void
weston_wm_handle_map_notify(struct weston_wm *wm, xcb_generic_event_t *event)
{
#define F(field) offsetof(struct weston_wm_window, field)
	const struct {
		xcb_atom_t atom;
		xcb_atom_t type;
		int offset;
	} props[] = {
		{ XCB_ATOM_WM_CLASS, XCB_ATOM_STRING, F(class) },
		{ XCB_ATOM_WM_TRANSIENT_FOR, XCB_ATOM_WINDOW, F(transient_for) },
		{ wm->atom.wm_normal_hints, XCB_ATOM_WM_NORMAL_HINTS, F(size_hints) },
	//	{ wm->atom.wm_normal_hints, XCB_ATOM_WM_SIZE_HINTS, F(size_hints) },
	//	{ wm->atom.wm_normal_hints, wm->atom.wm_size_hints, F(size_hints) },
		{ wm->atom.wm_protocols, TYPE_WM_PROTOCOLS, F(protocols) },
		{ wm->atom.net_wm_window_type, XCB_ATOM_ATOM, F(type) },
		{ wm->atom.net_wm_name, XCB_ATOM_STRING, F(name) },
	};
#undef F
	
	xcb_map_notify_event_t *map_notify = (xcb_map_notify_event_t *) event;
	xcb_get_property_cookie_t cookie[ARRAY_LENGTH(props)];
	xcb_get_property_reply_t *reply;
	struct weston_wm_window *window;
	void *p;
	uint32_t *xid;
	xcb_atom_t *atom;
	uint32_t i;
	
	fprintf(stderr, "XCB_MAP_NOTIFY (window %d)\n", map_notify->window);
	
	dump_window_properties(wm, map_notify->window);
	
	window = hash_table_lookup(wm->window_hash, map_notify->window);
	
	for (i = 0; i < ARRAY_LENGTH(props); i++)
		cookie[i] = xcb_get_property(wm->conn,
					     0, /* delete */
					     window->id,
					     props[i].atom,
					     XCB_ATOM_ANY, 0, 2048);
	
	for (i = 0; i < ARRAY_LENGTH(props); i++)  {
		reply = xcb_get_property_reply(wm->conn, cookie[i], NULL);
		if (!reply)
			/* Bad window, typically */
			continue;
		if (reply->type == XCB_ATOM_NONE) {
			/* No such property */
			free(reply);
			continue;
		}
	//	fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!! reply->type %d\n", reply->type);
		
		p = ((char *) window + props[i].offset);
		
		if (reply->type == XCB_ATOM_WM_SIZE_HINTS) {
			p = xcb_get_property_value(reply);
		//	fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!window->size_hints %lx\n", p);
		//	buf_print (p, sizeof(window->size_hints));
			window->size_hints = *(typeof(window->size_hints)*)p;
			buf_print (&window->size_hints, sizeof(window->size_hints));
		}else {
		switch (props[i].type) {
		case XCB_ATOM_STRING:
			/* FIXME: We're using this for both string and
			   utf8_string */
			*(char **) p =
				strndup(xcb_get_property_value(reply),
					xcb_get_property_value_length(reply));
			break;
		case XCB_ATOM_WINDOW:
			xid = xcb_get_property_value(reply);
			*(struct weston_wm_window **) p =
				hash_table_lookup(wm->window_hash, *xid);
			break;
		case XCB_ATOM_ATOM:
			atom = xcb_get_property_value(reply);
			*(xcb_atom_t *) p = *atom;
			break;
		case TYPE_WM_PROTOCOLS:
			break;
		case XCB_ATOM_WM_SIZE_HINTS:
			p = xcb_get_property_value(reply);
		//	fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!window->size_hints %lx\n", p);
			buf_print (p, sizeof(window->size_hints));
			window->size_hints = *(typeof(window->size_hints)*)p;
			buf_print (&window->size_hints, sizeof(window->size_hints));
			break;
		default:
			break;
		}
		}
		free(reply);
	}
/*	if (xwl_screen->shell) {
		xf86DrvMsgVerb(0, X_INFO, 0, "AEUEUEOUUEAEUEUEOUUEAEUEUEOUUE  xwl_realize_window  A666\n");
	//	xwl_window->shsurf = wl_shell_get_shell_surface (xwl_screen->shell, xwl_window->surface);
		
		if (xwl_window->shsurf) {
			xf86DrvMsgVerb(0, X_INFO, 0, "AEUEUEOUUEAEUEUEOUUEAEUEUEOUUE  xwl_realize_window  A333\n");
		//	wl_shell_surface_set_user_data(xwl_window->shsurf, xwl_window);
		//	wl_shell_surface_add_listener(xwl_window->shsurf, &xwl_shell_surface_listener, xwl_window);
		}
		wl_shell_surface_set_toplevel(xwl_window->shsurf);
	}/**/
/*	xcb_get_window_attributes_reply_t *attr;
	{
		xcb_get_window_attributes_cookie_t cookie = xcb_get_window_attributes (wm->conn, window->id);
		
		xcb_generic_error_t *e;
		attr = xcb_get_window_attributes_reply (wm->conn, cookie, &e);
		if (attr) {
			fprintf (stderr, "attr %d\n", attr
		}
	}*/
	
/*	struct wl_client *client,
			struct wl_resource *resource,
			uint32_t id,
			struct wl_resource *surface_resource
	*/
//	window->shsurf = shell_get_shell_surface (wm->server->client, (struct wl_resource*)&gShell, 0, (struct wl_resource*)window->surface);
/*	fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!window->size_hints	%x\n	%d %d\n	%d %d\n	%d %d\n	%d %d\n	%d %d\n",
		window->size_hints.flags,
		window->size_hints.x, window->size_hints.y, window->size_hints.width, window->size_hints.height,
		window->size_hints.min_width, window->size_hints.min_height,
		window->size_hints.max_width, window->size_hints.max_height,
		window->size_hints.base_width, window->size_hints.base_height
	);*/
	
	fprintf (stderr, "aoeuhtnsoheutnaesouhtneoashutsensoauntheotunseohautnseouhsnaeo Shell_get_surface\n");
	if (window->surface) {
		window->shsurf = Shell_get_surface(wm->server->client, (struct wl_resource*)window->surface);
		
		if (window->transient_for || window->type == wm->atom.net_wm_window_type_popup_menu) {
		//	shell_surface_set_toplevel (wm->server->client, (struct wl_resource*)window->shsurf);
		//	shell_surface_set_transient(wm->server->client, (struct wl_resource*)window->shsurf,
		//	    struct wl_resource *parent_resource,
		//	    int x, int y, uint32_t flags)
		/*	static void
			shell_surface_set_transient(struct wl_client *client,
						    struct wl_resource *resource,
						    struct wl_resource *parent_resource,
						    int x, int y, uint32_t flags)
			{
				struct shell_surface *shsurf = resource->data;
				tSurf *es = shsurf->surface;
				struct shell_surface *pshsurf = parent_resource->data;
				tSurf *pes = pshsurf->surface;

				if (reset_shell_surface_type(shsurf))
					return;

				shsurf->output = pes->output;
				weston_surface_set_position(es, pes->geometry.x + x,
								pes->geometry.y + y);

				shsurf->type = SHELL_SURFACE_TRANSIENT;
			}*/
			int basex = 0, basey = 0;
			
		/*	if (window->transient_for) {
				if (window->transient_for->shsurf && window->transient_for->shsurf->surface) {
					basex = window->transient_for->shsurf->surface->geometry.x;
					basey = window->transient_for->shsurf->surface->geometry.y;
				}else if (window->transient_for->surface) {
					basex = window->transient_for->surface->geometry.x;
					basey = window->transient_for->surface->geometry.y;
				}
				fprintf (stderr, "aoeuhtnsoheutnaesouhtneoashutsensoauntheotunseohautnseouhsnaeo %d %d\n", basex, basey);
			}*/
			
			Win_LSet (window->shsurf, L_eFloat);
		//	window->shsurf->type = SHELL_SURFACE_TRANSIENT;
			window->shsurf->type = SHELL_SURFACE_TOPLEVEL;
			
			if (window->size_hints.flags & XCB_SIZE_HINT_P_POSITION) {
				if (window->size_hints.flags & XCB_SIZE_HINT_P_SIZE) {
					weston_surface_configure(window->shsurf->surface,
						basex + window->size_hints.x,
						basey + window->size_hints.y,
						window->size_hints.width,
						window->size_hints.height
					);
				}else {
					weston_surface_set_position(window->shsurf->surface,
						basex + window->size_hints.x,
						basey + window->size_hints.y
					);
				}
			}
		}else {
		/*	window->shsurf->type = SHELL_SURFACE_TOPLEVEL;
			
			weston_surface_configure(window->shsurf->surface,
				window->size_hints.x,
				window->size_hints.y,
				window->size_hints.width,
				window->size_hints.height
			);*/
			shell_surface_set_toplevel (wm->server->client, (struct wl_resource*)window->shsurf);
		}
	}
	
	
	fprintf(stderr, "window %d: name %s, class %s, transient_for %d\n",
		window->id, window->name, window->class,
		window->transient_for ? window->transient_for->id : 0
	);
	fprintf(stderr, "type %s\n",
		get_atom_name(wm->conn, window->type)
	);
	
	weston_wm_activate(wm, window, XCB_TIME_CURRENT_TIME);
}

static void
weston_wm_handle_unmap_notify(struct weston_wm *wm, xcb_generic_event_t *event)
{
	xcb_unmap_notify_event_t *unmap_notify = (xcb_map_notify_event_t *) event;
	
	fprintf(stderr, "XCB_UNMAP_NOTIFY (window %d)\n", unmap_notify->window);
	
	struct weston_wm_window *window = hash_table_lookup(wm->window_hash, unmap_notify->window);
	
	if (!window) {
		fprintf(stderr, "XCB_UNMAP_NOTIFY no window\n");
		return;
	}
	if (window->surface) {
	//	window->shsurf = Shell_get_surface(wm->server->client, (struct wl_resource*)window->surface);
		
	}
}



static const size_t incr_chunk_size = 64 * 1024;

static void
weston_wm_send_selection_notify(struct weston_wm *wm, xcb_atom_t property)
{
	xcb_selection_notify_event_t selection_notify;

	memset(&selection_notify, 0, sizeof selection_notify);
	selection_notify.response_type = XCB_SELECTION_NOTIFY;
	selection_notify.sequence = 0;
	selection_notify.time = wm->selection_request.time;
	selection_notify.requestor = wm->selection_request.requestor;
	selection_notify.selection = wm->selection_request.selection;
	selection_notify.target = wm->selection_request.target;
	selection_notify.property = property;

	xcb_send_event(wm->conn, 0, /* propagate */
		       wm->selection_request.requestor,
		       XCB_EVENT_MASK_NO_EVENT, (char *) &selection_notify);
}

static void
weston_wm_send_targets(struct weston_wm *wm)
{
	xcb_atom_t targets[] = {
		wm->atom.timestamp,
		wm->atom.targets,
		wm->atom.utf8_string,
		/* wm->atom.compound_text, */
		wm->atom.text,
		/* wm->atom.string */
	};

	xcb_change_property(wm->conn,
			    XCB_PROP_MODE_REPLACE,
			    wm->selection_request.requestor,
			    wm->selection_request.property,
			    XCB_ATOM_ATOM,
			    32, /* format */
			    ARRAY_LENGTH(targets), targets);

	weston_wm_send_selection_notify(wm, wm->selection_request.property);
}

static void
weston_wm_send_timestamp(struct weston_wm *wm)
{
	xcb_change_property(wm->conn,
			    XCB_PROP_MODE_REPLACE,
			    wm->selection_request.requestor,
			    wm->selection_request.property,
			    XCB_ATOM_INTEGER,
			    32, /* format */
			    1, &wm->selection_timestamp);

	weston_wm_send_selection_notify(wm, wm->selection_request.property);
}

static int
weston_wm_flush_source_data(struct weston_wm *wm)
{
	int length;

	xcb_change_property(wm->conn,
			    XCB_PROP_MODE_REPLACE,
			    wm->selection_request.requestor,
			    wm->selection_request.property,
			    wm->selection_target,
			    8, /* format */
			    wm->source_data.size,
			    wm->source_data.data);
	wm->selection_property_set = 1;
	length = wm->source_data.size;
	wm->source_data.size = 0;

	return length;
}

static int
weston_wm_read_data_source(int fd, uint32_t mask, void *data)
{
	struct weston_wm *wm = data;
	int len, current, available;
	void *p;

	current = wm->source_data.size;
	if (wm->source_data.size < incr_chunk_size)
		p = wl_array_add(&wm->source_data, incr_chunk_size);
	else
		p = (char *) wm->source_data.data + wm->source_data.size;
	available = wm->source_data.alloc - current;

	len = read(fd, p, available);
	if (len == -1) {
		fprintf(stderr, "read error from data source: %m\n");
		weston_wm_send_selection_notify(wm, XCB_ATOM_NONE);
		wl_event_source_remove(wm->property_source);
		close(fd);
		wl_array_release(&wm->source_data);
	}

	fprintf(stderr, "read %d (available %d, mask 0x%x) bytes: \"%.*s\"\n",
		len, available, mask, len, (char *) p);

	wm->source_data.size = current + len;
	if (wm->source_data.size >= incr_chunk_size) {
		if (!wm->incr) {
			fprintf(stderr, "got %zu bytes, starting incr\n",
				wm->source_data.size);
			wm->incr = 1;
			xcb_change_property(wm->conn,
					    XCB_PROP_MODE_REPLACE,
					    wm->selection_request.requestor,
					    wm->selection_request.property,
					    wm->atom.incr,
					    32, /* format */
					    1, &incr_chunk_size);
			wm->selection_property_set = 1;
			wm->flush_property_on_delete = 1;
			wl_event_source_remove(wm->property_source);
			weston_wm_send_selection_notify(wm, wm->selection_request.property);
		} else if (wm->selection_property_set) {
			fprintf(stderr, "got %zu bytes, waiting for "
				"property delete\n", wm->source_data.size);

			wm->flush_property_on_delete = 1;
			wl_event_source_remove(wm->property_source);
		} else {
			fprintf(stderr, "got %zu bytes, "
				"property deleted, seting new property\n",
				wm->source_data.size);
			weston_wm_flush_source_data(wm);
		}
	} else if (len == 0 && !wm->incr) {
		fprintf(stderr, "non-incr transfer complete\n");
		/* Non-incr transfer all done. */
		weston_wm_flush_source_data(wm);
		weston_wm_send_selection_notify(wm, wm->selection_request.property);
		xcb_flush(wm->conn);
		wl_event_source_remove(wm->property_source);
		close(fd);
		wl_array_release(&wm->source_data);
		wm->selection_request.requestor = XCB_NONE;
	} else if (len == 0 && wm->incr) {
		fprintf(stderr, "incr transfer complete\n");

		wm->flush_property_on_delete = 1;
		if (wm->selection_property_set) {
			fprintf(stderr, "got %zu bytes, waiting for "
				"property delete\n", wm->source_data.size);
		} else {
			fprintf(stderr, "got %zu bytes, "
				"property deleted, seting new property\n",
				wm->source_data.size);
			weston_wm_flush_source_data(wm);
		}
		xcb_flush(wm->conn);
		wl_event_source_remove(wm->property_source);
		wm->data_source_fd = -1;
		close(fd);
	} else {
		fprintf(stderr, "nothing happened, buffered the bytes\n");
	}

	return 1;
}

static void
weston_wm_send_data(struct weston_wm *wm, xcb_atom_t target, const char *mime_type)
{
	struct wl_input_device *device = wm->server->compositor->input_device;
	int p[2];

	if (pipe2(p, O_CLOEXEC | O_NONBLOCK) == -1) {
		fprintf(stderr, "pipe2 failed: %m\n");
		weston_wm_send_selection_notify(wm, XCB_ATOM_NONE);
		return;
	}

	wl_array_init(&wm->source_data);
	wm->selection_target = target;
	wm->data_source_fd = p[0];
	wm->property_source = wl_event_loop_add_fd(wm->server->loop,
						   wm->data_source_fd,
						   WL_EVENT_READABLE,
						   weston_wm_read_data_source,
						   wm);

	wl_data_source_send_send(&device->selection_data_source->resource,
				 mime_type, p[1]);
	close(p[1]);
}

static void
weston_wm_send_incr_chunk(struct weston_wm *wm)
{
	fprintf(stderr, "property deleted\n");
	int length;

	wm->selection_property_set = 0;
	if (wm->flush_property_on_delete) {
		fprintf(stderr, "setting new property, %zu bytes\n",
			wm->source_data.size);
		wm->flush_property_on_delete = 0;
		length = weston_wm_flush_source_data(wm);

		if (wm->data_source_fd >= 0) {
			wm->property_source =
				wl_event_loop_add_fd(wm->server->loop,
						     wm->data_source_fd,
						     WL_EVENT_READABLE,
						     weston_wm_read_data_source,
						     wm);
		} else if (length > 0) {
			/* Transfer is all done, but queue a flush for
			 * the delete of the last chunk so we can set
			 * the 0 sized propert to signal the end of
			 * the transfer. */
			wm->flush_property_on_delete = 1;
			wl_array_release(&wm->source_data);
		} else {
			wm->selection_request.requestor = XCB_NONE;
		}
	}
}

static void
weston_wm_handle_selection_request(struct weston_wm *wm,
				 xcb_generic_event_t *event)
{
	xcb_selection_request_event_t *selection_request =
		(xcb_selection_request_event_t *) event;

	fprintf(stderr, "selection request, %s, ",
		get_atom_name(wm->conn, selection_request->selection));
	fprintf(stderr, "target %s, ",
		get_atom_name(wm->conn, selection_request->target));
	fprintf(stderr, "property %s\n",
		get_atom_name(wm->conn, selection_request->property));

	wm->selection_request = *selection_request;
	wm->incr = 0;
	wm->flush_property_on_delete = 0;

	if (selection_request->target == wm->atom.targets) {
		weston_wm_send_targets(wm);
	} else if (selection_request->target == wm->atom.timestamp) {
		weston_wm_send_timestamp(wm);
	} else if (selection_request->target == wm->atom.utf8_string ||
		   selection_request->target == wm->atom.text) {
		weston_wm_send_data(wm, wm->atom.utf8_string,
				  "text/plain;charset=utf-8");
	} else {
		fprintf(stderr, "can only handle UTF8_STRING targets...\n");
		weston_wm_send_selection_notify(wm, XCB_ATOM_NONE);
	}
}

static void
weston_wm_handle_property_notify(struct weston_wm *wm, xcb_generic_event_t *event)
{
	xcb_property_notify_event_t *property_notify =
		(xcb_property_notify_event_t *) event;

	if (property_notify->window == wm->selection_window) {
		if (property_notify->state == XCB_PROPERTY_NEW_VALUE &&
		    property_notify->atom == wm->atom.wl_selection &&
		    wm->incr)
			weston_wm_get_incr_chunk(wm);
	} else if (property_notify->window == wm->selection_request.requestor) {
		if (property_notify->state == XCB_PROPERTY_DELETE &&
		    property_notify->atom == wm->selection_request.property &&
		    wm->incr)
			weston_wm_send_incr_chunk(wm);
	} else if (property_notify->atom == XCB_ATOM_WM_CLASS) {
		fprintf(stderr, "wm_class changed\n");
	} else if (property_notify->atom == XCB_ATOM_WM_TRANSIENT_FOR) {
		fprintf(stderr, "wm_transient_for changed\n");
	} else if (property_notify->atom == wm->atom.wm_protocols) {
		fprintf(stderr, "wm_protocols changed\n");
	} else if (property_notify->atom == wm->atom.net_wm_name) {
		fprintf(stderr, "_net_wm_name changed\n");
	} else if (property_notify->atom == wm->atom.net_wm_user_time) {
		fprintf(stderr, "_net_wm_user_time changed\n");
	} else if (property_notify->atom == wm->atom.net_wm_icon_name) {
		fprintf(stderr, "_net_wm_icon_name changed\n");
	} else if (property_notify->atom == XCB_ATOM_WM_NAME) {
		fprintf(stderr, "wm_name changed\n");
	} else if (property_notify->atom == XCB_ATOM_WM_ICON_NAME) {
		fprintf(stderr, "wm_icon_name changed\n");
	} else {
		fprintf(stderr, "XCB_PROPERTY_NOTIFY: "
			"unhandled property change: %s\n",
			get_atom_name(wm->conn, property_notify->atom));
	}
}



static void
weston_wm_handle_create_notify(struct weston_wm *wm, xcb_generic_event_t *event)
{
	xcb_create_notify_event_t *create_notify = (xcb_create_notify_event_t *) event;
	struct weston_wm_window *window;
	uint32_t values[1];

	fprintf(stderr, "XCB_CREATE_NOTIFY (window %d)\n", create_notify->window);

	window = malloc(sizeof *window);
	if (window == NULL) {
		fprintf(stderr, "failed to allocate window\n");
		return;
	}

	values[0] = XCB_EVENT_MASK_PROPERTY_CHANGE;
	xcb_change_window_attributes(wm->conn, create_notify->window, XCB_CW_EVENT_MASK, values);
	
	fprintf(stderr, "XCB_CREATE_NOTIFY (window %d) prop %d %d %d %d\n", create_notify->window,
		create_notify->x, create_notify->y,
		create_notify->width, create_notify->height
	);
	window->size_hints.x = create_notify->x;
	window->size_hints.y = create_notify->y;
	window->size_hints.width = create_notify->width;
	window->size_hints.height = create_notify->height;
//	int16_t        x; /**<  */
//	int16_t        y; /**<  */
//	uint16_t       width; /**<  */
//	uint16_t       height; /**<  */
	
	memset(window, 0, sizeof *window);
	window->id = create_notify->window;
	hash_table_insert(wm->window_hash, window->id, window);
}

static void
weston_wm_handle_destroy_notify(struct weston_wm *wm, xcb_generic_event_t *event)
{
	fprintf(stderr, "weston_wm_handle_destroy_notify\n");
	xcb_destroy_notify_event_t *destroy_notify =
		(xcb_destroy_notify_event_t *) event;
	struct weston_wm_window *window;

	fprintf(stderr, "XCB_DESTROY_NOTIFY, win %d\n",
		destroy_notify->window);

	window = hash_table_lookup(wm->window_hash, destroy_notify->window);
	if (window == NULL) {
		fprintf(stderr, "destroy notify for unknow window %d\n",
			destroy_notify->window);
		return;
	}
	
	fprintf(stderr, "destroy window %p\n", window);
	hash_table_remove(wm->window_hash, window->id);
	if (window->surface)
		wl_list_remove(&window->surface_destroy_listener.link);
	free(window);
	fprintf(stderr, "weston_wm_handle_destroy_notify exit\n");
}



static void
weston_wm_handle_selection_notify(struct weston_wm *wm,
				xcb_generic_event_t *event)
{
	xcb_selection_notify_event_t *selection_notify =
		(xcb_selection_notify_event_t *) event;

	if (selection_notify->property == XCB_ATOM_NONE) {
		/* convert selection failed */
	} else if (selection_notify->target == wm->atom.targets) {
		weston_wm_get_selection_targets(wm);
	} else {
		weston_wm_get_selection_data(wm);
	}
}

static void
weston_wm_handle_xfixes_selection_notify(struct weston_wm *wm,
				       xcb_generic_event_t *event)
{
	xcb_xfixes_selection_notify_event_t *xfixes_selection_notify =
		(xcb_xfixes_selection_notify_event_t *) event;

	printf("xfixes selection notify event: owner %d\n",
	       xfixes_selection_notify->owner);

	/* We have to use XCB_TIME_CURRENT_TIME when we claim the
	 * selection, so grab the actual timestamp here so we can
	 * answer TIMESTAMP conversion requests correctly. */
	if (xfixes_selection_notify->owner == wm->selection_window) {
		wm->selection_timestamp = xfixes_selection_notify->timestamp;
		fprintf(stderr, "our window, skipping\n");
		return;
	}

	wm->incr = 0;
	xcb_convert_selection(wm->conn, wm->selection_window,
			      wm->atom.clipboard,
			      wm->atom.targets,
			      wm->atom.wl_selection,
			      xfixes_selection_notify->timestamp);

	xcb_flush(wm->conn);
}

static void
weston_wm_handle_client_message(struct weston_wm *wm,
				xcb_generic_event_t *event)
{
	xcb_client_message_event_t *client_message =
		(xcb_client_message_event_t *) event;

	fprintf(stderr, "got client message, type: %s\n",
		get_atom_name(wm->conn, client_message->type));
}

static int
weston_wm_handle_event(int fd, uint32_t mask, void *data)
{
	struct weston_wm *wm = data;
	xcb_generic_event_t *event;
	int count = 0;

	while (event = xcb_poll_for_event(wm->conn), event != NULL) {
		switch (event->response_type & ~0x80) {
		case XCB_CREATE_NOTIFY:
			weston_wm_handle_create_notify(wm, event);
			break;
		case XCB_MAP_REQUEST:
			weston_wm_handle_map_request(wm, event);
			break;
		case XCB_MAP_NOTIFY:
			weston_wm_handle_map_notify(wm, event);
			break;
		case XCB_UNMAP_NOTIFY:
		//	fprintf(stderr, "XCB_UNMAP_NOTIFY\n");
			weston_wm_handle_unmap_notify(wm, event);
			break;
		case XCB_CONFIGURE_REQUEST:
			weston_wm_handle_configure_request(wm, event);
			break;
		case XCB_CONFIGURE_NOTIFY:
			weston_wm_handle_configure_notify(wm, event);
			break;
		case XCB_DESTROY_NOTIFY:
			weston_wm_handle_destroy_notify(wm, event);
			break;
		case XCB_MAPPING_NOTIFY:
			fprintf(stderr, "XCB_MAPPING_NOTIFY\n");
			break;
		case XCB_PROPERTY_NOTIFY:
			weston_wm_handle_property_notify(wm, event);
			break;
		case XCB_SELECTION_NOTIFY:
			weston_wm_handle_selection_notify(wm, event);
			break;
		case XCB_SELECTION_REQUEST:
			weston_wm_handle_selection_request(wm, event);
			break;
		case XCB_CLIENT_MESSAGE:
			weston_wm_handle_client_message(wm, event);
			break;
		}

		switch (event->response_type - wm->xfixes->first_event) {
		case XCB_XFIXES_SELECTION_NOTIFY:
			weston_wm_handle_xfixes_selection_notify(wm, event);
			break;
		}


		free(event);
		count++;
	}

	xcb_flush(wm->conn);

	return count;
}

static void
wxs_wm_get_resources(struct weston_wm *wm)
{

#define F(field) offsetof(struct weston_wm, field)

	static const struct { const char *name; int offset; } atoms[] = {
		{ "WM_PROTOCOLS",	F(atom.wm_protocols) },
		{ "WM_TAKE_FOCUS",	F(atom.wm_take_focus) },
		{ "WM_DELETE_WINDOW",	F(atom.wm_delete_window) },
		{ "WM_NORMAL_HINTS",	F(atom.wm_normal_hints) },
		{ "WM_SIZE_HINTS",	F(atom.wm_size_hints) },
		{ "_NET_WM_NAME",	F(atom.net_wm_name) },
		{ "_NET_WM_ICON",	F(atom.net_wm_icon) },
		{ "_NET_WM_STATE",	F(atom.net_wm_state) },
		{ "_NET_WM_STATE_FULLSCREEN", F(atom.net_wm_state_fullscreen) },
		{ "_NET_WM_USER_TIME", F(atom.net_wm_user_time) },
		{ "_NET_WM_ICON_NAME", F(atom.net_wm_icon_name) },
		{ "_NET_WM_WINDOW_TYPE",			F(atom.net_wm_window_type) },
		{ "_NET_WM_WINDOW_TYPE_POPUP_MENU",		F(atom.net_wm_window_type_popup_menu) },
		{ "_NET_WM_MOVERESIZE",				F(atom.net_wm_moveresize) },
		{ "_NET_SUPPORTING_WM_CHECK",			F(atom.net_supporting_wm_check) },
		{ "_NET_SUPPORTED",     F(atom.net_supported) },
		{ "CLIPBOARD",		F(atom.clipboard) },
		{ "TARGETS",		F(atom.targets) },
		{ "UTF8_STRING",	F(atom.utf8_string) },
		{ "_WL_SELECTION",	F(atom.wl_selection) },
		{ "INCR",		F(atom.incr) },
		{ "TIMESTAMP",		F(atom.timestamp) },
		{ "MULTIPLE",		F(atom.multiple) },
		{ "UTF8_STRING"	,	F(atom.utf8_string) },
		{ "COMPOUND_TEXT",	F(atom.compound_text) },
		{ "TEXT",		F(atom.text) },
		{ "STRING",		F(atom.string) },
		{ "text/plain;charset=utf-8",	F(atom.text_plain_utf8) },
		{ "text/plain",		F(atom.text_plain) },
	};

	xcb_xfixes_query_version_cookie_t xfixes_cookie;
	xcb_xfixes_query_version_reply_t *xfixes_reply;
	xcb_intern_atom_cookie_t cookies[ARRAY_LENGTH(atoms)];
	xcb_intern_atom_reply_t *reply;
	uint32_t i;

	xcb_prefetch_extension_data (wm->conn, &xcb_xfixes_id);

	for (i = 0; i < ARRAY_LENGTH(atoms); i++)
		cookies[i] = xcb_intern_atom (wm->conn, 0,
					      strlen(atoms[i].name),
					      atoms[i].name);

	for (i = 0; i < ARRAY_LENGTH(atoms); i++) {
		reply = xcb_intern_atom_reply (wm->conn, cookies[i], NULL);
		*(xcb_atom_t *) ((char *) wm + atoms[i].offset) = reply->atom;
		free(reply);
	}

	wm->xfixes = xcb_get_extension_data(wm->conn, &xcb_xfixes_id);
	if (!wm->xfixes || !wm->xfixes->present)
		fprintf(stderr, "xfixes not available\n");

	xfixes_cookie = xcb_xfixes_query_version(wm->conn,
						 XCB_XFIXES_MAJOR_VERSION,
						 XCB_XFIXES_MINOR_VERSION);
	xfixes_reply = xcb_xfixes_query_version_reply(wm->conn,
						      xfixes_cookie, NULL);

	printf("xfixes version: %d.%d\n",
	       xfixes_reply->major_version, xfixes_reply->minor_version);

	free(xfixes_reply);
}

static void
weston_wm_create_wm_window(struct weston_wm *wm)
{
	static const char name[] = "Weston WM";

	wm->wm_window = xcb_generate_id(wm->conn);
	xcb_create_window(wm->conn,
			  XCB_COPY_FROM_PARENT,
			  wm->wm_window,
			  wm->screen->root,
			  0, 0,
			  10, 10,
			  0,
			  XCB_WINDOW_CLASS_INPUT_OUTPUT,
			  wm->screen->root_visual,
			  0, NULL);

	xcb_change_property(wm->conn,
			    XCB_PROP_MODE_REPLACE,
			    wm->wm_window,
			    wm->atom.net_supporting_wm_check,
			    XCB_ATOM_WINDOW,
			    32, /* format */
			    1, &wm->wm_window);

	xcb_change_property(wm->conn,
			    XCB_PROP_MODE_REPLACE,
			    wm->wm_window,
			    wm->atom.net_wm_name,
			    wm->atom.utf8_string,
			    8, /* format */
			    strlen(name), name);

	xcb_change_property(wm->conn,
			    XCB_PROP_MODE_REPLACE,
			    wm->screen->root,
			    wm->atom.net_supporting_wm_check,
			    XCB_ATOM_WINDOW,
			    32, /* format */
			    1, &wm->wm_window);

}

static struct weston_wm *
weston_wm_create(struct weston_xserver *wxs)
{
	struct wl_input_device *device;
	struct weston_wm *wm;
	struct wl_event_loop *loop;
	xcb_screen_iterator_t s;
	uint32_t values[1], mask;
	int sv[2];
	xcb_atom_t supported[1];

	wm = malloc(sizeof *wm);
	if (wm == NULL)
		return NULL;

	wm->server = wxs;
	wm->window_hash = hash_table_create();
	if (wm->window_hash == NULL) {
		free(wm);
		return NULL;
	}

	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, sv) < 0) {
		fprintf(stderr, "socketpair failed\n");
		hash_table_destroy(wm->window_hash);
		free(wm);
		return NULL;
	}

	xserver_send_client(wxs->resource, sv[1]);
	wl_client_flush(wxs->resource->client);
	close(sv[1]);
	
	/* xcb_connect_to_fd takes ownership of the fd. */
	wm->conn = xcb_connect_to_fd(sv[0], NULL);
	if (xcb_connection_has_error(wm->conn)) {
		fprintf(stderr, "xcb_connect_to_fd failed\n");
		close(sv[0]);
		hash_table_destroy(wm->window_hash);
		free(wm);
		return NULL;
	}

	s = xcb_setup_roots_iterator(xcb_get_setup(wm->conn));
	wm->screen = s.data;

	loop = wl_display_get_event_loop(wxs->wl_display);
	wm->source =
		wl_event_loop_add_fd(loop, sv[0],
				     WL_EVENT_READABLE,
				     weston_wm_handle_event, wm);
	wl_event_source_check(wm->source);

	wxs_wm_get_resources(wm);

	values[0] =
		XCB_EVENT_MASK_STRUCTURE_NOTIFY |
		XCB_EVENT_MASK_RESIZE_REDIRECT |
		XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
		XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
		XCB_EVENT_MASK_PROPERTY_CHANGE;
	xcb_change_window_attributes(wm->conn, wm->screen->root,
				     XCB_CW_EVENT_MASK, values);

	weston_wm_create_wm_window(wm);

	supported[0] = wm->atom.net_wm_moveresize;
	xcb_change_property(wm->conn,
			    XCB_PROP_MODE_REPLACE,
			    wm->screen->root,
			    wm->atom.net_supported,
			    XCB_ATOM_ATOM,
			    32, /* format */
			    ARRAY_LENGTH(supported), supported);

	wm->selection_request.requestor = XCB_NONE;

	wm->selection_window = xcb_generate_id(wm->conn);
	xcb_create_window(wm->conn,
			  XCB_COPY_FROM_PARENT,
			  wm->selection_window,
			  wm->screen->root,
			  0, 0,
			  10, 10,
			  0,
			  XCB_WINDOW_CLASS_INPUT_OUTPUT,
			  wm->screen->root_visual,
			  XCB_CW_EVENT_MASK, values);

	mask =
		XCB_XFIXES_SELECTION_EVENT_MASK_SET_SELECTION_OWNER |
		XCB_XFIXES_SELECTION_EVENT_MASK_SELECTION_WINDOW_DESTROY |
		XCB_XFIXES_SELECTION_EVENT_MASK_SELECTION_CLIENT_CLOSE;

	xcb_xfixes_select_selection_input(wm->conn, wm->selection_window,
					  wm->atom.clipboard, mask);

	xcb_flush(wm->conn);

	device = wxs->compositor->input_device;
	wm->selection_listener.notify = weston_wm_set_selection;
	wl_signal_add(&device->selection_signal, &wm->selection_listener);

	fprintf(stderr, "created wm\n");

	return wm;
}

static void
weston_wm_destroy(struct weston_wm *wm)
{
	/* FIXME: Free windows in hash. */
	hash_table_destroy(wm->window_hash);
	xcb_disconnect(wm->conn);
	wl_event_source_remove(wm->source);
	wl_list_remove(&wm->selection_listener.link);
	free(wm);
}

static int
weston_xserver_handle_event(int listen_fd, uint32_t mask, void *data)
{
	struct weston_xserver *mxs = data;
	char display[8], s[8];
	int sv[2], client_fd;

	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, sv) < 0) {
		fprintf(stderr, "socketpair failed\n");
		return 1;
	}

	mxs->process.pid = fork();
	switch (mxs->process.pid) {
	case 0:
		/* SOCK_CLOEXEC closes both ends, so we need to unset
		 * the flag on the client fd. */
		client_fd = dup(sv[1]);
		if (client_fd < 0)
			return 1;

		snprintf(s, sizeof s, "%d", client_fd);
		setenv("WAYLAND_SOCKET", s, 1);

		snprintf(display, sizeof display, ":%d", mxs->display);

		if (execl(XSERVER_PATH,
			  XSERVER_PATH,
			  display,
			  "-wayland",
			  "-rootless",
			  "-retro",
			  "-nolisten", "all",
			  "-terminate",
			  "-config", "/opt/wayland/etc/X11/xorg.conf",
			  NULL) < 0)
			fprintf(stderr, "exec failed: %m\n");
		exit(-1);

	default:
		fprintf(stderr, "forked X server, pid %d\n", mxs->process.pid);

		close(sv[1]);
		mxs->client = wl_client_create(mxs->wl_display, sv[0]);

		weston_watch_process(&mxs->process);

		wl_event_source_remove(mxs->abstract_source);
		wl_event_source_remove(mxs->unix_source);
		break;

	case -1:
		fprintf(stderr, "failed to fork\n");
		break;
	}

	return 1;
}

static void
weston_xserver_shutdown(struct weston_xserver *wxs)
{
	char path[256];

	snprintf(path, sizeof path, "/tmp/.X%d-lock", wxs->display);
	unlink(path);
	snprintf(path, sizeof path, "/tmp/.X11-unix/X%d", wxs->display);
	unlink(path);
	if (wxs->process.pid == 0) {
		wl_event_source_remove(wxs->abstract_source);
		wl_event_source_remove(wxs->unix_source);
	}
	close(wxs->abstract_fd);
	close(wxs->unix_fd);
	if (wxs->wm)
		weston_wm_destroy(wxs->wm);
	wxs->loop = NULL;
}

static void
weston_xserver_cleanup(struct weston_process *process, int status)
{
	struct weston_xserver *mxs =
		container_of(process, struct weston_xserver, process);

	mxs->process.pid = 0;
	mxs->client = NULL;
	mxs->resource = NULL;

	mxs->abstract_source =
		wl_event_loop_add_fd(mxs->loop, mxs->abstract_fd,
				     WL_EVENT_READABLE,
				     weston_xserver_handle_event, mxs);

	mxs->unix_source =
		wl_event_loop_add_fd(mxs->loop, mxs->unix_fd,
				     WL_EVENT_READABLE,
				     weston_xserver_handle_event, mxs);

	if (mxs->wm) {
		fprintf(stderr, "xserver exited, code %d\n", status);
		weston_wm_destroy(mxs->wm);
		mxs->wm = NULL;
	} else {
		/* If the X server crashes before it binds to the
		 * xserver interface, shut down and don't try
		 * again. */
		fprintf(stderr, "xserver crashing too fast: %d\n", status);
		weston_xserver_shutdown(mxs);
	}
}

static void
surface_destroy(struct wl_listener *listener, void *data)
{
	struct weston_wm_window *window =
		container_of(listener,
			     struct weston_wm_window, surface_destroy_listener);

	fprintf(stderr, "surface for xid %d destroyed\n", window->id);
}

static struct weston_wm_window *
get_wm_window(tSurf *surface)
{
	struct wl_resource *resource = &surface->surface.resource;
	struct wl_listener *listener;

	listener = wl_signal_get(&resource->destroy_signal, surface_destroy);
	if (listener)
		return container_of(listener, struct weston_wm_window, surface_destroy_listener);

	return NULL;
}

static void
xserver_set_window_id(struct wl_client *client, struct wl_resource *resource, struct wl_resource *surface_resource, uint32_t id)
{
	fprintf(stderr, "xserver_set_window_id\n");
	struct weston_xserver *wxs = resource->data;
	struct weston_wm *wm = wxs->wm;
	struct wl_surface *surface = surface_resource->data;
	struct weston_wm_window *window;

	if (client != wxs->client)
		return;
	
	window = NULL;
//	while (!window)
		window = hash_table_lookup(wm->window_hash, id);
	if (window == NULL)
	{
		fprintf(stderr, "set_window_id for unknown window %d\n", id);
		return;
	}
	
	fprintf(stderr, "set_window_id %d for surface %p\n", id, surface);
	
	window->surface = (tSurf *) surface;
	window->surface_destroy_listener.notify = surface_destroy;
	wl_signal_add(&surface->resource.destroy_signal, &window->surface_destroy_listener);
	fprintf(stderr, "xserver_set_window_id leave\n");
}

static const struct xserver_interface xserver_implementation = {
	xserver_set_window_id
};

static void
bind_xserver(struct wl_client *client,
	     void *data, uint32_t version, uint32_t id)
{
	struct weston_xserver *wxs = data;

	/* If it's a different client than the xserver we launched,
	 * don't start the wm. */
	if (client != wxs->client)
		return;
	
	wxs->resource = wl_client_add_object(client, &xserver_interface, &xserver_implementation, id, wxs);

	wxs->wm = weston_wm_create(wxs);
	if (wxs->wm == NULL) {
		fprintf(stderr, "failed to create wm\n");
	}

	xserver_send_listen_socket(wxs->resource, wxs->abstract_fd);
	xserver_send_listen_socket(wxs->resource, wxs->unix_fd);
}

static int
bind_to_abstract_socket(int display)
{
	struct sockaddr_un addr;
	socklen_t size, name_size;
	int fd;

	fd = socket(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (fd < 0)
		return -1;

	addr.sun_family = AF_LOCAL;
	name_size = snprintf(addr.sun_path, sizeof addr.sun_path,
			     "%c/tmp/.X11-unix/X%d", 0, display);
	size = offsetof(struct sockaddr_un, sun_path) + name_size;
	if (bind(fd, (struct sockaddr *) &addr, size) < 0) {
		fprintf(stderr, "failed to bind to @%s: %s\n",
			addr.sun_path + 1, strerror(errno));
		close(fd);
		return -1;
	}

	if (listen(fd, 1) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

static int
bind_to_unix_socket(int display)
{
	struct sockaddr_un addr;
	socklen_t size, name_size;
	int fd;

	fd = socket(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (fd < 0)
		return -1;

	addr.sun_family = AF_LOCAL;
	name_size = snprintf(addr.sun_path, sizeof addr.sun_path,
			     "/tmp/.X11-unix/X%d", display) + 1;
	size = offsetof(struct sockaddr_un, sun_path) + name_size;
	unlink(addr.sun_path);
	if (bind(fd, (struct sockaddr *) &addr, size) < 0) {
		fprintf(stderr, "failed to bind to %s (%s)\n",
			addr.sun_path, strerror(errno));
		close(fd);
		return -1;
	}

	if (listen(fd, 1) < 0) {
		unlink(addr.sun_path);
		close(fd);
		return -1;
	}

	return fd;
}

static int
create_lockfile(int display, char *lockfile, size_t lsize)
{
	char pid[16], *end;
	int fd, size;
	pid_t other;

	snprintf(lockfile, lsize, "/tmp/.X%d-lock", display);
	fd = open(lockfile, O_WRONLY | O_CLOEXEC | O_CREAT | O_EXCL, 0444);
	if (fd < 0 && errno == EEXIST) {
		fd = open(lockfile, O_CLOEXEC, O_RDONLY);
		if (fd < 0 || read(fd, pid, 11) != 11) {
			fprintf(stderr, "can't read lock file %s: %s\n",
				lockfile, strerror(errno));
			errno = EEXIST;
			return -1;
		}

		other = strtol(pid, &end, 0);
		if (end != pid + 10) {
			fprintf(stderr, "can't parse lock file %s\n",
				lockfile);
			close(fd);
			errno = EEXIST;
			return -1;
		}

		if (kill(other, 0) < 0 && errno == ESRCH) {
			/* stale lock file; unlink and try again */
			fprintf(stderr,
				"unlinking stale lock file %s\n", lockfile);
			close(fd);
			if (unlink(lockfile))
				/* If we fail to unlink, return EEXIST
				   so we try the next display number.*/
				errno = EEXIST;
			else
				errno = EAGAIN;
			return -1;
		}

		errno = EEXIST;
		return -1;
	} else if (fd < 0) {
		fprintf(stderr, "failed to create lock file %s: %s\n",
			lockfile, strerror(errno));
		return -1;
	}

	/* Subtle detail: we use the pid of the wayland
	 * compositor, not the xserver in the lock file. */
	size = snprintf(pid, sizeof pid, "%10d\n", getpid());
	if (write(fd, pid, size) != size) {
		unlink(lockfile);
		close(fd);
		return -1;
	}

	close(fd);

	return 0;
}

static void
weston_xserver_destroy(struct wl_listener *l, void *data)
{
	struct weston_xserver *wxs =
		container_of(l, struct weston_xserver, destroy_listener);

	if (!wxs)
		return;

	if (wxs->loop)
		weston_xserver_shutdown(wxs);

	free(wxs);
}

WL_EXPORT void*
weston_xserver_init(struct weston_compositor *compositor)
{
	struct wl_display *display = compositor->wl_display;
	struct weston_xserver *mxs;
	char lockfile[256], display_name[8];

	mxs = malloc(sizeof *mxs);
	memset(mxs, 0, sizeof *mxs);

	mxs->process.cleanup = weston_xserver_cleanup;
	mxs->wl_display = display;
	mxs->compositor = compositor;

	mxs->display = 0;

retry:
	if (create_lockfile(mxs->display, lockfile, sizeof lockfile) < 0) {
		if (errno == EAGAIN) {
			goto retry;
		} else if (errno == EEXIST) {
			mxs->display++;
			goto retry;
		} else {
			free(mxs);
			return 0;
		}
	}				

	mxs->abstract_fd = bind_to_abstract_socket(mxs->display);
	if (mxs->abstract_fd < 0 && errno == EADDRINUSE) {
		mxs->display++;
		unlink(lockfile);
		goto retry;
	}

	mxs->unix_fd = bind_to_unix_socket(mxs->display);
	if (mxs->unix_fd < 0) {
		unlink(lockfile);
		close(mxs->abstract_fd);
		free(mxs);
		return 0;
	}

	snprintf(display_name, sizeof display_name, ":%d", mxs->display);
	fprintf(stderr, "xserver listening on display %s\n", display_name);
	setenv("DISPLAY", display_name, 1);

	mxs->loop = wl_display_get_event_loop(display);
	mxs->abstract_source =
		wl_event_loop_add_fd(mxs->loop, mxs->abstract_fd,
				     WL_EVENT_READABLE,
				     weston_xserver_handle_event, mxs);
	mxs->unix_source =
		wl_event_loop_add_fd(mxs->loop, mxs->unix_fd,
				     WL_EVENT_READABLE,
				     weston_xserver_handle_event, mxs);

	wl_display_add_global(display, &xserver_interface, mxs, bind_xserver);

	mxs->destroy_listener.notify = weston_xserver_destroy;
	wl_signal_add(&compositor->destroy_signal, &mxs->destroy_listener);


	mxs->activate_listener.notify = weston_xserver_surface_activate;
	wl_signal_add(&compositor->activate_signal, &mxs->activate_listener);

	return mxs;
}
