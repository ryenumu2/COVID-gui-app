/////////////////////////////////////////////////////////////////////////////
// Name:        animation.cpp
// Purpose:     breathing app and timer internal functions
// Author:      Rohith Yenumula
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once

using namespace rppg_data_collection;

namespace animation
{
    //dot function definitions
    Dot::Dot()
    {
        //Initialize the offsets
        mPosX = 300;
        mPosY = 0;

        //Initialize the velocity
        mVelX = 0;
        mVelY = 4;
    }

    void Dot::move()
    {
        time_t current_time;

        current_time = time(NULL);

        const double PI = 3.14159265358979;
        const int radius = 10;
        const int BrPM = 12;

        unsigned long ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch())
                               .count();

        mPosY = (SCREEN_HEIGHT_BALL / 2 - radius) * sin(BrPM * 2 * PI / 60 * ((double)ms / 1000)) + SCREEN_HEIGHT_BALL / 2.09; //this will output the correct boundaries, but circle is not continuous!!

        //BrPM default: 12
        //dot radius: 10
        //const int SCREEN_WIDTH_BALL = 640;
        //const int SCREEN_HEIGHT_BALL = 480; : /2 is 240

        //If the dot went too far up or down
        if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT_BALL))
        {
            mVelY = -mVelY;
        }
    }

    void Dot::render()
    {
        //Show the dot
        gDotTexture.render(mPosX, mPosY);
    }

    //load dot.bmp into bouncing ball implementation
    bool LTexture::loadFromFile(std::string path)
    {
        //Get rid of preexisting texture
        free();

        //The final texture
        SDL_Texture *newTexture = NULL;

        //Load image at specified path
        SDL_Surface *loadedSurface = IMG_Load(path.c_str());

        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        //Get image dimensions
        mWidth = loadedSurface->w;
        mHeight = loadedSurface->h;

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);

        //Return success
        mTexture = newTexture;
        return mTexture != NULL;
    }

    void LTexture::free()
    {
        //Free texture if it exists
        if (mTexture != NULL)
        {
            SDL_DestroyTexture(mTexture);
            mTexture = NULL;
            mWidth = 0;
            mHeight = 0;
        }
    }

    void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
    {
        //Modulate texture rgb
        SDL_SetTextureColorMod(mTexture, red, green, blue);
    }

    LTexture::LTexture()
    {
        //Initialize
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }

    LTexture::~LTexture()
    {
        //Deallocate
        free();
    }

    void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = {x, y, mWidth, mHeight};

        //Set clip rendering dimensions
        if (clip != NULL)
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
    }

    //countdown function definitions
    void LTexture::render_countdown(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
    {
        //Set rendering space and render to screen. Note the distinct 70 parameter for int x
        SDL_Rect renderQuad = {70, y, mWidth, mHeight};

        //Set clip rendering dimensions
        if (clip != NULL)
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }
        //Render to screen
        SDL_RenderCopyEx(gRenderer_countdown, mTexture, clip, &renderQuad, angle, center, flip);
    }

    int LTexture::getWidth()
    {
        return mWidth;
    }

    int LTexture::getHeight()
    {
        return mHeight;
    }

    #if defined(SDL_TTF_MAJOR_VERSION)
    bool LTexture::loadFromRenderedText_countdown(std::string textureText, SDL_Color textColor)
    {

        //Get rid of preexisting texture
        free();

        //Render text surface
        SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
        if (textSurface != NULL)
        {
            //Create texture from surface pixels
            mTexture = SDL_CreateTextureFromSurface(gRenderer_countdown, textSurface);
            if (mTexture == NULL)
            {
                printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                //Get image dimensions
                mWidth = textSurface->w;
                mHeight = textSurface->h;
            }

            //Get rid of old surface
            SDL_FreeSurface(textSurface);
        }
        else
        {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        }

        //Return success
        return mTexture != NULL;
    }
#endif //second endif

    bool MiscFuncs::init()
    {
        
        //Initialization flag
        bool success = true;

        TTF_Init();

        TTF_Font *font = TTF_OpenFont("arial.ttf", 25);

        SDL_Color color = {100, 100, 100};
        SDL_Surface *surface = TTF_RenderText_Solid(font, "IT works", color);

        SDL_Renderer *renderer = SDL_CreateRenderer(gWindow, -1, 0);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        //Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
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
            gWindow = SDL_CreateWindow("Breathing Rate", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_BALL, SCREEN_HEIGHT_BALL, SDL_WINDOW_SHOWN);
            if (gWindow == NULL)
            {
                printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Create vsynced renderer for window
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                if (gRenderer == NULL)
                {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                }
                else
                {
                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags))
                    {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                        success = false;
                    }
                }
            }
        }

        return success;
    }

    bool MiscFuncs::loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load dot texture: essential for ball
        if (!gDotTexture.loadFromFile("dot.bmp"))
        {
            printf("Failed to load dot texture!\n");
            success = false;
        }

        return success;
    }

    void MiscFuncs::close()
    {

        //Free loaded images
        gDotTexture.free();

        //Destroy window
        SDL_DestroyRenderer(gRenderer);
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
        gRenderer = NULL;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

    bool MiscFuncs::init_countdown()
    {
        //The window we'll be rendering to
        SDL_Window *gWindow_countdown = NULL;
        
        //Initialization flag
        bool success = true;

        TTF_Init();
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        gWindow_countdown = SDL_CreateWindow("Countdown", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_COUNTDOWN, SCREEN_HEIGHT_COUNTDOWN, SDL_WINDOW_SHOWN);
        gRenderer_countdown = SDL_CreateRenderer(gWindow_countdown, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawColor(gRenderer_countdown, 0xFF, 0xFF, 0xFF, 0xFF);
        return success;
    }

    bool MiscFuncs::loadMediacountdown()
    {
        //Open the font
        gFont = TTF_OpenFont("/HelveticaNeue.ttf", 28);
        SDL_Color textColor = {0, 0, 0, 255};
        return true;
    }

    void MiscFuncs::close_countdown()
    {
        //Scene textures
        LTexture gTimeTextTexture;
        LTexture gPromptTextTexture;

        //The window we'll be rendering to
        SDL_Window *gWindow_countdown = NULL;
    
        //The window renderer
        SDL_Renderer *gRenderer_countdown = NULL;

        //Free loaded images
        gTimeTextTexture.free();
        gPromptTextTexture.free();

        //Free global font
        TTF_CloseFont(gFont);
        gFont = NULL;

        //Destroy window
        SDL_DestroyRenderer(gRenderer_countdown);
        SDL_DestroyWindow(gWindow_countdown);
        gWindow_countdown = NULL;
        gRenderer_countdown = NULL;

        //Quit SDL subsystems
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    int MiscFuncs::countdown()
    {
        Uint32 startTime = 0;
        int sec = (SDL_GetTicks() * 1000 - startTime) / 1000000; //time incremented in seconds
        int sectest = (SDL_GetTicks() * 1000) / 1000000;
        int sectest2 = 10 - sectest; //change sectest2's constant in coundown() function as well if changing constant here. Defined in taborder.cpp
        //int sectest2 = 75 - sectest;

        if (sectest2 < 0)
        {
            return 0;
        }
        return sectest2;
    }

} // namespace animation