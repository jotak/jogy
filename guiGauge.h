#ifndef _GUI_GAUGE_H
#define _GUI_GAUGE_H

#include "guiComponent.h"

class guiGauge : public guiComponent
{
public:
    // Constructor / destructor
    guiGauge();
    virtual ~guiGauge();

    // Inherited functions
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Size and position
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Other
    void setMax(int iVal);
    void setValue(int iVal);

    // Builder
    virtual guiGauge * build();
    guiGauge * withGauge(int reference, int value) {
    	m_iRefValue = reference; m_iCurValue = value; return this;
    }
    guiGauge * withColor(Color color) {
    	m_Color = color; return this;
    }
    guiGauge * withForeground(ITexture * pTex, IGeometryQuads * pGeo);
    guiGauge * withBackground(ITexture * pTex, IGeometryQuads * pGeo);

private:
    IGeometryQuads * m_pForegroundGeometry;
    IGeometryQuads * m_pBackgroundGeometry;
    int m_iRefValue;
    int m_iCurValue;
    Color m_Color;

    void rebuildGeometries();
};

#endif
