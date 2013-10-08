#ifndef _GUI_GAUGE_H
#define _GUI_GAUGE_H

#include "guiComponent.h"

class GeometryQuads;

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
    virtual guiGauge * build(ITexture * pFgTex, ITexture * pBgTex);
    guiGauge * withGauge(int reference, int value) {
    	m_iRefValue = reference; m_iCurValue = value; return this;
    }
    guiGauge * withColor(Color color) {
    	m_Color = color; return this;
    }
    guiGauge * withForeground(IGeometryQuads * pFgGeo) {
    	m_pForegroundGeometry = pFgGeo; return this;
    }
    guiGauge * withBackground(IGeometryQuads * pBgGeo) {
    	withGeometry(pBgGeo); return this;
    }

protected:
    IGeometryQuads * m_pForegroundGeometry;
    int m_iRefValue;
    int m_iCurValue;
    Color m_Color;
};

#endif
