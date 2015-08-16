#ifndef CEVENTHANDLER_H_INCLUDED
#define CEVENTHANDLER_H_INCLUDED

#include "cEvent.h"

class cEventHandler : public cEvent
{
protected:
    // List of the keys
    bool keyA;
    bool keyM;
    bool keyS;
    bool keyEscape;
    bool keyUp;
    bool keyDown;
    bool keyLeft;
    bool keyRight;
    bool keyControl;
    bool keyTab;
    bool keyNum[10];
    bool keyAlt;
    bool keyAbility[4];
    bool keyShift;

    // List of the MouseButtons
    bool LMButton;

    bool RMButton;
    bool MMButton;

    //Mouse position
    int MouseX;
    int MouseY;

    //Coordinates of Mousedown
    int LBcX;
    int LBcY;
    int LmXd;
    int LmYd;
    int RBcX;
    int RBcY;
public:
    cEventHandler();
    void InitEventHandler();

    void PollEvents();
    bool UseEvent(SDL_Event* Event);

    //handling Key events
    void InpKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void InpKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    //hanlding Mosue movement
    void InpMouseMove(int mX, int mY, int relX, int relY, bool LeftB, bool RightB, bool MiddleB);

    //handling mouse button inputs
    void InpMouseButtonDown(Uint8 button,int mX, int mY);
    void InpMouseButtonUp(Uint8 button,int mX, int mY);

    bool checkKeyA();
    void setKeyA(bool key);
    bool checkKeyM();
    void setKeyM(bool key);
    bool checkKeyS();
    void setKeyS(bool key);
    bool checkKeyEscape();
    void setKeyEscape(bool key);
    bool checkKeyUp();
    void setKeyUp(bool key);
    bool checkKeyDown();
    void setKeyDown(bool key);
    bool checkKeyLeft();
    void setKeyLeft(bool key);
    bool checkKeyRight();
    void setKeyRight(bool key);
    bool checkKeyControl();
    void setKeyControl(bool key);
    bool checkKeyTab();
    void setKeyTab(bool key);
    bool checkKeyNum(int num);
    void setKeyNum(int num, bool key);
    bool checkKeyAlt();
    void setKeyAlt(bool key);
    bool checkKeyAbility(int num);
    void setKeyAbility(int num, bool key);
    bool checkKeyShift();
    void setKeyShift(bool key);

    bool checkLeftMouseButton();
    void setLeftMouseButton(bool set);
    bool checkRightMouseButton();
    void setRightMouseButton(bool set);

    int getMouseX();
    int getMouseY();

    int getLeftMouseDownX();
    int getLeftMouseDownY();
    void setLeftMouseDown(int x, int y);
    int getRightMouseDownX();
    int getRightMouseDownY();
    void setRightMouseDown(int x, int y);
};

#endif // CEVENTHANDLER_H_INCLUDED
