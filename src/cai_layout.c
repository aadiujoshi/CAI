#pragma once
#include "cai_fwd_dec.h"
#include "cai.h"
#include "cai_linked_list.h"
#include <stdbool.h>

//--------------------------------------------------------------------------------------------
// 
//------------------------------  LAYOUT CREATION --------------------------------------------
// 
//--------------------------------------------------------------------------------------------

CAI_fixed_aspect_ratio* CAI_create_fixed_aspect_ratio(float w_to_h_ratio, float rel_size) {
	CAI_fixed_aspect_ratio* fxr = malloc(sizeof(CAI_fixed_aspect_ratio));
	fxr->w_to_h_ratio = w_to_h_ratio;
	fxr->rel_size = rel_size;
	return fxr;
}

CAI_layout_manager* _CAI_internal_create_layout_manager(size_t size, CAI_impose_obj_layout impose_layout_func, CAI_obj_layout_manager_destructor destructor, CAI_fixed_aspect_ratio* fx_ar) {
	CAI_layout_manager* lm = malloc(size);
	lm->impose_layout = impose_layout_func;
	lm->destructor = destructor;
	lm->fx_ar = fx_ar;
	return lm;
}

CAI_layout_manager* CAI_create_relative_layout(float relx, float rely, float relw, float relh) {
	CAI_relative_layout* lm = (CAI_relative_layout*)_CAI_internal_create_layout_manager(sizeof(CAI_relative_layout), _CAI_impose_relative_layout, _CAI_dispose_relative_layout, NULL);
	lm->relx = relx;
	lm->rely = rely;
	lm->relw = relw;
	lm->relh = relh;
	return lm;
}

CAI_layout_manager* CAI_create_box_layout(uint parent_rows, uint parent_cols, int row, int col, CAI_fixed_aspect_ratio* far) {
	CAI_box_layout* lm = _CAI_internal_create_layout_manager(sizeof(CAI_box_layout), _CAI_impose_box_layout, _CAI_dispose_box_layout, far);
	lm->parent_rows = parent_rows;
	lm->parent_cols = parent_cols;
	lm->row = row;
	lm->col = col;
	return lm;
}

CAI_layout_manager* CAI_create_linear_layout(uint orientation, CAI_fixed_aspect_ratio* far) {
	CAI_linear_layout* lm = _CAI_internal_create_layout_manager(sizeof(CAI_linear_layout), _CAI_impose_linear_layout, _CAI_dispose_linear_layout, far);
	lm->orientation = orientation;
	return lm;
}

//--------------------------------------------------------------------------------------------
// 
//------------------------------  LAYOUT DISPOSAL --------------------------------------------
// 
//--------------------------------------------------------------------------------------------

void _CAI_dispose_layout_manager(CAI_layout_manager* lm) {
	if (lm->fx_ar != NULL) {
		free(lm->fx_ar);
	}
	free(lm);
}

void _CAI_dispose_relative_layout(CAI_layout_manager* lm) {
	_CAI_dispose_layout_manager(lm);
}

void _CAI_dispose_box_layout(CAI_layout_manager* lm) {
	_CAI_dispose_layout_manager(lm);
}

void _CAI_dispose_linear_layout(CAI_layout_manager* lm) {
	_CAI_dispose_layout_manager(lm);
}


//--------------------------------------------------------------------------------------------
// 
//------------------------------  LAYOUT IMPOSE FUNCTIONS ------------------------------------
// 
//--------------------------------------------------------------------------------------------

void _CAI_impose_relative_layout(CAI_obj* self, CAI_obj* parent) {
	CAI_generic* cont = self->container;
	CAI_relative_layout* lm = cont->layout_manager;
	CAI_generic* pcont = parent->container;

	int origin_x = pcont->x - pcont->w / 2;
	int origin_y = pcont->y - pcont->h / 2;

	cont->x = lm->relx * pcont->w + origin_x;
	cont->y = lm->rely * pcont->h + origin_y;
	cont->w = lm->relw * pcont->w;
	cont->h = lm->relh * pcont->h;
}

void _CAI_impose_box_layout(CAI_obj* self, CAI_obj* parent) {
	CAI_generic* cont = self->container;
	CAI_box_layout* lm = cont->layout_manager;
	CAI_generic* pcont = parent->container;
	CAI_fixed_aspect_ratio* fxr = lm->fx_ar;

	int origin_x = pcont->x - pcont->w / 2;
	int origin_y = pcont->y - pcont->h / 2;

	float box_w = (1.0f * pcont->w / lm->parent_cols);
	float box_h = (1.0f * pcont->h / lm->parent_rows);

	cont->x = (lm->col * box_w) + (box_w / 2) + origin_x; 
	cont->y = (lm->row * box_h) + (box_h / 2) + origin_y;

	//width is greater than height (conform to width)
	if (fxr->w_to_h_ratio >= 1) {
		cont->w = box_w * fxr->rel_size;
		cont->h = box_w * (1 / fxr->w_to_h_ratio) * fxr->rel_size;
	}
	else {
		cont->w = box_h * fxr->w_to_h_ratio * fxr->rel_size;
		cont->h = box_h * fxr->rel_size;
	}
}

void _CAI_impose_linear_layout(CAI_obj* self, CAI_obj* parent) {
	CAI_generic* cont = self->container;
	CAI_linear_layout* lm = cont->layout_manager;
	CAI_generic* pcont = parent->container;
	CAI_fixed_aspect_ratio* fxr = lm->fx_ar;

	//conform 
	uint child_num = 0;
	for (uint i = 0; i < pcont->children->length; i++) {
		CAI_obj* c = *((CAI_obj**)CAI_list_get(pcont->children, i));

		if (c->container->id == self->container->id ) {
			child_num = i;
			break;
		}
	}

	uint orient = lm->orientation;

	if (lm->orientation & ORIENTATION_FLOW) {
		if (pcont->w >= pcont->h)
			orient = ORIENTATION_HORIZONTAL;
		else
			orient = ORIENTATION_VERTICAL;
	}

	int origin_x = pcont->x - pcont->w / 2;
	int origin_y = pcont->y - pcont->h / 2;

	float box_w = 0;
	float box_h = 0;

	if (orient & ORIENTATION_VERTICAL) {
		box_w = pcont->w;
		box_h = pcont->h / pcont->children->length;

		cont->x = box_w / 2 + origin_x;
		cont->y = child_num * box_h + box_h / 2 + origin_y;
	}

	if (orient & ORIENTATION_HORIZONTAL) {
		box_w = pcont->w / pcont->children->length;
		box_h = pcont->h;

		cont->x = child_num * box_w + box_w / 2 + origin_x;
		cont->y = box_h / 2 + origin_y;
	}

	if (fxr->w_to_h_ratio >= 1) {
		cont->w = box_w * fxr->rel_size;
		cont->h = box_w * (1 / fxr->w_to_h_ratio) * fxr->rel_size;
	}
	else {
		cont->w = box_h * fxr->w_to_h_ratio * fxr->rel_size;
		cont->h = box_h * fxr->rel_size;
	}
}

//--------------------------------------------------------------------------------------------
// 
//------------------------------  CAI_OBJ LAYOUT FUNCTIONS------------------------------------
// 
//--------------------------------------------------------------------------------------------

void CAI_set_layout_manager(CAI_obj* obj, CAI_layout_manager* new_lm) {
	CAI_layout_manager* lm = obj->container->layout_manager;

	if (lm != NULL) {
		lm->destructor(lm);
	}

	obj->container->layout_manager = lm;
}