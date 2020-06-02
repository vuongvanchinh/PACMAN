
#include"COMOMFUNC.h"

#include"Object.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia( Map* Maps[] );

//Frees media and shuts down SDL
void close( Map* Maps[] );

//Box collision detector
//bool checkCollision( SDL_Rect a, SDL_Rect b );



//Sets Maps from Map map
bool setMaps( Map *Maps[] );


//Scene textures
Texture gObjectTexture;
Texture gMapTexture;
Texture test;
SDL_Rect gMapClips[ TOTAL_Map_SPRITES ];
SDL_Rect frame[TOTAL_Pacman_SPRITES];



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(Map *Maps[] )
{
	//Loading success flag
	bool success = true;

	//Load Object texture
	if( !gObjectTexture.loadFromFile( "pacman.png", gRenderer ) )
	{
		printf( "Failed to load Object texture!\n" );
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

	//Load Map texture
	if( !gMapTexture.loadFromFile( "map.png" , gRenderer) )
	{
		printf( "Failed to load Map set texture!\n" );
		success = false;
	}

	//Load Map map
	if( !setMaps(Maps) )
	{
		printf( "Failed to load Map set!\n" );

		success = false;
	}

	return success;
}

void close( Map* Maps[] )
{
	//Deallocate Maps
	for( int i = 0; i < TOTAL_MapS; ++i )
	{
		 if( Maps[ i ] != NULL )
		 {
			delete Maps[ i ];
			Maps[ i ] = NULL;
		 }
	}

	//Free loaded images
	gObjectTexture.free();
	gMapTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


bool setMaps( Map* Maps[] )
{
	//Success flag
	bool MapsLoaded = true;

    //The Map offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "filemap.txt" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		MapsLoaded = false;
    }
	else
	{
		//Initialize the Maps
		for( int i = 0; i < TOTAL_MapS; ++i )
		{
			//Determines what kind of Map will be made
			int MapType = -1;

			//Read Map from map file
			map >> MapType;
			if (i % 25 == 0)cout << "\n";

			cout << MapType << " ";

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				MapsLoaded = false;
				break;
			}

			//If the number is a valid Map number
			if( ( MapType >= 0 ) && ( MapType < TOTAL_Map_SPRITES ) )
			{
				Maps[ i ] = new Map( x, y, MapType );
			}
			//If we don't recognize the Map type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid Map type at %d!\n", i );
				MapsLoaded = false;
				break;
			}

			//Move to next Map spot
			x += Map_WIDTH;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += Map_HEIGHT;
			}
		}
		
		//Clip the sprite sheet
		if( MapsLoaded )
		{
			gMapClips[Map_BLACK ].x =0;
			gMapClips[Map_BLACK].y = 0;
			gMapClips[Map_BLACK].w = Map_WIDTH;
			gMapClips[Map_BLACK].h = Map_HEIGHT;

			gMapClips[ Map_BLUE ].x = 32;
			gMapClips[ Map_BLUE ].y = 0;
			gMapClips[ Map_BLUE ].w = Map_WIDTH;
			gMapClips[ Map_BLUE ].h = Map_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return MapsLoaded;
}



int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//The level Maps
		Map* MapSet[ TOTAL_MapS ];

		//Load media
		if( !loadMedia( MapSet ) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The Object that will be moving around on the screen
			Object Pacman;
		
			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the Object
					Pacman.handleEvent( e );
				}

				//Move the Object
				Pacman.move( MapSet );
				Pacman.setCamera( camera );

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render level
				for( int i = 0; i < TOTAL_MapS; ++i )
				{
					MapSet[ i ]->render( camera , gRenderer ,gMapTexture, gMapClips);
					
				}

				//Render Pacman 
				
				Pacman.render(camera, gRenderer, gObjectTexture, frame);

				
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
		
		//Free resources and close SDL
		close( MapSet );
	}

	return 0;
}