
#include"COMOMFUNC.h"

#include"Object.h"
#include"Text.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia( Map* Maps[] );
//init audio 
bool init_Audio(bool& success);

//Frees media and shuts down SDL
void close( Map* Maps[] , vector<Object*> Dots);

void RanDom(Object ghost[], const int VelGhost[]);
void ShowMenu(bool& quit,  Text Texts[], SDL_Rect& X);


//Sets Maps from Map map
bool setMaps( Map *Maps[] );
void initDots(vector<Object*>& Dots, Map* Maps[]);
void checkEndgame(Object Ghosts[], Object Pacman, bool& quit, vector<Object*> Dost);
void handleEatDot(Object Pacman, vector<Object*>& Dots);
void init_Possition_of_Ghost(Object Ghosts[]);

void Game();

// oObject 
Object BackgroundMenu;
Object Pacman;
Object Ghosts[TOTAL_GHOST];

Map* MapSet[TOTAL_MapS];
vector<Object*>Dots;

const int Velghost[4] = { -5,5,0 };// van toc cua ghost 
//Level camera
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//Scene textures
Texture gObjectTexture;
Texture gMapTexture;
Texture DotTexture;
Texture GhostTexture;
Texture Background;
SDL_Rect gMapClips[ TOTAL_Map_SPRITES ];
SDL_Rect frame[TOTAL_Pacman_SPRITES];
SDL_Rect frame_ghost[4];

TTF_Font* gFont = NULL;
Mix_Chunk* EatDot = NULL;

Mix_Chunk* Intro = NULL;
Mix_Chunk* Die = NULL;
Mix_Chunk* Wingame = NULL;






int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		
		
		
		//Load media
		if( !loadMedia( MapSet ) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			SDL_Event e_;
			bool quit = false;
			Text Texts[2];
			Texts[0].str_val_ = "Play";
			Texts[0].X = { 100, SCREEN_HEIGHT - 100,0,0 };
			Texts[0].LoadFromRenderText(gFont, gRenderer);


			Texts[1].str_val_ = "Exit";
			Texts[1].X = { 100, SCREEN_HEIGHT - 70,0,0 };
			Texts[1].LoadFromRenderText(gFont, gRenderer);
			SDL_Rect X = { 1,1,1,1 }; // de kiem tra vaa cham
			// 
			ShowMenu(quit, Texts, X );
			//Game();
			
		}
		
		//Free resources and close SDL
		close( MapSet, Dots);
	}

	return 0;
}
void RanDom(Object ghost[], const int VelGhost[]) { 
	int x, y;

	for (int i = 0; i < TOTAL_GHOST; i++)
	{
		srand(time(NULL));
		x = rand() % (2 - 0 + 1) + 0;
		ghost[i].mVelX = VelGhost[x];
		srand(time(NULL));
		y = rand() % (1 - 0 + 1) + 0;
		ghost[i].mVelY = VelGhost[y];
	}
}
	
void checkEndgame(Object Ghosts[], Object Pacman, bool& quit, vector<Object*> Dots)
{
	for (int i = 0; i < TOTAL_GHOST; i++)
	{
		if (checkCollision(Ghosts[i].mBox, Pacman.mBox) == true)
		{
			Mix_PlayChannel(-1, Die, 0);
			SDL_Delay(2000);
			quit = true;
		}
	}
	if (Dots.size() == 0)
	{
		// wingame
		Mix_PlayChannel(-1, Wingame, 0);
		SDL_Delay(5000);

		quit = true;
	}
}

bool init_Audio(bool& success)
{
	//Load music 
	EatDot = Mix_LoadWAV("amthanh/EatDot.wav");
	if (EatDot == NULL)
	{
		printf("\nFailed to load music eatDot set!\n");
		success = false;
	}
	Intro = Mix_LoadWAV("amthanh/Intro.wav");
	if (Intro == NULL)
	{
		printf("\nFailed to load music Intro set!\n");
		success = false;
	}
	Die= Mix_LoadWAV("amthanh/Die.wav");
	if (Die == NULL)
	{
		printf("\nFailed to load music Die set!\n");
		success = false;
	}
	Wingame = Mix_LoadWAV("amthanh/wingame.wav");
	if (Wingame == NULL)
	{
		printf("\nFailed to load music Die set!\n");
		success = false;
	}
	return success;
}
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//init  SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError();
					success = false;
				}
				//init TTF
				if (TTF_Init() == -1)
				{
					cout << "SDL_ttf could not initialize! SDL_ttf Error: %s\n" << TTF_GetError();
					success = false;
				}

			}
		}
	}

	return success;
}

