#ifndef CSURFACE_H_INCLUDED
#define CSURFACE_H_INCLUDED

#include <SDL/SDL.h>
#include <fstream>
#include <string>
#include "../global.h"
#include "../tools.h"

class cSurface
{
public:
	cSurface();

	static SDL_Surface* Load(std::string File);
	static SDL_Surface* CreateRGBSurf(int width, int height, int bitdepth, Uint32 rmask, Uint32 gmask, Uint32 bmask, Uint32 amask);

	static bool Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
    static bool Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);
    static bool DrawRectangle(SDL_Surface* Screen,SDL_Rect* input,int Color,int thickness);
};


#endif // CSURFACE_H_INCLUDED
