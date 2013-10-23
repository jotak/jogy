/*
 * ButtonAction.h
 *
 *  Created on: 3 oct. 2013
 *      Author: jotak
 */

#ifndef JOGY_BUTTONACTION_H_
#define JOGY_BUTTONACTION_H_

namespace jogy {

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
    EventNone = 0,
    EventDown,
    EventUp,
    EventClick,
    EventDoubleClick,
    EventDrag
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

} // namespace

#endif /* BUTTONACTION_H_ */
