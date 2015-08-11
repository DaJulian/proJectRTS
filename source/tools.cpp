#include "tools.h"

//calculates the distance between two points in the way the game needs it; diagonal paths count sqqrt(2) times non
//diagonal. Direct paths always means the path a unit would walk without obstacles.
float realDist(int xS,int yS,int xG,int yG)
{
    int relX,relY;
    float result;
    relX = abs(xS - xG);
    relY = abs(yS - yG);
    if(relX > relY)
    {
        result = (relX - relY) + relY*1.4142135623;
    }
    else
    {
        result = (relY - relX) + relX*1.4142135623;
    }
    return result;
}

//The power of an integer to an integer as an intger ;) easy function; not optimized
int pot(int base, int power)
{
    int result;
    result = base;
    if(power > 0)
    {
        for(int i=1;i<power;i++)
        {
            result = result*base;
        }
        return result;
    }
    return 1;
}

//The following 2 functions allay conversion from and to 4byte char strings to use them in the net protocol

//converts an Integer to a Char[4]; gives back a pointer to the array
char* intToChar4(int Integer)
{
    char* Character;
    Character = new char[4];
    int test2;
    for(int i=0; i<4;i++)
    {
        Character[i] = 0;
        test2 = Integer%256;
        Character[i] = char(test2);
        Integer = (Integer - Integer%256)/256;
    }
    return Character;
}
//converts a 4 byte char array to an integer;
int Char4ToInt(char Character[4])
{
    int output,sample;
    output = 0;
    for(int i=0; i<4;i++)
    {
        sample = Character[i];
        if(sample<0) sample+=256;
        output += sample*pot(256,i);
    }
    return output;
}



//Error output functions TODO clean file usage
void Error(std::string Error)
{
	std::fstream f;
	f.open("Error.log", std::ios::out | std::ios::app);
	f << Error << "\n";
	f.close();
}
void Error(std::string Error,int integer)
{
	std::fstream f;
	f.open("Error.log", std::ios::out | std::ios::app);
	f << Error << integer << "\n";
	f.close();
}
void Error(std::string Error,float floa)
{
	std::fstream f;
	f.open("Error.log", std::ios::out | std::ios::app);
	f << Error << floa << "\n";
	f.close();
}
void Error2(std::string Error)
{
	std::fstream f;
	f.open("Error2.log", std::ios::out | std::ios::app);
	f << Error << "\n";
	f.close();
}
void Error2(std::string Error,int integer)
{
	std::fstream f;
	f.open("Error2.log", std::ios::out | std::ios::app);
	f << Error << integer << "\n";
	f.close();
}
void Error2(std::string Error,float floa)
{
	std::fstream f;
	f.open("Error2.log", std::ios::out | std::ios::app);
	f << Error << floa << "\n";
	f.close();
}
