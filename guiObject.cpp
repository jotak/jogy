#include "guiObject.h"

int guiObject::m_aiAllFonts[];

// -----------------------------------------------------------------
// Name : guiObject
//  Constructor
// -----------------------------------------------------------------
guiObject::guiObject() : GraphicObject(), m_DiffuseColor(1,1,1,1)
{
    m_iXPxl = 0;
    m_iYPxl = 0;
    m_iWidth = 0;
    m_iHeight = 0;
    m_sTooltip = "";
}

// -----------------------------------------------------------------
// Name : ~guiObject
//  Destructor
// -----------------------------------------------------------------
guiObject::~guiObject()
{
}

// -----------------------------------------------------------------
// Name : isAt
// -----------------------------------------------------------------
bool guiObject::isAt(int xPxl, int yPxl)
{
    return (xPxl >= m_iXPxl && yPxl >= m_iYPxl && xPxl <= m_iXPxl + m_iWidth && yPxl <= m_iYPxl + m_iHeight);
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void guiObject::moveTo(int xPxl, int yPxl)
{
    m_iXPxl = xPxl;
    m_iYPxl = yPxl;
}

// -----------------------------------------------------------------
// Name : moveBy
// -----------------------------------------------------------------
void guiObject::moveBy(int xPxl, int yPxl)
{
    m_iXPxl += xPxl;
    m_iYPxl += yPxl;
}

// -----------------------------------------------------------------
// Name : setWidth
// -----------------------------------------------------------------
void guiObject::setWidth(int iWidth)
{
    int oldw = m_iWidth;
    m_iWidth = iWidth;
    onResize(oldw, m_iHeight);
}

// -----------------------------------------------------------------
// Name : setHeight
// -----------------------------------------------------------------
void guiObject::setHeight(int iHeight)
{
    int oldh = m_iHeight;
    m_iHeight = iHeight;
    onResize(m_iWidth, oldh);
}

// -----------------------------------------------------------------
// Name : setDimensions
// -----------------------------------------------------------------
void guiObject::setDimensions(int iWidth, int iHeight)
{
    int oldw = m_iWidth;
    m_iWidth = iWidth;
    int oldh = m_iHeight;
    m_iHeight = iHeight;
    onResize(oldw, oldh);
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiObject::init(int xPxl, int yPxl, int wPxl, int hPxl, IGeometry * pGeometry)
{
    m_iXPxl = xPxl;
    m_iYPxl = yPxl;
    m_iWidth = wPxl;
    m_iHeight = hPxl;
    m_pGeometry = pGeometry;
}

// -----------------------------------------------------------------
// Name : centerOn
// -----------------------------------------------------------------
void guiObject::centerOn(guiObject * pOther)
{
    moveTo(pOther->getXPos() + (pOther->getWidth() - getWidth()) / 2, pOther->getYPos() + (pOther->getHeight() - getHeight()) / 2);
}

// -----------------------------------------------------------------
// Name : vCenterOn
// -----------------------------------------------------------------
void guiObject::vCenterOn(guiObject * pOther)
{
    moveTo(getXPos(), pOther->getYPos() + (pOther->getHeight() - getHeight()) / 2);
}

// -----------------------------------------------------------------
// Name : hCenterOn
// -----------------------------------------------------------------
void guiObject::hCenterOn(guiObject * pOther)
{
    moveTo(pOther->getXPos() + (pOther->getWidth() - getWidth()) / 2, getYPos());
}

// -----------------------------------------------------------------
// Name : registerTextures
//  Static function
// -----------------------------------------------------------------
void guiObject::registerTextures(const fontid * pFonts, unsigned int nbFonts)
{
	memcpy(m_aiAllFonts, pFonts, nbFonts * sizeof(fontid));
}
