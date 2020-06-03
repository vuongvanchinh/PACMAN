#ifndef Object_H
#define Object_H
#include"COMOMFUNC.h"
#include"map.h"
#include"Texture.h"
class Object
{
public:
	//The dimensions of the Object
	static const int Object_WIDTH = 20;
	static const int Object_HEIGHT = 20;
	
	//Maximum axis velocity of the Object
	static const int Object_VEL = 10;

	//Initializes the variables
	Object();
	bool touchesWall(SDL_Rect box, Map* Maps[]);
	
	//Takes key presses and adjusts the Object's velocity
	void handleEvent(SDL_Event& e);

	//Moves the Object and check collision against Maps
	void move(Map *Maps[]);

	//Centers the camera over the Object
	void setCamera(SDL_Rect& camera);

	//Shows the Object on the screen
	void render(SDL_Rect& camera, SDL_Renderer* &des, Texture &gObjectTexture, SDL_Rect frame[]);
	SDL_Rect getBox()
	{
		return mBox;
	}
	//Collision box of the Object
	SDL_Rect mBox;
	int frame;

	//The velocity of the Object
	int mVelX, mVelY;



};
#endif // !Object_H
