#pragma once
#include "cai_fwd_dec.h"
#include "SDL2/SDL.h"
#include "cai.h"
#include "cai_linked_list.h"
#include <stdbool.h>

void CAI_impose_hierarchy_layout(CAI_obj* root, CAI_context* context) {
	CAI_generic* g = root->container;

	if(root->type == CAI_ROOT_PANEL){
		SDL_Window* win = context->hwin;
		int w, h;
		SDL_GetWindowSize(win, &w, &h);
		g->x = w / 2;
		g->y = h / 2;
		g->w = w;
		g->h = h;
	}

	for (uint i = 0; i < g->children->length; i++) {
		CAI_obj* c = *((CAI_obj**)CAI_list_get(g->children, i));
		CAI_layout_manager* lm = c->container->layout_manager;

		if (lm != NULL) {
			lm->impose_layout(c, root);
		}

		CAI_impose_hierarchy_layout(c, context);
	}
}

void CAI_draw_hierarchy(CAI_obj* root, CAI_context* context) {
	CAI_generic* g = root->container;

	g->paint(root, context);

	for (uint i = 0; i < g->children->length; i++) {
		CAI_obj* c = *((CAI_obj**)CAI_list_get(g->children, i));

		CAI_draw_hierarchy(c, context);
	}
}

void unpack_rgb(uint color, Uint8* r, Uint8* g, Uint8* b, Uint8* a) {
	*r = (color >> 0) & 0xff;
	*g = (color >> 8) & 0xff;
	*b = (color >> 16) & 0xff;
	*a = (color >> 24) & 0xff;
}

uint pack_rgb(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	return	  (r << 0)
			| (g << 8)
			| (b << 16)
			| (a << 24);
}

CAI_obj* CAI_dispatch_click(CAI_obj* root, SDL_MouseButtonEvent* bt_event, CAI_context* context) {
	return NULL;
}

bool CAI_delete_obj(CAI_obj* root, int id) {
	if (root->container->id == id) {
		_CAI_internal_delete_obj(root);
		return true;
	}

	CAI_linked_list* children = root->container->children;

	for (uint i = 0; i < children->length; i++) {
		CAI_obj* c = *((CAI_obj**)CAI_list_get(children, i));

		bool res = CAI_delete_obj(c, id);
		if (res)
			return true;
	}

	return false;
}

void _CAI_internal_delete_obj(CAI_obj* obj) {
	//children first, then root

	CAI_linked_list* children = obj->container->children;
	for (uint i = 0; i < children->length; i++) {
		CAI_obj* c = *((CAI_obj**)CAI_list_get(children, i));

		_CAI_internal_delete_obj(c);
	}

	obj->container->destructor(obj);
}