#include "Gauge.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : Gauge
//  Constructor
// -----------------------------------------------------------------
Gauge::Gauge() : Component(), m_Color(1,1,1,1)
{
    m_iWidth = m_iHeight = -1;
    m_pForegroundGeometry = NULL;
    m_pBackgroundGeometry = NULL;
    m_iRefValue = 1;
    m_iCurValue = 0;
}

// -----------------------------------------------------------------
// Name : ~Gauge
//  Destructor
// -----------------------------------------------------------------
Gauge::~Gauge()
{
    FREE(m_pForegroundGeometry);
    FREE(m_pBackgroundGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
Gauge * Gauge::build()
{
    Component::build();
    if (m_iWidth < 0) {
        m_iWidth = m_pBackgroundGeometry->getTexture()->getWidth();
    }
    if (m_iHeight < 0) {
        m_iHeight = m_pBackgroundGeometry->getTexture()->getHeight();
    }
    rebuildGeometries();
    return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void Gauge::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    m_pForegroundGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, m_Color * cpntColor); // actually display foreground before background
    m_pBackgroundGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor);
}

// -----------------------------------------------------------------
// Name : rebuildGeometries
// -----------------------------------------------------------------
void Gauge::rebuildGeometries()
{
    QuadData quad(0, m_iWidth, 0, m_iHeight, m_pBackgroundGeometry->getTexture());
    m_pBackgroundGeometry->build(&quad);
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
    m_pForegroundGeometry->build(&quad2);
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void Gauge::onResize(int iOldWidth, int iOldHeight)
{
    Component::onResize(iOldWidth, iOldHeight);
    if (iOldWidth == m_iWidth && iOldHeight == m_iHeight) {
        return;
    }
    if (m_pBackgroundGeometry != NULL) {
    	rebuildGeometries();
    }
}

// -----------------------------------------------------------------
// Name : setMax
// -----------------------------------------------------------------
void Gauge::setMax(int iVal)
{
    m_iRefValue = iVal;
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
    m_pForegroundGeometry->build(&quad2);
}

// -----------------------------------------------------------------
// Name : setValue
// -----------------------------------------------------------------
void Gauge::setValue(int iVal)
{
    m_iCurValue = iVal;
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
    m_pForegroundGeometry->build(&quad2);
}
}
