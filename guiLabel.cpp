#include "guiLabel.h"
#include "Geometries/IGeometryText.h"
#include "ComponentOwnerInterface.h"

// -----------------------------------------------------------------
// Name : guiLabel
//  Constructor
// -----------------------------------------------------------------
guiLabel::guiLabel() : guiComponent()
{
    m_sText = "";
    m_FontId = 0;
    m_iBoxWidth = 0;
    m_bCatchClicks = false;
    m_pComponentOwner = NULL;
    m_pGeometry = NULL;
}

// -----------------------------------------------------------------
// Name : ~guiLabel
//  Destructor
// -----------------------------------------------------------------
guiLabel::~guiLabel()
{
	FREE(m_pGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
guiLabel * guiLabel::build()
{
    guiComponent::build();
    m_iBoxWidth = m_iWidth;
    computeGeometry();
    return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiLabel::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    m_pGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor * m_DiffuseColor);
}

// -----------------------------------------------------------------
// Name : setText
// -----------------------------------------------------------------
void guiLabel::setText(string sText)
{
    if (sText == m_sText) {
        return;
    }
    m_sText = sText;
    computeGeometry();
}

// -----------------------------------------------------------------
// Name : setBoxWidth
// -----------------------------------------------------------------
void guiLabel::setBoxWidth(int iWidth)
{
    m_iBoxWidth = iWidth;
    computeGeometry();
}

// -----------------------------------------------------------------
// Name : computeGeometry
// -----------------------------------------------------------------
void guiLabel::computeGeometry()
{
    if (m_iBoxWidth > 0) {
    	Jogy::interface->wrapText(m_sText, m_iBoxWidth, m_FontId);
    }
    setWidth(Jogy::interface->computeTextWidth(m_sText, m_FontId));
    setHeight(Jogy::interface->computeTextHeight(m_sText, m_FontId));
    m_pGeometry->setText(m_sText, m_FontId;
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiLabel::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || !m_bCatchClicks) {
        return NULL;
    }
    if (m_pComponentOwner != NULL)
    {
        m_pComponentOwner->onButtonEvent(pEvent);
        return this;
    }
    if (pEvent->eButton != Button1 || pEvent->eEvent != Event_Down || !m_bEnabled || !m_bVisible || !m_bCatchClicks) {
        return NULL;
    }
    m_pOwner->onButtonEvent(pEvent, this);
    return this;
}
