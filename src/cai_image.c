#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "cai_image.h"
#include "cai.h"

CAI_img* CAI_load_img(const char* path, CAI_context* ctx) {
	CAI_img* image = malloc(sizeof(CAI_img));

	if (image == NULL) {
		fprintf(stderr, "failed to init memory for, %s", path);
		return NULL;
	}

	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL) {
		//create empty surface
		surface = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
		fprintf(stderr, "could not find specified path, %s", path);
	}

	image->htex = SDL_CreateTextureFromSurface(ctx->hrend, surface);
	image->width = surface->w;
	image->height = surface->h;
	SDL_FreeSurface(surface);

	return image;
}

CAI_img* CAI_img_text(const char* text, const char* ttf_path, int color, int size, CAI_context* context) {
	TTF_Font* font = TTF_OpenFont(ttf_path, size);
	if (font == NULL) {
		fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
		return NULL;
	}

	Uint8 r, g, b, a;
	unpack_rgb(color, &r, &g, &b, &a);

	SDL_Color c = { r, g, b, a };

	SDL_Surface* surface = TTF_RenderText_Blended(font, text, c);
	if (surface == NULL) {
		fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
		TTF_CloseFont(font);
		return NULL;
	}
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(context->hrend, surface);
	if (texture == NULL) {
		fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
	}

	CAI_img* image = malloc(sizeof(CAI_img));

	if (image == NULL) {
		fprintf(stderr, "failed to init memory for, %s", ttf_path);
		return NULL;
	}

	image->htex = texture;
	image->width = surface->w;
	image->height = surface->h;

	SDL_FreeSurface(surface);
	TTF_CloseFont(font);

	return image;
}

void CAI_free_img(CAI_img* himg) {
	SDL_DestroyTexture(himg->htex);
	free(himg);
}