#ifndef Map_H
#define Map_H
#include"COMOMFUNC.h"
#include"Texture.h"
class Map
{
public:
	//Initializes position and type
	Map(int x, int y , int MapType);

	//Shows the Map
	void render(SDL_Rect& camera, SDL_Renderer*& des, Texture &gMapTexture,  SDL_Rect gMapClips[]);

	//Get the Map type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

private:
	//The attributes of the Map
	SDL_Rect mBox;

	//The Map type
	int mType;
};

#endif // !Map

