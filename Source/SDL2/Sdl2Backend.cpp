#include <SDL2/Sdl2Backend.h>
#include <SDL2/SDL.h>

namespace Core
{

bool Sdl2Backend::mSdl2Init = false;

Sdl2Backend::Sdl2Backend()
    : mWindow("Modeler3D", 800, 600),
      mGraphics(&mWindow)
{
    // TODO Auto-generated constructor stub

}

Sdl2Backend::~Sdl2Backend()
{
    // TODO Auto-generated destructor stub
}

void Sdl2Backend::Init()
{
//    InitSdl2();

    mGraphics.Init();
}

void Sdl2Backend::Destroy()
{
}

void Sdl2Backend::InitSdl2()
{
    if (mSdl2Init) return;

    SDL_Init(SDL_INIT_EVERYTHING);
    mSdl2Init = true;
}

}
