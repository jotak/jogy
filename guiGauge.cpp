#include "guiGauge.h"
#include "../Geometries/IGeometryQuads.h"

// -----------------------------------------------------------------
// Name : guiGauge
//  Constructor
// -----------------------------------------------------------------
guiGauge::guiGauge() : guiComponent(), m_Color(1,1,1,1)
{
    m_iWidth = m_iHeight = -1;
    m_pForegroundGeometry = NULL;
    m_iRefValue = 1;
    m_iCurValue = 0;
}

// -----------------------------------------------------------------
// Name : ~guiGauge
//  Destructor
// -----------------------------------------------------------------
guiGauge::~guiGauge()
{
    FREE(m_pForegroundGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
guiGauge * guiGauge::build(ITexture * pFgTex, ITexture * pBgTex)
{
    guiComponent::build();
    if (m_iWidth < 0) {
        m_iWidth = pBgTex->getWidth();
    }
    if (m_iHeight < 0) {
        m_iHeight = pBgTex->getHeight();
    }
    QuadData quad(0, m_iWidth, 0, m_iHeight, pBgTex);
    ((IGeometryQuads*)m_pGeometry)->build(&quad);
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, pFgTex);
    m_pForegroundGeometry->build(&quad2);
    return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiGauge::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    m_pForegroundGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, m_Color * cpntColor); // actually display foreground before background
    m_pGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor);
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void guiGauge::onResize(int iOldWidth, int iOldHeight)
{
    guiComponent::onResize(iOldWidth, iOldHeight);
    if (iOldWidth == m_iWidth && iOldHeight == m_iHeight) {
        return;
    }
    if (m_pGeometry != NULL)
    {
        QuadData quad(0, m_iWidth, 0, m_iHeight, ((IGeometryQuads*)m_pGeometry)->getTexture());
        ((IGeometryQuads*)m_pGeometry)->build(&quad);
        int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
        QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
        m_pForegroundGeometry->build(&quad2);
    }
}

// -----------------------------------------------------------------
// Name : setMax
// -----------------------------------------------------------------
void guiGauge::setMax(int iVal)
{
    m_iRefValue = iVal;
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
    m_pForegroundGeometry->build(&quad2);
}

// -----------------------------------------------------------------
// Name : setValue
// -----------------------------------------------------------------
void guiGauge::setValue(int iVal)
{
    m_iCurValue = iVal;
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
    m_pForegroundGeometry->build(&quad2);
}

