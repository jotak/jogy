#ifndef _GUI_IMAGE_H
#define _GUI_IMAGE_H

#include "guiComponent.h"
#include "Geometries/IGeometryQuads.h"

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
    virtual guiImage * build();
    guiImage * withGeometry(ITexture * pTex, IGeometryQuads * pGeo);

protected:
    bool m_bCatchClicks;
    IGeometryQuads * m_pGeometry;
};

#endif
