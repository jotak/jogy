#include "guiToggleButton.h"
#include "ComponentOwnerInterface.h"
#include "../Display/TextureEngine.h"

// -----------------------------------------------------------------
// Name : guiToggleButton
//  Constructor
// -----------------------------------------------------------------
guiToggleButton::guiToggleButton() : guiButton()
{
}

// -----------------------------------------------------------------
// Name : ~guiToggleButton
//  Destructor
// -----------------------------------------------------------------
guiToggleButton::~guiToggleButton()
{
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
guiObject * guiToggleButton::clone()
{
    guiToggleButton * pObj = new guiToggleButton();
    pObj->init(m_pLabel->getText(), m_pLabel->getFontId(), m_pLabel->getDiffuseColor(), (m_pGeometryClicked == NULL) ? NULL : m_pGeometryClicked->getTexture(), m_ClickOption, (m_pGeometryOver == NULL) ? NULL : m_pGeometryOver->getTexture(), m_OverOption, m_pGeometryNormal->getTexture(), m_sCpntId, m_iXPxl, m_iYPxl, m_iWidth, m_iHeight);
    return pObj;
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiToggleButton::onButtonEvent(ButtonAction * pEvent)
{
    if ((pEvent->eButton != Button1 && (pEvent->eButton != Button2 || !m_bCatchButton2Events)) || !m_bEnabled || !m_bVisible) {
        return NULL;
    }
    bool bClickState = m_bClickState;
    bool bMouseOver = m_bMouseOver;
    guiObject * pObj = (pEvent->eEvent != Event_Up) ? guiButton::onButtonEvent(pEvent) : this;
    if (pEvent->eEvent == Event_Down)
    {
        m_bClickState = !bClickState;
        return this;
    }
    else if (pEvent->eEvent == Event_Drag)
    {
        m_bClickState = m_bMouseOver == bMouseOver ? bClickState : !bClickState;
        return this;
    }
    else if (pEvent->eEvent == Event_Up)
    {
        m_bMouseDown = false;
        if (m_bMouseOver && m_pOwner != NULL)
            m_pOwner->onButtonEvent(pEvent, this);
        return this;
    }
    return pObj;
}

// -----------------------------------------------------------------
// Name : createDefaultTexturedToggleButton
//  Static default constructor
//  Use it to avoid passing always the same 3591218 arguments to "init"
// -----------------------------------------------------------------
guiToggleButton * guiToggleButton::createDefaultTexturedToggleButton(Texture * pTex, int iSize, string sId)
{
    guiToggleButton * pBtn = new guiToggleButton();
    pBtn->init(
        "", (FontId)0, Color(0,0,0),
        _tex->findTexture("gui/interface:Selector"),
        BCO_AddTex, NULL, BCO_None, pTex, sId, 0, 0, iSize, iSize);
    return pBtn;
}

// -----------------------------------------------------------------
// Name : createDefaultCheckBox
//  Static default constructor
//  Use it to avoid passing always the same 3591218 arguments to "init"
// -----------------------------------------------------------------
guiToggleButton * guiToggleButton::createDefaultCheckBox(string sId)
{
    guiToggleButton * pBtn = new guiToggleButton();
    pBtn->init(
        "", (FontId)0, Color(0,0,0),
        _tex->findTexture("gui/interface:CheckBoxTick"),
        BCO_AddTex, NULL, BCO_None,
        _tex->findTexture("gui/interface:Transparent"),
        sId, 0, 0, 23, 23);
//  pBtn->m_pLabel->setCatchClicks(true);
//  pBtn->m_pLabel->setComponentOwner(pBtn);
    return pBtn;
}