bool loadMedia(Map* Maps[])
{
	//Loading success flag
	bool success = true;
	// Load  backgroundme nu
	if (!Background.loadFromFile("bacgroundMenu.png", gRenderer))
	{
		success = false;
		printf("Failed to load Bacground texture!\n");
	}
	//Load Object texture
	if (!gObjectTexture.loadFromFile("pacman.png", gRenderer))
	{
		printf("Failed to load Object texture!\n");
		success = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_Pacman_SPRITES; i++)
		{
			frame[i].x = i * 25;
			frame[i].y = 0;
			frame[i].w = 25;
			frame[i].h = 25;

		}
	}

	// Load Ghost Texture
	if (!GhostTexture.loadFromFile("ghost.png", gRenderer))
	{
		printf("Failed to load Ghost texture!\n");
		success = false;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			frame_ghost[i].x = i * 25;
			frame_ghost[i].y = 0;
			frame_ghost[i].w = 25;
			frame_ghost[i].h = 25;

		}
	}
	//Load Map texture
	if (!gMapTexture.loadFromFile("map.png", gRenderer))
	{
		printf("Failed to load Map set texture!\n");
		success = false;
	}
	//Load Dot texture 
	if (!DotTexture.loadFromFile("dot.png", gRenderer))
	{
		printf("Failed to load Dot  texture!\n");
		success = false;
	}

	//Load Map map
	if (!setMaps(Maps))
	{
		printf("Failed to load Map set!\n");

		success = false;
	}
	init_Audio(success);
	// load font 
	gFont = TTF_OpenFont("font/Fuente.ttf", 15);

	if (gFont == NULL)
	{
		cout << "Failed to load  font! SDL_ttf Error: %s\n" << TTF_GetError();
		success= false;
	}
	

	return success;
}

void close(Map* Maps[], vector<Object*> Dots)
{
	//Deallocate Maps
	for (int i = 0; i < TOTAL_MapS; ++i)
	{
		if (Maps[i] != NULL)
		{
			delete Maps[i];
			Maps[i] = NULL;
		}
	}
	int i = 0;
	while (i < Dots.size())
	{
		delete Dots[i];
		Dots[i] = NULL;
		Dots.erase(Dots.begin() + i);
	}

	//Free loaded images
	gObjectTexture.free();
	gMapTexture.free();
	DotTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
	TTF_Quit();

}


bool setMaps(Map* Maps[])
{
	//Success flag
	bool MapsLoaded = true;

	//The Map offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("filemap.txt");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		MapsLoaded = false;
	}
	else
	{
		//Initialize the Maps
		for (int i = 0; i < TOTAL_MapS; ++i)
		{
			//Determines what kind of Map will be made
			int MapType = -1;

			//Read Map from map file
			map >> MapType;
			if (i % 25 == 0)cout << "\n";

			cout << MapType << " ";

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				MapsLoaded = false;
				break;
			}

			//If the number is a valid Map number
			if ((MapType >= 0) && (MapType < TOTAL_Map_SPRITES))
			{
				Maps[i] = new Map(x, y, MapType);
			}

			//If we don't recognize the Map type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid Map type at %d!\n", i);
				MapsLoaded = false;
				break;
			}

			//Move to next Map spot
			x += Map_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += Map_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (MapsLoaded)
		{
			gMapClips[Map_BLACK].x = 0;
			gMapClips[Map_BLACK].y = 0;
			gMapClips[Map_BLACK].w = Map_WIDTH;
			gMapClips[Map_BLACK].h = Map_HEIGHT;

			gMapClips[Map_BLUE].x = 32;
			gMapClips[Map_BLUE].y = 0;
			gMapClips[Map_BLUE].w = Map_WIDTH;
			gMapClips[Map_BLUE].h = Map_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return MapsLoaded;
}

