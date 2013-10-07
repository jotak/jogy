#ifndef _GUI_IMAGE_H
#define _GUI_IMAGE_H

#include "guiComponent.h"

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

    // Clone / init
    virtual void init(ITexture * pTex, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl, IGeometryQuads * pGeometry);

    // Misc.
    void setCatchClicks(bool b) { m_bCatchClicks = b; };
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    ITexture * getImageTexture();
    void setImageTexture(ITexture * pTex);

protected:
    bool m_bCatchClicks;
};

#endif
