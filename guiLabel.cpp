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
}

// -----------------------------------------------------------------
// Name : ~guiLabel
//  Destructor
// -----------------------------------------------------------------
guiLabel::~guiLabel()
{
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiLabel::init(string sText, fontid fontId, Color textColor, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl, IGeometry * pGeometry)
{
    guiComponent::init(sCpntId, xPxl, yPxl, wPxl, hPxl, pGeometry);
    m_FontId = fontId;
    setDiffuseColor(textColor);
    m_iBoxWidth = wPxl;
    m_sText = sText;
    computeGeometry();
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
        setHeight(_font->putStringInBox(m_sText, m_iBoxWidth, m_aiAllFonts[m_FontId]));
        setWidth(_font->getStringLength(m_sText, m_aiAllFonts[m_FontId]));
    } else {
        setWidth(_font->getStringLength(m_sText, m_aiAllFonts[m_FontId]));
        setHeight(_font->getStringHeight(m_sText, m_aiAllFonts[m_FontId]));
    }
    ((GeometryText*)m_pGeometry)->setText(m_sText, m_aiAllFonts[m_FontId]);
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
//
//// -----------------------------------------------------------------
//// Name : createDefaultLabel
////  Static default constructor
//// -----------------------------------------------------------------
//guiLabel * guiLabel::createDefaultLabel(string sText, string sId)
//{
//    guiLabel * pLbl = new guiLabel();
//    pLbl->init(
//    		sText,
//    		TEXT_FONT,
//    		TEXT_COLOR,
//    		sId,
//    		0/*x*/,
//    		0/*y*/,
//    		-1/*w - it will be computed automatically*/,
//    		-1/*h - it will be computed automatically*/);
//    return pLbl;
//}
//
//// -----------------------------------------------------------------
//// Name : createDefaultLabel
////  Static default constructor
//// -----------------------------------------------------------------
//guiLabel * guiLabel::createDefaultLabel(string sText, string sId, int xPxl, int yPxl)
//{
//	guiLabel * pLbl = createDefaultLabel(sText, sId);
//	pLbl->setXPos(xPxl);
//	pLbl->setYPos(yPxl);
//	return pLbl;
//}
