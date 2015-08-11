#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include "global.h"

void Error(std::string message);
void Error(std::string message,int integer);
void Error(std::string message,float floa);

void Error2(std::string message);
void Error2(std::string message,int integer);
void Error2(std::string message,float floa);

float realDist(int xS,int yS,int xG,int yG);

char* intToChar4(int Integer);
int Char4ToInt(char Character[4]);

int pot(int base, int power);

#endif // TOOLS_H_INCLUDED
