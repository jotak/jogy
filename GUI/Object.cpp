#include "Object.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : Object
//  Constructor
// -----------------------------------------------------------------
Object::Object() : m_DiffuseColor(1,1,1,1)
{
    m_iXPxl = 0;
    m_iYPxl = 0;
    m_iWidth = 0;
    m_iHeight = 0;
    m_sTooltip = "";
}

// -----------------------------------------------------------------
// Name : ~Object
//  Destructor
// -----------------------------------------------------------------
Object::~Object()
{
}

// -----------------------------------------------------------------
// Name : isAt
// -----------------------------------------------------------------
bool Object::isAt(int xPxl, int yPxl)
{
    return (xPxl >= m_iXPxl && yPxl >= m_iYPxl && xPxl <= m_iXPxl + m_iWidth && yPxl <= m_iYPxl + m_iHeight);
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void Object::moveTo(int xPxl, int yPxl)
{
    m_iXPxl = xPxl;
    m_iYPxl = yPxl;
}

// -----------------------------------------------------------------
// Name : moveBy
// -----------------------------------------------------------------
void Object::moveBy(int xPxl, int yPxl)
{
    m_iXPxl += xPxl;
    m_iYPxl += yPxl;
}

// -----------------------------------------------------------------
// Name : setWidth
// -----------------------------------------------------------------
void Object::setWidth(int iWidth)
{
    int oldw = m_iWidth;
    m_iWidth = iWidth;
    onResize(oldw, m_iHeight);
}

// -----------------------------------------------------------------
// Name : setHeight
// -----------------------------------------------------------------
void Object::setHeight(int iHeight)
{
    int oldh = m_iHeight;
    m_iHeight = iHeight;
    onResize(m_iWidth, oldh);
}

// -----------------------------------------------------------------
// Name : setDimensions
// -----------------------------------------------------------------
void Object::setDimensions(int iWidth, int iHeight)
{
    int oldw = m_iWidth;
    m_iWidth = iWidth;
    int oldh = m_iHeight;
    m_iHeight = iHeight;
    onResize(oldw, oldh);
}

// -----------------------------------------------------------------
// Name : centerOn
// -----------------------------------------------------------------
void Object::centerOn(Object * pOther)
{
    moveTo(pOther->getXPos() + (pOther->getWidth() - getWidth()) / 2, pOther->getYPos() + (pOther->getHeight() - getHeight()) / 2);
}

// -----------------------------------------------------------------
// Name : vCenterOn
// -----------------------------------------------------------------
void Object::vCenterOn(Object * pOther)
{
    moveTo(getXPos(), pOther->getYPos() + (pOther->getHeight() - getHeight()) / 2);
}

// -----------------------------------------------------------------
// Name : hCenterOn
// -----------------------------------------------------------------
void Object::hCenterOn(Object * pOther)
{
    moveTo(pOther->getXPos() + (pOther->getWidth() - getWidth()) / 2, getYPos());
}
}
