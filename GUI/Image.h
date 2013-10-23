#ifndef _GUI_IMAGE_H
#define _GUI_IMAGE_H

#include "Component.h"
#include "../Geometries/IGeometryQuads.h"


namespace jogy {

class Image : public Component
{
public:
    // Constructor / destructor
    Image();
    virtual ~Image();

    // Inherited functions
    virtual u32 getType() { return GOTYPE_IMAGE; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Size and position
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Misc.
    void setCatchClicks(bool b) { m_bCatchClicks = b; };
    virtual Object * onButtonEvent(ButtonAction * pEvent);
    ITexture * getImageTexture();
    void setImageTexture(ITexture * pTex);

    // Builder
    virtual Image * build();
    Image * withGeometry(ITexture * pTex, IGeometryQuads * pGeo);

protected:
    bool m_bCatchClicks;
    IGeometryQuads * m_pGeometry;
};

}

#endif
