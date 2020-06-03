
#include"Text.h"

Text::Text()
{
	text_color_ = { 255,0,0 };

	texture_ = NULL;
}

Text::~Text()
{

}

bool Text::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
	if (text_surface)
	{
		texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
		X.w = text_surface->w;
		X.h = text_surface->h;

		SDL_FreeSurface(text_surface);
	}

	return texture_ != NULL;
}

void Text::Free()
{
	if (texture_ != NULL)
	{
		SDL_DestroyTexture(texture_);
		texture_ = NULL;
	}
}


void Text::RenderText(SDL_Renderer* screen,int xp, int yp,SDL_Rect* clip)
{
	
	SDL_RenderCopy(screen, texture_, clip, &X);
}