#ifndef Text_H
#define Text_H
#include"COMOMFUNC.h"


class Text
{
public:
	Text();
	~Text();


	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
	void Free();

	
	void RenderText(SDL_Renderer* gRenderer,int x, int y,SDL_Rect* clip = NULL);

	
	

	string str_val_;
	SDL_Color text_color_;
	SDL_Texture* texture_;
	SDL_Rect X;



};

#endif
 // !Text

