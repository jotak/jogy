#ifndef _GUI_IMAGE_H
#define _GUI_IMAGE_H

#include "guiComponent.h"
#include "Utils/ITexture.h"

class guiImage : public guiComponent
{
public:
    // Constructor / destructor
    guiImage();
    virtual ~guiImage();

    // Inherited functions
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Size and position
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Misc.
    void setCatchClicks(bool b) { m_bCatchClicks = b; };
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    ITexture * getImageTexture();
    void setImageTexture(ITexture * pTex);

    // Builder
    virtual guiImage * build(ITexture * pTex);

protected:
    bool m_bCatchClicks;
};

#endif
