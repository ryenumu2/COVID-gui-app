/////////////////////////////////////////////////////////////////////////////
// Name:        animation.hpp
// Purpose:     breathing app and timer internal functions
// Author:      Rohith Yenumula
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifndef WX_PRECOMP
#include "wx/wxprec.h"
#include "wx/notebook.h"
#include "wx/app.h"
#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/sizer.h"
#include "wx/panel.h"
#include "wx/msgdlg.h"
#include "wx/button.h"
#include "wx/listbox.h"
#include "wx/stattext.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace animation
{
    // ----------------------------------------------------------------------------
    // declarations and initializations
    // ----------------------------------------------------------------------------

    const int SCREEN_WIDTH_BALL = 640;
    const int SCREEN_HEIGHT_BALL = 480;

    //Screen dimension constants: Timer
    const int SCREEN_WIDTH_COUNTDOWN = 400;
    const int SCREEN_HEIGHT_COUNTDOWN = 80;

    //--------------------------------

    //The window we'll be rendering to
    SDL_Window *gWindow = NULL;

    //The window renderer
    SDL_Renderer *gRenderer = NULL;

    //Texture wrapper class
    class LTexture
    {
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile(std::string path);

        bool loadFromFile_countdown(std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)
        //Creates image from font string
        bool loadFromRenderedText(std::string textureText, SDL_Color color);
        bool loadFromRenderedText_countdown(std::string textureText, SDL_Color textColor);
#endif

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor(Uint8 red, Uint8 green, Uint8 blue);

        //Renders texture at given point
        void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void render_countdown(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture *mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
    };

    //The dot that will move around on the screen
    class Dot
    {
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 10;

        //Initializes the variables
        Dot();

        //Moves the dot
        void move();

        //Shows the dot on the screen
        void render();

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX, mVelY;
    };

    class MiscFuncs
    {
    public:
        //Starts up SDL and creates window
        bool init();

        bool init_countdown();

        //Loads media
        bool loadMedia();

        bool loadMediacountdown();

        //Frees media and shuts down SDL
        void close();

        void close_countdown();

        int countdown();
    };
    //-------------------------------
    //For countdown implementation

    //The window renderer
    SDL_Renderer *gRenderer_countdown = NULL;

    //Globally used font
    TTF_Font *gFont = NULL;

    //Scene textures
    LTexture gPromptTextTexture;
    LTexture gDotTexture;

} // namespace animation