#include "Label.h"
#include "../Geometries/IGeometryText.h"
#include "../ComponentOwnerInterface.h"
#include "../jogy.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : Label
//  Constructor
// -----------------------------------------------------------------
Label::Label() : Component()
{
    m_sText = "";
    m_FontId = 0;
    m_iBoxWidth = 0;
    m_bCatchClicks = false;
    m_pComponentOwner = NULL;
    m_pGeometry = NULL;
}

// -----------------------------------------------------------------
// Name : ~Label
//  Destructor
// -----------------------------------------------------------------
Label::~Label()
{
	FREE(m_pGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
Label * Label::build()
{
    Component::build();
    m_iBoxWidth = m_iWidth;
    computeGeometry();
    return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void Label::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    m_pGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor * m_DiffuseColor);
}

// -----------------------------------------------------------------
// Name : setText
// -----------------------------------------------------------------
void Label::setText(string sText)
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
void Label::setBoxWidth(int iWidth)
{
    m_iBoxWidth = iWidth;
    computeGeometry();
}

// -----------------------------------------------------------------
// Name : computeGeometry
// -----------------------------------------------------------------
void Label::computeGeometry()
{
    if (m_iBoxWidth > 0) {
    	Jogy::interface->wrapText(m_sText, m_iBoxWidth, m_FontId);
    }
    setWidth(Jogy::interface->computeTextWidth(m_sText, m_FontId));
    setHeight(Jogy::interface->computeTextHeight(m_sText, m_FontId));
    m_pGeometry->setText(m_sText, m_FontId);
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * Label::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || !m_bCatchClicks) {
        return NULL;
    }
    if (m_pComponentOwner != NULL)
    {
        m_pComponentOwner->onButtonEvent(pEvent);
        return this;
    }
    if (pEvent->eButton != Button1 || pEvent->eEvent != EventDown || !m_bEnabled || !m_bVisible || !m_bCatchClicks) {
        return NULL;
    }
    m_pOwner->onButtonEvent(pEvent, this);
    return this;
}
}
