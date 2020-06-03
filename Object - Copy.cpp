#include"Object.h"
Object::Object()
{
    //Initialize the collision box
    mBox.x = 12*32;
    mBox.y = 11*32;
    mBox.w = Object_WIDTH;
    mBox.h = Object_HEIGHT;
    frame = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Object::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY -= Object_VEL; frame = 3; break;
        case SDLK_DOWN: mVelY += Object_VEL; frame = 1; break;
        case SDLK_LEFT: mVelX -= Object_VEL; frame = 2; break;
        case SDLK_RIGHT: mVelX += Object_VEL; frame = 0; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY += Object_VEL; break;
        case SDLK_DOWN: mVelY -= Object_VEL; break;
        case SDLK_LEFT: mVelX += Object_VEL; break;
        case SDLK_RIGHT: mVelX -= Object_VEL; break;
        }
    }
}

void Object::move(Map *Maps[])
{
    //Move the Object left or right
    mBox.x += mVelX;

    //If the Object went too far to the left or right or touched a wall
    if(touchesWall(mBox, Maps))
    {
        //move back
        mBox.x -= mVelX;
    }
    // if over screen
    if ((mBox.x < 0))
    {
        mBox.x += SCREEN_WIDTH;

    }
    else
    {
        if (mBox.x + Object_WIDTH > SCREEN_WIDTH)
        {
            mBox.x -= SCREEN_WIDTH;
        }
    }

    //Move the Object up or down
    mBox.y += mVelY;

    //If the Object went too far up or down or touched a wall
    if ((mBox.y < 0) || (mBox.y + Object_HEIGHT > LEVEL_HEIGHT) || touchesWall(mBox, Maps))
    {
        //move back
        mBox.y -= mVelY;
    }
}

void Object::setCamera(SDL_Rect& camera)
{
    //Center the camera over the Object
    camera.x = (mBox.x + Object_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mBox.y + Object_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > LEVEL_WIDTH - camera.w)
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if (camera.y > LEVEL_HEIGHT - camera.h)
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

void Object::render(SDL_Rect& camera, SDL_Renderer* &des, Texture &gObjectTexture, SDL_Rect frame_[])
{
    //Show the Object
    gObjectTexture.render(mBox.x - camera.x, mBox.y - camera.y,des,&frame_[frame]);
}
bool Object:: touchesWall(SDL_Rect box, Map* Maps[])
{
    //Go through the Maps
    for (int i = 0; i < TOTAL_MapS; ++i)
    {
        //If the Map is a wall type Map
        if ((Maps[i]->getType() == 1))
        {
            //If the collision box touches the wall Map
            if (checkCollision(box, Maps[i]->getBox()))
            {
                return true;
            }
        }
    }

    //If no wall Maps were touched
    return false;
}