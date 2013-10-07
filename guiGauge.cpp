#include "guiGauge.h"
#include "../Geometries/GeometryQuads.h"
#include "../Display/TextureEngine.h"
#include "../Display/DisplayEngine.h"

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
// Name : init
// -----------------------------------------------------------------
void guiGauge::init(int iRef, int iVal, Color color, Texture * pFgTex, Texture * pBgTex, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl)
{
    guiComponent::init(sCpntId, xPxl, yPxl, wPxl, hPxl);
    m_iRefValue = iRef;
    m_iCurValue = iVal;
    m_Color = color;
    if (m_iWidth < 0)
        m_iWidth = pBgTex->getWidth();
    if (m_iHeight < 0)
        m_iHeight = pBgTex->getHeight();
    QuadData quad(0, m_iWidth, 0, m_iHeight, pBgTex);
    m_pGeometry = new GeometryQuads(&quad, VB_Static);
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, pFgTex);
    m_pForegroundGeometry = new GeometryQuads(&quad2, VB_Static);
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
guiObject * guiGauge::clone()
{
    guiGauge * pObj = new guiGauge();
    pObj->init(m_iRefValue, m_iCurValue, m_Color, m_pForegroundGeometry->getTexture(), ((GeometryQuads*)m_pGeometry)->getTexture(), m_sCpntId, m_iXPxl, m_iYPxl, m_iWidth, m_iHeight);
    return pObj;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiGauge::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    CoordsScreen coords = CoordsScreen(m_iXPxl + iXOffset, m_iYPxl + iYOffset, GUIPLANE);
    m_pForegroundGeometry->display(coords, m_Color * cpntColor); // actually display foreground before background
    m_pGeometry->display(coords, cpntColor);
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
        QuadData quad(0, m_iWidth, 0, m_iHeight, ((GeometryQuads*)m_pGeometry)->getTexture());
        ((GeometryQuads*)m_pGeometry)->modify(&quad);
        int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
        QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
        m_pForegroundGeometry->modify(&quad2);
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
    m_pForegroundGeometry->modify(&quad2);
}

// -----------------------------------------------------------------
// Name : setValue
// -----------------------------------------------------------------
void guiGauge::setValue(int iVal)
{
    m_iCurValue = iVal;
    int fgw = max(1, m_iWidth * m_iCurValue / m_iRefValue);
    QuadData quad2(0, fgw, 0, m_iHeight, m_pForegroundGeometry->getTexture());
    m_pForegroundGeometry->modify(&quad2);
}

// -----------------------------------------------------------------
// Name : createDefaultGauge
//  Static default constructor
// -----------------------------------------------------------------
guiGauge * guiGauge::createDefaultGauge(int iRef, Color color, int iWidth, int iHeight, string sId)
{
    guiGauge * pGauge = new guiGauge();
    pGauge->init(
        iRef, 0, color,
        _tex->findTexture("gui/interface:GaugeFg"),
        _tex->findTexture("gui/interface:GaugeBg"),
        sId, 0, 0, iWidth, iHeight);
    return pGauge;
}
