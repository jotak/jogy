#ifndef _GUI_TOGGLEBUTTON_H
#define _GUI_TOGGLEBUTTON_H

#include "Button.h"


namespace jogy {

class ToggleButton : public Button
{
public:
    ToggleButton();
    virtual ~ToggleButton();

    virtual u32 getType() { return Button::getType() | GOTYPE_TOGGLEBUTTON; };
    virtual Object * onButtonEvent(ButtonAction * pEvent);
    virtual void setClickState(bool bClickState) { m_bClickState = bClickState; };
    bool getClickState() { return m_bClickState; };
};
}

#endif
