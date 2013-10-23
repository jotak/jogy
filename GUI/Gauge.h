#ifndef _GUI_GAUGE_H
#define _GUI_GAUGE_H

#include "Component.h"
#include "../Geometries/IGeometryQuads.h"


namespace jogy {

class Gauge : public Component
{
public:
    // Constructor / destructor
    Gauge();
    virtual ~Gauge();

    // Inherited functions
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Size and position
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Other
    void setMax(int iVal);
    void setValue(int iVal);

    // Builder
    virtual Gauge * build();
    Gauge * withGauge(int reference, int value) {
    	m_iRefValue = reference; m_iCurValue = value; return this;
    }
    Gauge * withColor(Color color) {
    	m_Color = color; return this;
    }
    Gauge * withForeground(ITexture * pTex, IGeometryQuads * pGeo);
    Gauge * withBackground(ITexture * pTex, IGeometryQuads * pGeo);

private:
    IGeometryQuads * m_pForegroundGeometry;
    IGeometryQuads * m_pBackgroundGeometry;
    int m_iRefValue;
    int m_iCurValue;
    Color m_Color;

    void rebuildGeometries();
};
}

#endif
