#pragma once
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "cai_fwd_dec.h"

typedef struct _CAI_img {
	SDL_Texture* htex;
	Uint32 width, height;
} CAI_img;

CAI_img* CAI_load_img(const char* path, CAI_context* context);
CAI_img* CAI_img_text(const char* text, const char* ttf_path, int color, int size, CAI_context* context);
void CAI_free_img(CAI_img* himg);