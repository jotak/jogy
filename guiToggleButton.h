#ifndef _GUI_TOGGLEBUTTON_H
#define _GUI_TOGGLEBUTTON_H

#include "guiButton.h"

class guiToggleButton : public guiButton
{
public:
    guiToggleButton();
    virtual ~guiToggleButton();

    virtual u32 getType() { return guiButton::getType() | GOTYPE_TOGGLEBUTTON; };
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    virtual void setClickState(bool bClickState) { m_bClickState = bClickState; };
    bool getClickState() { return m_bClickState; };

    // Static default constructors
    static guiToggleButton * createDefaultTexturedToggleButton(Texture * pTex, int iSize, string sId);
    static guiToggleButton * createDefaultCheckBox(string sId);
};

#endif
