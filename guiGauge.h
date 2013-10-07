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

    // Clone / init
    virtual void init(int iRef, int iVal, Color color, Texture * pFgTex, Texture * pBgTex, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl);

    // Static default constructors
    static guiGauge * createDefaultGauge(int iRef, Color color, int iWidth, int iHeight, string sId);

    // Other
    void setMax(int iVal);
    void setValue(int iVal);

protected:
    GeometryQuads * m_pForegroundGeometry;
    int m_iRefValue;
    int m_iCurValue;
    Color m_Color;
};

#endif
