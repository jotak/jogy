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
    ButtonZ
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
//    unsigned long dragDistance;
//    double clickTime;
//    EventListener * pTargetListener;
};

#endif /* BUTTONACTION_H_ */
