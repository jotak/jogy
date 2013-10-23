#include "Image.h"
#include "../ComponentOwnerInterface.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : Image
//  Constructor
// -----------------------------------------------------------------
Image::Image() : Component()
{
    m_iWidth = m_iHeight = -1;
    m_bCatchClicks = false;
    m_pGeometry = NULL;
}

// -----------------------------------------------------------------
// Name : ~Image
//  Destructor
// -----------------------------------------------------------------
Image::~Image()
{
	FREE(m_pGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
Image * Image::build()
{
    Component::build();
    if (m_iWidth < 0) {
    	m_iWidth = m_pGeometry->getTexture()->getWidth();
    }
    if (m_iHeight < 0) {
    	m_iHeight = m_pGeometry->getTexture()->getHeight();
    }
    return this;
}

// -----------------------------------------------------------------
// Name : withGeometry
// -----------------------------------------------------------------
Image * Image::withGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometry = pGeo;
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void Image::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    m_pGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor * m_DiffuseColor);
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void Image::onResize(int iOldWidth, int iOldHeight)
{
    Component::onResize(iOldWidth, iOldHeight);
    if (iOldWidth == m_iWidth && iOldHeight == m_iHeight) {
        return;
    }
    if (m_pGeometry != NULL) {
        QuadData quad(0, m_iWidth, 0, m_iHeight, m_pGeometry->getTexture());
        m_pGeometry->build(&quad);
    }
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * Image::onButtonEvent(ButtonAction * pEvent)
{
    if (pEvent->eButton != Button1 || /*pEvent->eEvent != Event_Down || */!m_bEnabled || !m_bVisible || !m_bCatchClicks) {
        return NULL;
    }
    m_pOwner->onButtonEvent(pEvent, this);
    return this;
}

// -----------------------------------------------------------------
// Name : getImageTexture
// -----------------------------------------------------------------
ITexture * Image::getImageTexture()
{
    if (m_pGeometry) {
        return m_pGeometry->getTexture();
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : setImageTexture
// -----------------------------------------------------------------
void Image::setImageTexture(ITexture * pTex)
{
    if (m_pGeometry) {
        m_pGeometry->setTexture(pTex);
    }
}
}
