#pragma once
#include "cai_fwd_dec.h"
#include "SDL2/SDL.h"
#include "cai.h"
#include "cai_linked_list.h"
#include "cai_image.h"

//--------------------------------------------------------------------------------------------
// 
//------------------------------  OBJ CREATION FUNCTIONS--------------------------------------
// 
//--------------------------------------------------------------------------------------------

CAI_obj* _CAI_internal_create_cai_obj(size_t size, int id, uint color, CAI_paint_obj paint_func, CAI_obj_destructor destructor) {
	CAI_obj* co = malloc(size);
	CAI_generic* cg = (CAI_generic*) co;

	cg->children = CAI_create_list(sizeof(CAI_obj*));

	cg->color = color;
	cg->border_color = TRANSPARENT;
	cg->x = 0;
	cg->y = 0;
	cg->w = 0;
	cg->h = 0;
	cg->id = id;
	cg->layout_manager = NULL;

	cg->paint = paint_func;
	cg->destructor = destructor;

	return co;
}

CAI_obj* CAI_create_panel(int id) {
	CAI_obj* obj = _CAI_internal_create_cai_obj(sizeof(CAI_panel), id, TRANSPARENT, _CAI_paint_panel, _CAI_dispose_panel);
	obj->type = CAI_PANEL;
	return obj;
}

CAI_obj* CAI_create_button(int id, CAI_on_click func) {
	CAI_obj* obj = _CAI_internal_create_cai_obj(sizeof(CAI_button), id, TRANSPARENT, _CAI_paint_button, _CAI_dispose_button);
	((CAI_button*)obj->container)->click_func = func;
	obj->type = CAI_BUTTON;
	return obj;
}

CAI_obj* CAI_create_textarea(int id) {
	CAI_obj* obj = _CAI_internal_create_cai_obj(sizeof(CAI_textarea), id, TRANSPARENT, _CAI_paint_textarea, _CAI_dispose_textarea);
	CAI_textarea* tx = obj->container;
	tx->text = CAI_create_list(sizeof(char));
	tx->text_image = NULL;
	obj->type = CAI_TEXTAREA;
	return obj;
}

CAI_obj* CAI_create_img_panel(int id, CAI_img* image) {
	CAI_obj* obj = _CAI_internal_create_cai_obj(sizeof(CAI_img_panel), id, TRANSPARENT, _CAI_paint_img_panel, _CAI_dispose_img_panel);
	CAI_img_panel* ip = obj->container;
	ip->image = image;
	obj->type = CAI_IMG_PANEL;
	return obj;
}

CAI_obj* CAI_create_root_panel(int id) {
	CAI_obj* obj = CAI_create_panel(id);
	obj->type = CAI_ROOT_PANEL;
	return obj;
}

//--------------------------------------------------------------------------------------------
// 
//------------------------------  OBJ DISPOSE FUNCTIONS---------------------------------------
// 
//--------------------------------------------------------------------------------------------


void _CAI_internal_dispose_obj(CAI_obj* obj) {
	CAI_generic* cont = obj->container;

	CAI_free_list(cont->children);
	cont->layout_manager->destructor(cont->layout_manager);
	free(obj);
}

void _CAI_dispose_panel(CAI_obj* obj) {
	_CAI_internal_dispose_obj(obj);
}

void _CAI_dispose_button(CAI_obj* obj) {
	_CAI_internal_dispose_obj(obj);
}

void _CAI_dispose_textarea(CAI_obj* obj) {
	_CAI_internal_dispose_obj(obj);
}

void _CAI_dispose_img_panel(CAI_obj* obj) {
	CAI_img_panel* cont = (CAI_img_panel*)obj->container;
	CAI_free_img(cont->image);

	_CAI_internal_dispose_obj(obj);
}

//--------------------------------------------------------------------------------------------
// 
//------------------------------  PAINTING FUNCTIONS------------------------------------------
// 
//--------------------------------------------------------------------------------------------


void _CAI_paint_panel(CAI_obj* obj, CAI_context* context) {

}

void _CAI_paint_button(CAI_obj* obj, CAI_context* context) {

}

void _CAI_paint_textarea(CAI_obj* obj, CAI_context* context) {

}

void _CAI_paint_img_panel(CAI_obj* obj, CAI_context* context) {

}

//--------------------------------------------------------------------------------------------
// 
//------------------------------  OBJ MANAGEMENT FUNCTIONS------------------------------------
// 
//--------------------------------------------------------------------------------------------


void CAI_add_child(CAI_obj* parent, CAI_obj* child) {
	//pass double pointer (save 'child' address)
	CAI_list_add(parent->container->children, &child);
}

CAI_obj* CAI_get_child(CAI_obj* parent, int id) {
	if (parent->container->id == id)
		return parent;

	CAI_linked_list* children = parent->container->children;

	for (size_t i = 0; i < children->length; i++) {
		CAI_obj* c = *((CAI_obj**)CAI_list_get(children, i));
		
		CAI_obj* res = CAI_get_child(c, id);
		if (res)
			return res;
	}

	return NULL;
}

bool CAI_obj_contains(CAI_obj* obj, int x, int y) {
	CAI_generic* g = obj->container;

	float left = g->x - g->w / 2.0f;
	float right = g->x + g->w / 2.0f;
	float top = g->y + g->h / 2.0f;
	float bottom = g->y - g->h / 2.0f;

	return (x >= left && x <= right && y >= bottom && y <= top);
}

void CAI_change_img_panel_img(CAI_obj* img_view, CAI_img* new_image) {
	if (img_view->type != CAI_IMG_PANEL)
		return;

	CAI_img_panel* ip = (CAI_img_panel*) img_view->container;
	CAI_free_img(ip->image);
	ip->image = new_image;
}

void CAI_update_textarea_text(CAI_obj* textarea, SDL_Keycode key_typed, const char* ttf_path, uint color, int size, CAI_context* context) {
	CAI_textarea* ta = textarea->container;

	if (key_typed == SDLK_BACKSPACE) {
		if (ta->text->length == 0) 
			return;
		CAI_list_remove(ta->text, ta->text->length - 1);
	}
	else {
		char ch = (char)key_typed;
		CAI_list_add(ta->text, &ch);
	}

	//update text img
	if (ta->text_image != NULL) {
		CAI_free_img(ta->text_image);
	}

	//convert linked list to text char array
	//alloc on stack
	char* txt = alloca(sizeof(char) * (ta->text->length + 1));

	for (uint i = 0; i < ta->text->length; i++) {
		char ch = *((char*)CAI_list_get(ta->text, i));
		txt[i] = ch;
	}
	txt[ta->text->length] = ((char)'\0');

	ta->text_image = CAI_img_text(txt, ttf_path, color, size, context);
}