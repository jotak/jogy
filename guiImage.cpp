#include "guiImage.h"
#include "ComponentOwnerInterface.h"

// -----------------------------------------------------------------
// Name : guiImage
//  Constructor
// -----------------------------------------------------------------
guiImage::guiImage() : guiComponent()
{
    m_iWidth = m_iHeight = -1;
    m_bCatchClicks = false;
}

// -----------------------------------------------------------------
// Name : ~guiImage
//  Destructor
// -----------------------------------------------------------------
guiImage::~guiImage()
{
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
guiImage * guiImage::build(ITexture * pTex)
{
    guiComponent::build();
    if (m_iWidth < 0) {
    	m_iWidth = pTex->getWidth();
    }
    if (m_iHeight < 0) {
    	m_iHeight = pTex->getHeight();
    }
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    ((IGeometryQuads*)m_pGeometry)->build(&quad);
    return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiImage::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    m_pGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor * m_DiffuseColor);
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void guiImage::onResize(int iOldWidth, int iOldHeight)
{
    guiComponent::onResize(iOldWidth, iOldHeight);
    if (iOldWidth == m_iWidth && iOldHeight == m_iHeight) {
        return;
    }
    if (m_pGeometry != NULL)
    {
        QuadData quad(0, m_iWidth, 0, m_iHeight, ((IGeometryQuads*)m_pGeometry)->getTexture());
        ((IGeometryQuads*)m_pGeometry)->build(&quad);
    }
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiImage::onButtonEvent(ButtonAction * pEvent)
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
ITexture * guiImage::getImageTexture()
{
    if (m_pGeometry) {
        return ((IGeometryQuads*)m_pGeometry)->getTexture();
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : setImageTexture
// -----------------------------------------------------------------
void guiImage::setImageTexture(ITexture * pTex)
{
    if (m_pGeometry) {
        ((IGeometryQuads*)m_pGeometry)->setTexture(pTex);
    }
}