void initDots(vector<Object*>& Dots, Map* Maps[])
{
	for (int i = 0; i < TOTAL_MapS; i++)
	{
		if (Maps[i]->getType() == 0)
		{
			Object* newDot = new Object();
			newDot->mBox.x = Maps[i]->getBox().x + 14;
			newDot->mBox.y = Maps[i]->getBox().y + 14;
			newDot->mBox.w = 4;
			newDot->mBox.h = 4;
			Dots.push_back(newDot);
		}
	}
}
void handleEatDot(Object Pacman, vector<Object*>& Dots)
{

	int i = 0;

	while (i < Dots.size())
	{
		if (checkCollision(Pacman.getBox(), Dots[i]->getBox()) == true)
		{
			Dots.erase(Dots.begin() + i);//erase dot i
			// play music 
			Mix_PlayChannel(-1, EatDot, 0);
		}
		else
		{
			i++;
		}
	}
	// neeus xoas roi thi i khoong can ++ vi.........
}
void init_Possition_of_Ghost(Object Ghosts[])
{
	//init possition and frame of Ghost 
	for (int i = 0; i < TOTAL_GHOST; i++)
	{
		Ghosts[i].frame = i;
	}

	Ghosts[0].mBox.x = 2 * 32;
	Ghosts[0].mBox.y = 5 * 32;

	Ghosts[1].mBox.x = 22 * 32;
	Ghosts[1].mBox.y = 7 * 32;

	Ghosts[2].mBox.x = 4 * 32;
	Ghosts[2].mBox.y = 20 * 32;

	Ghosts[3].mBox.y = 22 * 32;
	Ghosts[3].mBox.y = 20 * 32;

}

void ShowMenu(bool &quit,Text Texts[], SDL_Rect &X )
{
	SDL_Event e;
	BackgroundMenu.mBox = {0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
	
	while (quit == false)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;

			}
			else
			{
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					SDL_GetMouseState(&X.x, &X.y);// get possition of mouse click 
					// check coli
					
					if (checkCollision(Texts[0].X, X)==true)
					{
						Game();
					}
					else
					{
						if (checkCollision(Texts[1].X, X) == true)
						{
							quit = true;
						}
					}
				}
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		BackgroundMenu.render(camera, gRenderer, Background, NULL);

		Texts[0].RenderText(gRenderer, 100, 100, NULL);
		Texts[1].RenderText(gRenderer, 100, 130, NULL);
		//Update screen
		SDL_RenderPresent(gRenderer);
	}
}
void Game()
{
	//Main loop flag
	bool quit = false;

	Pacman.mBox.y = 15 * 32;

	initDots(Dots, MapSet);
	//Event handler
	SDL_Event e;

	//The Object that will be moving around on the screen

	Mix_PlayChannel(-1, Intro, 0);

	init_Possition_of_Ghost(Ghosts);
	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//Handle input for the Object
			Pacman.handleEvent(e);

		}

		checkEndgame(Ghosts, Pacman, quit, Dots);
		//Move the Object
		Pacman.move(MapSet);

		RanDom(Ghosts, Velghost);

		for (int i = 0; i < TOTAL_GHOST; i++)
		{
			Ghosts[i].move(MapSet);
		}


		handleEatDot(Pacman, Dots);

		Pacman.setCamera(camera);

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render level
		for (int i = 0; i < TOTAL_MapS; ++i)
		{
			MapSet[i]->render(camera, gRenderer, gMapTexture, gMapClips);

		}

		// rener dots 
		int Dots_Size = Dots.size();

		for (int i = 0; i < Dots_Size; i++)
		{
			Dots[i]->render(camera, gRenderer, DotTexture, NULL);
		}

		//Render Pacman 
		Pacman.render(camera, gRenderer, gObjectTexture, frame);
		//render Ghost 
		for (int i = 0; i < TOTAL_GHOST; i++)
		{
			Ghosts[i].render(camera, gRenderer, GhostTexture, frame_ghost);
		}

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
}