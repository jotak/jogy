/*
 * ButtonAction.h
 *
 *  Created on: 3 oct. 2013
 *      Author: jotak
 */

#ifndef BUTTONACTION_H_
#define BUTTONACTION_H_


enum InputButton
{
    Button1 = 0,
    Button2,
    ButtonX,
    ButtonZ,
    ButtonLeft,
    ButtonRight,
    ButtonUp,
    ButtonDown,
    ButtonHome,
    ButtonEnd
};

enum ButtonEvent
{
    Event_None = 0,
    Event_Down,
    Event_Up,
    Event_Click,
    Event_DoubleClick,
    Event_Drag
};

class ButtonAction
{
public:
    InputButton eButton;
    ButtonEvent eEvent;
    int xPos, yPos;
    int xPosInit, yPosInit;
    int xOffset, yOffset;
};

#endif /* BUTTONACTION_H_ */
