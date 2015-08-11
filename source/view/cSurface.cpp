#include "cSurface.h"

cSurface::cSurface()
{
}

SDL_Surface* cSurface::Load(std::string File)
{
	SDL_Surface* Surf_Temp = NULL;
	SDL_Surface* Surf_Return = NULL;

	if((Surf_Temp = SDL_LoadBMP(File.c_str())) == NULL)
	{
	    Error("Fehler beim Oberfläche laden ");
	    Error(File);
	    Error("\n");
        return NULL;
	}

	if(SDL_SetColorKey(Surf_Temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Temp->format, 255, 0 ,255)) == -1)
	{
	    Error("Warning: colorkey will not be used, reason: %s\n");
	    Error(SDL_GetError());
    }

	Surf_Return = SDL_DisplayFormat(Surf_Temp);
	SDL_FreeSurface(Surf_Temp);

	return Surf_Return;
}

SDL_Surface* cSurface::CreateRGBSurf(int width, int height, int bitdepth, Uint32 rmask, Uint32 gmask, Uint32 bmask, Uint32 amask)
{
	SDL_Surface* Surf_Temp = NULL;
	SDL_Surface* Surf_Return = NULL;

	if((Surf_Temp = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, bitdepth, rmask, gmask, bmask,amask)) == NULL)
	{
	    return NULL;
	    Error("Fehler beim Oberfläche laden ");
	    Error("\n");
    }

	if(SDL_SetColorKey(Surf_Temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Temp->format, 255, 0 ,255)) == -1) {Error("Warning: colorkey will not be used, reason: %s\n"); Error(SDL_GetError());}

	Surf_Return = SDL_DisplayFormat(Surf_Temp);
	SDL_FreeSurface(Surf_Temp);

	return Surf_Return;
}

bool cSurface::Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y)
{
	if(Surf_Dest == NULL || Surf_Src == NULL)
	{
	    return false;
	    Error("Fehler beim Oberfläche blitten \n");
    }

	SDL_Rect DestR;

	DestR.x = X;
	DestR.y = Y;

	SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);

	return true;
}

bool cSurface::DrawRectangle(SDL_Surface* Screen,SDL_Rect* input,int Color,int thickness)
{
    SDL_Rect rect;

    //Top Line
    rect.x = input->x;
    rect.y = input->y;
    rect.w = input->w;
    rect.h = thickness;
    SDL_FillRect(Screen,&rect,Color);

    //Left line
    rect.x = input->x;
    rect.y = input->y;
    rect.w = thickness;
    rect.h = input->h;
    SDL_FillRect(Screen,&rect,Color);

    //Bottom line
    rect.x = input->x;
    rect.y = input->h + input->y;
    rect.w = input->w;
    rect.h = thickness;
    SDL_FillRect(Screen,&rect,Color);

    //Right line
    rect.x = input->w + input->x;
    rect.y = input->y;
    rect.w = thickness;
    rect.h = input->h;
    SDL_FillRect(Screen,&rect,Color);

    return true;

}

bool cSurface::Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H)
{
	if(Surf_Dest == NULL || Surf_Src == NULL)
	{
	    return false;
//	    Error("Fehler beim Oberfläche blitten \n");
    }

	SDL_Rect DestR;

	DestR.x = X;
	DestR.y = Y;

	SDL_Rect SrcR;

	SrcR.x = X2;
	SrcR.y = Y2;
	SrcR.w = W;
	SrcR.h = H;

	SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);

	return true;
}
