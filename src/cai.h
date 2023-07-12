#pragma once
#include "cai_fwd_dec.h"
#include "SDL2/SDL.h"
#include <stdbool.h>

typedef unsigned int uint;

#define TRANSPARENT -1

#define CAI_ROOT_PANEL 0
#define CAI_PANEL 1
#define CAI_BUTTON 2
#define CAI_IMG_PANEL 4
#define CAI_TEXTAREA 8
#define CAI_TYPE uint

#define ORIENTATION_VERTICAL 1
#define ORIENTATION_HORIZONTAL 2
#define ORIENTATION_FLOW ORIENTATION_VERTICAL | ORIENTATION_HORIZONTAL


//------------------LAYOUT----------------------------------------

typedef void (*CAI_obj_layout_manager_destructor)(CAI_layout_manager* self);
typedef void (*CAI_impose_obj_layout)(CAI_obj* self, CAI_obj* parent);

#define CAI_generic_layout_manager_attrib	CAI_fixed_aspect_ratio* fx_ar;\
											CAI_impose_obj_layout impose_layout;\
											CAI_obj_layout_manager_destructor destructor;

typedef struct _CAI_fixed_aspect_ratio {
	float w_to_h_ratio;
	float rel_size;
} CAI_fixed_aspect_ratio;


typedef struct _CAI_layout_manager {
	CAI_generic_layout_manager_attrib
} CAI_layout_manager;

typedef struct _CAI_linear_layout {
	CAI_generic_layout_manager_attrib
	uint orientation;
} CAI_linear_layout;

typedef struct _CAI_relative_layout {
	CAI_generic_layout_manager_attrib
	float relx, rely, relh, relw;
} CAI_relative_layout;

typedef struct _CAI_box_layout {
	CAI_generic_layout_manager_attrib
	uint parent_rows, parent_cols;
	int row, col;
} CAI_box_layout;

CAI_fixed_aspect_ratio* CAI_create_fixed_aspect_ratio(float w_to_h_ratio, float rel_size);

CAI_layout_manager* _CAI_internal_create_layout_manager(size_t size, CAI_impose_obj_layout impose_layout_func, CAI_obj_layout_manager_destructor destructor, CAI_fixed_aspect_ratio* fx_ar);
CAI_layout_manager* CAI_create_relative_layout(float relx, float rely, float relw, float relh);
CAI_layout_manager* CAI_create_box_layout(uint parent_rows, uint parent_cols, int row, int col, CAI_fixed_aspect_ratio* far);
CAI_layout_manager* CAI_create_linear_layout(uint orientation, CAI_fixed_aspect_ratio* far);

void _CAI_dispose_layout_manager(CAI_layout_manager* lm);
void _CAI_dispose_relative_layout(CAI_layout_manager* lm);
void _CAI_dispose_box_layout(CAI_layout_manager* lm);
void _CAI_dispose_linear_layout(CAI_layout_manager* lm);

void _CAI_impose_relative_layout(CAI_obj* self, CAI_obj* parent);
void _CAI_impose_box_layout(CAI_obj* self, CAI_obj* parent);
void _CAI_impose_linear_layout(CAI_obj* self, CAI_obj* parent);

//-----------------------------------------------------------------

//-----------------CAI OBJS / GUI----------------------------------

typedef void (*CAI_on_click)(SDL_MouseButtonEvent* bt_event, CAI_context* context);
typedef void (*CAI_obj_destructor)(CAI_obj* self);
typedef void (*CAI_paint_obj)(CAI_obj* self, CAI_context* context);

#define CAI_generic_obj_attrib	int x, y; \
								uint w, h; \
								CAI_linked_list* children; \
								uint color; \
								uint border_color; \
								int id; \
								CAI_layout_manager* layout_manager;\
								CAI_paint_obj paint;\
								CAI_obj_destructor destructor;

typedef struct _CAI_context {
	SDL_Renderer* hrend;
	SDL_Window* hwin;
	CAI_obj* root;
} CAI_context;

typedef struct _rect {
	int x, y;
	int w, h;
} rect;

typedef struct _CAI_generic {
	CAI_generic_obj_attrib
} CAI_generic;

//wraps the CAI types
typedef struct _CAI_obj {
	CAI_generic* container;
	uint type;
} CAI_obj;

typedef struct _CAI_panel {
	CAI_generic_obj_attrib
} CAI_panel;

typedef struct _CAI_button {
	CAI_generic_obj_attrib
	CAI_on_click click_func;
} CAI_button;

typedef struct _CAI_img_panel {
	CAI_generic_obj_attrib
	CAI_img* image;
} CAI_img_panel;

typedef struct _CAI_textarea {
	CAI_generic_obj_attrib
	CAI_linked_list* text;
	CAI_img* text_image;
} CAI_textarea;

CAI_obj* _CAI_internal_create_cai_obj(size_t size, int id, uint color, CAI_paint_obj paint_func, CAI_obj_destructor destructor);

CAI_obj* CAI_create_panel(int id);
CAI_obj* CAI_create_button(int id, CAI_on_click func);
CAI_obj* CAI_create_textarea(int id);
CAI_obj* CAI_create_img_panel(int id, CAI_img* image);

void _CAI_internal_dispose_obj(CAI_obj* obj);

void _CAI_dispose_panel(CAI_obj* obj);
void _CAI_dispose_button(CAI_obj* obj);
void _CAI_dispose_textarea(CAI_obj* obj);
void _CAI_dispose_img_panel(CAI_obj* obj);

void _CAI_paint_panel(CAI_obj* obj, CAI_context* context);
void _CAI_paint_button(CAI_obj* obj, CAI_context* context);
void _CAI_paint_textarea(CAI_obj* obj, CAI_context* context);
void _CAI_paint_img_panel(CAI_obj* obj, CAI_context* context);

void CAI_add_child(CAI_obj* parent, CAI_obj* child);
CAI_obj* CAI_get_child(CAI_obj* parent, int id);
bool CAI_obj_contains(CAI_obj* obj, int x, int y);

//-----------------------------------------------------------------


//------------------- GENERAL ------------------------------------

bool CAI_delete_obj(CAI_obj* root, int id);
void _CAI_internal_delete_obj(CAI_obj* obj);

void CAI_impose_hierarchy_layout(CAI_obj* root, CAI_context* context);
void CAI_draw_hierarchy(CAI_obj* root, CAI_context* context);

void unpack_rgb(uint color, Uint8* r, Uint8* g, Uint8* b, Uint8* a);
uint pack_rgb(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

CAI_obj* CAI_dispatch_click(CAI_obj* root, SDL_MouseButtonEvent* bt_event, CAI_context* context);



#define white pack_rgb(255, 255, 255, 255)
#define black pack_rgb(0, 0, 0, 255)
#define red pack_rgb(255, 0, 0, 255)
#define green pack_rgb(0, 255, 0, 255)
#define blue pack_rgb(0, 0, 255, 255)

#define gray1 pack_rgb(30, 30, 30, 255)
#define gray2 pack_rgb(50, 50, 50, 255)
#define gray3 pack_rgb(75, 75, 75, 255)
#define gray4 pack_rgb(100, 100, 100, 255)
